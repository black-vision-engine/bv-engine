#include "stdafxBVApp.h"

#include "SceneEventsHandlers.h"

#include "Engine/Editors/BVProjectEditor.h"
#include "BVAppLogic.h"
#include "UseLoggerBVAppModule.h"

#include "ProjectManager.h"
#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Engine/Models/Plugins/Simple/TextPlugins/DefaultTextPlugin.h"

#include "Engine/Models/NodeLogics/MeshLoader/MeshLoader.h"

#include "Engine/Events/EventHandlerHelpers.h"
#include "Engine/Models/BoundingVolume.h"

#include "System/Path.h"
#include "IO/FileIO.h"

#include "Application/ApplicationContext.h"

#include <limits>
#undef LoadImageW
#undef LoadImage

#include "LibImage.h"
#include "DataTypes/Hash.h"
#include "Assets/Thumbnail/Impl/SceneThumbnail.h"
#include "Assets/Thumbnail/Impl/PresetThumbnail.h"
#include "Assets/Thumbnail/Impl/MeshAssetThumbnail.h"

#include "Mathematics/Interpolators/CompositeInterpolator.h"

namespace bv
{


// ***********************
//
SceneEventsHandlers::SceneEventsHandlers( BVAppLogic* logic )
    : m_appLogic( logic )
    , m_closeSavedPreset( false )
    , m_savedScene( nullptr )
{
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &SceneEventsHandlers::ThumbnailRendered ), ScreenShotRenderedEvent::Type() );
}

SceneEventsHandlers::~SceneEventsHandlers()
{}

// ***********************
//
void SceneEventsHandlers::SceneStructure    ( bv::IEventPtr evt )
{
    assert( evt->GetEventType() == bv::SceneEvent::Type() );

    bv::SceneEventPtr sceneEvent = std::static_pointer_cast<bv::SceneEvent>( evt );

    std::string & sceneName = sceneEvent->SceneName;
    std::string & newSceneName = sceneEvent->NewSceneName;
    auto attachIndex = sceneEvent->AttachIndex;
    auto command = sceneEvent->SceneCommand;
    auto eventID = sceneEvent->EventID;

    bool result = true;
    auto editor = m_appLogic->GetBVProject()->GetProjectEditor();


    switch( command )
    {
        case SceneEvent::Command::AddScene:
            editor->AddScene( newSceneName );
            break;
        case SceneEvent::Command::RemoveScene:
            result = editor->RemoveScene( sceneName );
            break;
        case SceneEvent::Command::RemoveAllScenes:
			if (sceneName != "")
			{
				editor->RemoveAllScenes(attachIndex);
			}
			else {
				editor->RemoveAllScenes();
			}
            break;
        case SceneEvent::Command::SetSceneVisible:
            result = editor->SetSceneVisible( sceneName, true );
            break;
        case SceneEvent::Command::SetSceneInvisible:
            result = editor->SetSceneVisible( sceneName, false );
            break;
        case SceneEvent::Command::RenameScene:
            result = editor->RenameScene( sceneName, newSceneName );
            break;
        case SceneEvent::Command::AttachScene:
            result = editor->AttachScene( sceneName, attachIndex );
            break;
        case SceneEvent::Command::DetachScene:
            result = editor->DetachScene( sceneName );
            break;
        case SceneEvent::Command::MoveScene:
            result = editor->MoveScene( sceneName, attachIndex );
            break;
        case SceneEvent::Command::CopyScene:
        {
            auto sceneCopy = editor->AddSceneCopy( sceneName );
            if( sceneCopy == nullptr )
                result = false;
            break;
        }
        case SceneEvent::Command::SetOutputChannel:
            editor->SetSceneOutputChannel( sceneName, attachIndex );
            result = true;
            break;
        default:
            result = false;
    }

    SendSimpleResponse( command, eventID, sceneEvent->SocketID, result );
}

// ***********************
//
void SceneEventsHandlers::NodeStructure      ( bv::IEventPtr evt )
{
    assert( evt->GetEventType() == bv::NodeStructureEvent::Type() );

    bv::NodeStructureEventPtr structureEvent = std::static_pointer_cast<bv::NodeStructureEvent>( evt );

    std::string& sceneName        = structureEvent->SceneName;
    std::string& nodePath        = structureEvent->NodePath;
    std::string& newNodeName    = structureEvent->NewNodeName;    
    auto attachIndex            = structureEvent->AttachIndex;
    auto eventID                = structureEvent->EventID;
    auto command                = structureEvent->SceneCommand;

    IDeserializer * request        = structureEvent->Request;

    bool result = true;
    auto editor = m_appLogic->GetBVProject()->GetProjectEditor();

    // FIXME: should be set externally
    bool enableUndo = true;

    if( command == NodeStructureEvent::Command::AddNode )
    {
        result = editor->AddChildNode( sceneName, nodePath, newNodeName, enableUndo );
        
        bool AddTransformPlugin = false;
        if( request && request->GetAttribute( "AddTransformPlugin" ) == "true" )
            AddTransformPlugin = true;

        if( AddTransformPlugin && result )
        {
            auto parentNode = editor->GetNode( sceneName, nodePath );
            assert( parentNode );
            auto parentNodeCasted = std::static_pointer_cast< model::BasicNode >( parentNode );

            unsigned int lastChildIdx = parentNodeCasted->GetNumChildren() - 1;
            auto addedChild = parentNodeCasted->GetChild( lastChildIdx );

            auto timeline = editor->GetSceneDefaultTimeline( editor->GetModelScene( sceneName ) );
            auto plugin = model::PluginsManager::DefaultInstance().CreatePlugin( "DEFAULT_TRANSFORM", "transform", timeline );

            UInt32 addIdx = 0;
            editor->AddPlugin( addedChild, plugin, addIdx );
        }
    }
    else if( command == NodeStructureEvent::Command::RemoveNode )
    {
        result = editor->DeleteChildNode( sceneName, nodePath, enableUndo );
    }
    else if( command == NodeStructureEvent::Command::SetNodeVisible )
    {
        result = editor->SetNodeVisible( sceneName, nodePath, true );
    }
    else if( command == NodeStructureEvent::Command::SetNodeInvisible )
    {
        result = editor->SetNodeVisible( sceneName, nodePath, false );
    }
    else if( command == NodeStructureEvent::Command::SelectNode )
    {
        auto node = editor->GetNode( sceneName, nodePath );

        if( node )
        {
            auto color = Convert::String2T< glm::vec4 >( newNodeName, glm::vec4( 1, 1, 1, 1 ) );

            result = editor->SelectNode( node, color );
        }

    }
    else if( command == NodeStructureEvent::Command::UnselectNodes )
    {
        editor->UnselectNodes();
        result = true;
    }
    else if( command == NodeStructureEvent::Command::RenameNode )
    {
        result = editor->RenameNode( sceneName, nodePath, newNodeName );
    }
    else if( command == NodeStructureEvent::Command::AttachNode )
    {
        result = editor->AttachChildNode( sceneName, nodePath, attachIndex );
    }
    else if( command == NodeStructureEvent::Command::DetachNode )
    {
        result = editor->DetachChildNode( sceneName, nodePath );
    }
    else if( command == NodeStructureEvent::Command::MoveNode )
    {
        assert( request != nullptr );
        if( request == nullptr )
        {
            SendSimpleErrorResponse( NodeStructureEvent::Command::MoveNode, eventID, structureEvent->SocketID, "Not valid request." );
            return;
        }

        //FIXME: replace with sth more generic
        auto destSceneName = request->GetAttribute( "DestSceneName" );
        auto destNodePath = request->GetAttribute( "DestPath" );
        auto destIdx = Convert::String2T< UInt32 >( request->GetAttribute( "DestIndex" ), 0 );
        auto srcSceneName = request->GetAttribute( "SrcSceneName" );
        auto srcNodePath = request->GetAttribute( "SrcPath" );
        
        result = editor->MoveNode( destSceneName, destNodePath, destIdx, srcSceneName, srcNodePath, enableUndo );
    }
    else if( command == NodeStructureEvent::Command::CopyNode )
    {
        assert( request != nullptr );
        if( request == nullptr )
        {
            SendSimpleErrorResponse( NodeStructureEvent::Command::CopyNode, eventID, structureEvent->SocketID, "Not valid request." );
            return;
        }

        //FIXME: replace with sth more generic
        auto destSceneName = request->GetAttribute( "DestSceneName" );
        auto destNodePath = request->GetAttribute( "DestPath" );
        auto srcSceneName = request->GetAttribute( "SrcSceneName" );
        auto srcNodePath = request->GetAttribute( "SrcPath" );

        auto copyPtr = editor->AddNodeCopy( destSceneName, destNodePath, srcSceneName, srcNodePath, enableUndo );
        if( copyPtr == nullptr )
            result = false;
    }
    else result = false;

    SendSimpleResponse( command, eventID, structureEvent->SocketID, result );
}

// ***********************
//
void SceneEventsHandlers::PluginStructure     ( bv::IEventPtr evt )
{
    assert( evt->GetEventType() == bv::PluginStructureEvent::Type() );

    bv::PluginStructureEventPtr structureEvent = std::static_pointer_cast<bv::PluginStructureEvent>( evt );

    std::string& nodePath        = structureEvent->NodePath;
    std::string& sceneName        = structureEvent->SceneName;
    std::string& pluginName        = structureEvent->PluginName;
    std::string& pluginUID        = structureEvent->PluginUID;
    std::string& timelinePath    = structureEvent->TimelinePath;
    unsigned int attachIndex    = structureEvent->AttachIndex;
    auto command                = structureEvent->PluginCommand;
    auto eventID                = structureEvent->EventID;
    IDeserializer * request        = structureEvent->Request;


    bool result = true;
    auto editor = m_appLogic->GetBVProject()->GetProjectEditor();


    // FIXME: should be set externally
    bool enableUndo = true;

    if( command == PluginStructureEvent::Command::AddPlugin )
    {
        result = editor->AddPlugin( sceneName, nodePath, pluginUID, pluginName, timelinePath, attachIndex, enableUndo );
    }
    else if( command == PluginStructureEvent::Command::RemovePlugin )
    {
        result = editor->DeletePlugin( sceneName, nodePath, pluginName, enableUndo );
    }
    else if( command == PluginStructureEvent::Command::AttachPlugin )
    {
        result = editor->AttachPlugin( sceneName, nodePath, attachIndex );
    }
    else if( command == PluginStructureEvent::Command::DetachPlugin )
    {
        result = editor->DetachPlugin( sceneName, nodePath, pluginName );
    }
    else if( command == PluginStructureEvent::Command::CopyPlugin )
    {
        assert( request != nullptr );
        if( request == nullptr )
        {
            SendSimpleErrorResponse( PluginStructureEvent::Command::CopyPlugin, eventID, structureEvent->SocketID, "Not valid request." );
            return;
        }

        //FIXME: replace with sth more generic
        auto destSceneName = request->GetAttribute( "SrcSceneName" );
        auto destNodePath = request->GetAttribute( "DestPath" );
        auto destIdx = Convert::String2T< UInt32 >( request->GetAttribute( "DestIndex" ), 0 );
        auto srcSceneName = request->GetAttribute( "SrcSceneName" );
        auto srcNodePath = request->GetAttribute( "SrcPath" );
        auto srcPluginName = request->GetAttribute( "SrcName" );

        auto pluginPtr = editor->AddPluginCopy( destSceneName, destNodePath, destIdx, srcSceneName, srcNodePath, srcPluginName, enableUndo );
        if( pluginPtr == nullptr )
            result = false;
    }
    else if( command == PluginStructureEvent::Command::MovePlugin )
    {
        assert( request != nullptr );
        if( request == nullptr )
        {
            SendSimpleErrorResponse( PluginStructureEvent::Command::MovePlugin, eventID, structureEvent->SocketID, "Not valid request." );
            return;
        }

        //FIXME: replace with sth more generic
        auto destSceneName = request->GetAttribute( "SrcSceneName" );
        auto destNodePath = request->GetAttribute( "DestPath" );
        auto destIdx = Convert::String2T< UInt32 >( request->GetAttribute( "DestIndex" ), 0 );
        auto srcSceneName = request->GetAttribute( "SrcSceneName" );
        auto srcNodePath = request->GetAttribute( "SrcPath" );
        auto srcPluginName = request->GetAttribute( "SrcName" );

        result = editor->MovePlugin( destSceneName, destNodePath, destIdx, srcSceneName, srcNodePath, srcPluginName, enableUndo );
    }
    else result = false;

    SendSimpleResponse( command, eventID, structureEvent->SocketID, result );
}

// ***********************
//
void SceneEventsHandlers::ProjectStructure    ( bv::IEventPtr evt )
{
    assert( evt->GetEventType() == bv::ProjectEvent::Type() );

    bv::ProjectEventPtr projectEvent = std::static_pointer_cast<bv::ProjectEvent>( evt );

    auto pm = ProjectManager::GetInstance();

    IDeserializer& request = *projectEvent->Request;
    auto command = projectEvent->ProjectCommand;
    int senderID = projectEvent->SocketID;

    assert( projectEvent->Request != nullptr );
    if( projectEvent->Request == nullptr )
    {
        SendSimpleErrorResponse( command, projectEvent->EventID, senderID, "Wrong request" );
        return;
    }


    if( command == ProjectEvent::Command::NewProject )
    {
        auto name = request.GetAttribute( "projectName" );

        pm->AddNewProject( name );

        SendSimpleResponse( command, projectEvent->EventID, senderID, true );
    }
    else if( command == ProjectEvent::Command::SetCurrentProject )
    {
        auto projName = request.GetAttribute( "projectName" );

        pm->SetCurrentProject( projName );

        SendSimpleResponse( command, projectEvent->EventID, senderID, true );
    }
    else if( command == ProjectEvent::Command::LoadProject )
    {
        auto projName = request.GetAttribute( "projectName" );

        auto projectScenesNames = pm->ListScenesNames( projName, "", true );

        bool status = false;

        if( !projectScenesNames.empty() )
        {
            m_appLogic->LoadScenes( projectScenesNames );
            status = true;
        }

        if( status )
            SendSimpleResponse( command, projectEvent->EventID, senderID, true );
        else
            SendSimpleResponse( command, projectEvent->EventID, senderID, false );
    }
    else if( command == ProjectEvent::Command::CopyAsset )
    {
        auto categoryName = request.GetAttribute( "category" );

        auto srcAssetName = request.GetAttribute( "srcAssetName" );
        auto dstAssetName = request.GetAttribute( "dstAssetName" );

        pm->CopyAsset( "", categoryName, srcAssetName, "", dstAssetName );

        SendSimpleResponse( command, projectEvent->EventID, senderID, true );
    }
    else if( command == ProjectEvent::Command::MoveAsset )
    {
        auto categoryName = request.GetAttribute( "category" );

        auto srcAssetName = request.GetAttribute( "srcAssetName" );
        auto dstAssetName = request.GetAttribute( "dstAssetName" );

        pm->MoveAsset( "", categoryName, srcAssetName, "", dstAssetName );

        SendSimpleResponse( command, projectEvent->EventID, senderID, true );
    }
    else if( command == ProjectEvent::Command::RemoveAsset )
    {
        auto categoryName = request.GetAttribute( "category" );

        auto assetName = request.GetAttribute( "assetName" );

        pm->RemoveAsset( "", categoryName, assetName );

        SendSimpleResponse( command, projectEvent->EventID, senderID, true );
    }
    else if( command == ProjectEvent::Command::ImportAsset )
    {
        auto categoryName = request.GetAttribute( "category" );

        auto assetFilePath = request.GetAttribute( "assetFilePath" );

        auto dstAssetPath = request.GetAttribute( "dstAssetPath" );

        pm->ImportAssetFromFile( "", categoryName, dstAssetPath, assetFilePath );

        SendSimpleResponse( command, projectEvent->EventID, senderID, true );
    }
    else if( command == ProjectEvent::Command::SaveScene )
    {
        auto sceneName = request.GetAttribute( "sceneName" );

        auto saveTo = request.GetAttribute( "saveTo" );

        auto ext = File::GetExtension( saveTo );

        if( ext.empty() || ext != ".scn" )
        {
            saveTo += ".scn";
        }

        if( saveTo.empty() || Path::IsValisPathName( saveTo ) )
        {
            auto forceSaveStr = request.GetAttribute( "forceSave" );
            bool renameScene = Convert::String2T( request.GetAttribute( "renameScene" ), true );

            bool forceSave = false;

            if( forceSaveStr == "true" )
            {
                forceSave = true;
            }

            model::SceneModelPtr scene = m_appLogic->GetBVProject()->GetModelScene( sceneName );

            if( scene != nullptr )
            {
                auto newSceneName = !saveTo.empty() ? saveTo : scene->GetName();

                m_appLogic->GetBVProject()->GetProjectEditor()->RenameScene( sceneName, newSceneName );

                scene = m_appLogic->GetBVProject()->GetModelScene( newSceneName );

                if( scene != nullptr )
                {
                    if( Path::Exists( newSceneName ) && !forceSave )
                    {
                        SendSimpleErrorResponse( command, projectEvent->EventID, senderID, ( "Scene name [" + saveTo + "] already exists. Use forceSave to overwrite file." ).c_str() );
                    }
                    else
                    {
                        pm->AddScene( scene, "", newSceneName );
                        
                        if( !renameScene )
                        {
                            // Set previous name.
                            m_appLogic->GetBVProject()->GetProjectEditor()->RenameScene( newSceneName, sceneName );
                        }

                        RequestThumbnail( scene, newSceneName, ThumbnailType::Scene );

                        SendSimpleResponse( command, projectEvent->EventID, senderID, true );
                    }
                }
            }
            else
            {
                SendSimpleErrorResponse( command, projectEvent->EventID, senderID, "Scene not found." );
            }
        }
        else
        {
            SendSimpleErrorResponse( command, projectEvent->EventID, senderID, ( "Scene name '" + saveTo + "' is not valid system path." ).c_str() );
        }
    }
    else if( command == ProjectEvent::Command::LoadScene )
    {
		auto sceneName = request.GetAttribute("sceneName");
		auto sceneNameTarget = request.GetAttribute("sceneNameTarget");
		auto destIdx = Convert::String2T< UInt32 >(request.GetAttribute("destIndex"), 0);
	

        auto scene = pm->LoadScene( "", sceneName );

		

        if( scene.IsValid() )
        {
			model::SceneModelPtr modelScene = scene;
			if (sceneNameTarget != "")
			{
				
				modelScene->SetName(sceneNameTarget);
			}
			modelScene->SetRenderChannelIdx(destIdx);
            m_appLogic->GetBVProject()->GetProjectEditor()->AddScene( scene );
            SendSimpleResponse( command, projectEvent->EventID, senderID, true );
        }
        else
        {
            SendSimpleErrorResponse( command, projectEvent->EventID, senderID, scene.GetErrorReason().c_str() );
        }
    }
    else if( command == ProjectEvent::Command::RemoveScene )
    {
        auto sceneName = request.GetAttribute( "sceneName" );
        auto projectName = request.GetAttribute( "projectName" );

        pm->RemoveScene( projectName, sceneName );

        SendSimpleResponse( command, projectEvent->EventID, senderID, true );
    }
    else if( command == ProjectEvent::Command::CopyScene )
    {
        auto sceneName = request.GetAttribute( "sceneName" );
        auto destSceneName = request.GetAttribute( "destToSceneName" );

        pm->CopyScene( "", sceneName, "", destSceneName );

        SendSimpleResponse( command, projectEvent->EventID, senderID, true );
    }
    else if( command == ProjectEvent::Command::MoveScene )
    {
        auto sceneName = request.GetAttribute( "sceneName" );
        auto destSceneName = request.GetAttribute( "destSceneName" );

        pm->MoveScene( "", sceneName, "", destSceneName );

        SendSimpleResponse( command, projectEvent->EventID, senderID, true );
    }
    else if( command == ProjectEvent::Command::MoveProject )
    {
        assert( false ); // TODO: Implement
    }
    else if( command == ProjectEvent::Command::DeleteProject )
    {
        assert( false ); // TODO: Implement
    }
    else if( command == ProjectEvent::Command::RenameProject )
    {
        assert( false ); // TODO: Implement
    }
    else if( command == ProjectEvent::Command::AddExistingSceneToProject )
    {
        auto inSceneName = request.GetAttribute( "inSceneName" );
        auto projectName = request.GetAttribute( "projectName" );
        auto outSceneName = request.GetAttribute( "outSceneName" );

        pm->CopyScene( "", inSceneName, projectName, outSceneName );

        SendSimpleResponse( command, projectEvent->EventID, senderID, true );
    }
    else if( command == ProjectEvent::Command::CreateSceneInProject )
    {
        auto inSceneName = request.GetAttribute( "inSceneName" );
        auto projectName = request.GetAttribute( "projectName" );

        auto scene = model::SceneModel::Create( ( Path( projectName ) / Path( inSceneName ) ).Str() );

        pm->AddScene( scene, projectName, inSceneName );

        SendSimpleResponse( command, projectEvent->EventID, senderID, true );
    }
    else if( command == ProjectEvent::Command::SavePreset )
    {
        auto destProjectName = request.GetAttribute( "DestProjectName" );
        auto destPath = request.GetAttribute( "DestPath" );
        auto sceneName = request.GetAttribute( "SceneName" );
        auto nodePath = request.GetAttribute( "NodePath" );
        m_closeSavedPreset = Convert::String2T( request.GetAttribute( "CloseSavedPreset" ), false );

        auto editor = m_appLogic->GetBVProject()->GetProjectEditor();

        auto n = editor->GetNode( sceneName, nodePath );
        assert( std::dynamic_pointer_cast< model::BasicNode >( n ) );
        auto bn = std::static_pointer_cast< model::BasicNode >( n );

        if( bn != nullptr )
        {
            pm->SavePreset( bn, destProjectName, destPath );
            SendSimpleResponse( command, projectEvent->EventID, senderID, true );

            auto scene = editor->GetModelScene( sceneName );

            // Thumbnails
            if( !IsPresetScene( sceneName ) )
            {
                // Copy node and create temporary preset scene
                auto tempSceneName = File::GetFileName( destPath ) + File::GetExtension( destPath );

                assert( editor->GetModelScene( tempSceneName ) == nullptr ); // Scene name shouldn't exist.

                editor->AddScene( tempSceneName );
                auto tempScene = editor->GetModelScene( tempSceneName );
                
                auto timeline = editor->GetTimeEvaluator( tempSceneName );
                assert( std::dynamic_pointer_cast< model::OffsetTimeEvaluator >( timeline ) );
                auto offsetTimeline = std::static_pointer_cast< model::OffsetTimeEvaluator >( timeline );

                auto node = pm->LoadPreset( destProjectName, destPath, offsetTimeline );
                editor->AddChildNode( tempScene, nullptr, node );

                for( UInt32 i = 0; i < scene->NumLights(); ++i )
                {
                    tempScene->AddLight( scene->GetLight( i ) );
                }

                scene = tempScene;
            }

            RequestThumbnail( scene, destPath, ThumbnailType::Preset );
        }
        else
        {
            SendSimpleErrorResponse( command, projectEvent->EventID, senderID, "Node not found." );
        }
    }
    else if( command == ProjectEvent::Command::LoadPreset )
    {
        auto projectName = request.GetAttribute( "ProjectName" );
        auto path = request.GetAttribute( "Path" );
        auto sceneName = request.GetAttribute( "SceneName" );
        auto nodePath = request.GetAttribute( "NodePath" );

        auto editor = m_appLogic->GetBVProject()->GetProjectEditor();

        auto timeline = editor->GetTimeEvaluator( sceneName );

        assert( std::dynamic_pointer_cast< model::OffsetTimeEvaluator >( timeline ) );
        auto offsetTimeline = std::static_pointer_cast< model::OffsetTimeEvaluator >( timeline );

        auto node = pm->LoadPreset( projectName, path, offsetTimeline );

        auto parentNode = editor->GetNode( sceneName, nodePath );
        auto scene = editor->GetModelScene( sceneName );

        auto success = false;

        if( node && parentNode && scene )
        {
            success = editor->AddChildNode( scene, parentNode, node );
        }

        if( success )
        {
            SendSimpleResponse( command, projectEvent->EventID, senderID, true );
        }
        else
        {
            SendSimpleErrorResponse( command, projectEvent->EventID, senderID, "Cannot load preset" );
        }
    }
    else if( command == ProjectEvent::Command::EditPreset )
    {
        auto projectName = request.GetAttribute( "ProjectName" );
        auto path = request.GetAttribute( "Path" );

        auto editor = m_appLogic->GetBVProject()->GetProjectEditor();

        std::string sceneName = File::GetFileName( path );
        editor->AddScene( sceneName );
        auto scene = editor->GetModelScene( sceneName );

        auto timeline = editor->GetTimeEvaluator( sceneName );

        assert( std::dynamic_pointer_cast< model::OffsetTimeEvaluator >( timeline ) );
        auto offsetTimeline = std::static_pointer_cast< model::OffsetTimeEvaluator >( timeline );

        auto node = pm->LoadPreset( projectName, path, offsetTimeline );
        if( node != nullptr )
        {
            bool result = editor->AddChildNode( scene, scene->GetRootNode(), node );
            SendSimpleResponse( command, projectEvent->EventID, senderID, result );
        }
        else
        {
            editor->RemoveScene( scene );
            SendSimpleErrorResponse( command, projectEvent->EventID, senderID, "Cannot load preset" );
        }
    }
    else if( command == ProjectEvent::Command::CreateFolder )
    {
        auto categoryName = request.GetAttribute( "categoryName" );
        auto path = request.GetAttribute( "path" );

        auto recursive = false;
        auto recStr = request.GetAttribute( "recursive" );
        if( recStr == "true" )
        {
            recursive = true;
        }

        bool success = false;
        if( categoryName == "scenes" )
        {
            success = pm->CreateSceneDir( path, recursive );
        }
        else if( categoryName == "presets" )
        {
            success = pm->CreatePresetDir( path, recursive );
        }
        else
        {
            success = pm->CreateAssetDir( categoryName, path, recursive );
        }

        SendSimpleResponse( command, projectEvent->EventID, senderID, success );
    }
    else if( command == ProjectEvent::Command::DeleteFolder )
    {
        auto categoryName = request.GetAttribute( "categoryName" );
        auto path = request.GetAttribute( "path" );

        bool success = false;
        if( categoryName == "scenes" )
        {
            success = pm->RemoveSceneDir( path );
        }
        else if( categoryName == "presets" )
        {
            success = pm->RemovePresetDir( path );
        }
        else
        {
            success = pm->RemoveAssetDir( categoryName, path );
        }

        SendSimpleResponse( command, projectEvent->EventID, senderID, success );
    }
    else if( command == ProjectEvent::Command::RenameFolder )
    {
        auto categoryName = request.GetAttribute( "categoryName" );
        auto path = request.GetAttribute( "path" );
        auto path2 = request.GetAttribute( "newName" );

        bool success = false;
        if( categoryName == "scenes" )
        {
            success = pm->RenameSceneDir( path, path2 );
        }
        else if( categoryName == "presets" )
        {
            success = pm->RenamePresetDir( path, path2 );
        }
        else
        {
            success = pm->RenameAssetDir( categoryName, path, path2 );
        }

        SendSimpleResponse( command, projectEvent->EventID, senderID, success );
    }
    else if( command == ProjectEvent::Command::CopyFolder )
    {
        auto categoryName = request.GetAttribute( "categoryName" );
        auto path = request.GetAttribute( "path" );
        auto path2 = request.GetAttribute( "newName" );

        bool success = false;
        if( categoryName == "scenes" )
        {
            success = pm->CopySceneDir( path, path2 );
        }
        else if( categoryName == "presets" )
        {
            success = pm->CopyPresetDir( path, path2 );
        }
        else
        {
            success = pm->CopyAssetDir( categoryName, path, path2 );
        }

        SendSimpleResponse( command, projectEvent->EventID, senderID, success );
    }
    else if( command == ProjectEvent::Command::MoveFolder )
    {
        auto categoryName = request.GetAttribute( "categoryName" );
        auto path = request.GetAttribute( "path" );
        auto path2 = request.GetAttribute( "newName" );

        bool success = false;
        if( categoryName == "scenes" )
        {
            success = pm->CopySceneDir( path, path2 );
            success = success && pm->RemoveSceneDir( path );
        }
        else if( categoryName == "presets" )
        {
            success = pm->CopyPresetDir( path, path2 );
            success = success && pm->RemovePresetDir( path );
        }
        else
        {
            success = pm->CopyAssetDir( categoryName, path, path2 );
            success = success && pm->RemoveAssetDir( categoryName, path );
        }

        SendSimpleResponse( command, projectEvent->EventID, senderID, success );
    }
    else if( command == ProjectEvent::Command::GenerateMeshThumbnail )//temp testing event
    {
        auto assetPath = request.GetAttribute( "assetFilePath" );
        auto destPath = request.GetAttribute( "destAssetFilePath" );
        GenerateMeshThumbnail( assetPath, destPath );
    }
    else
    {
        SendSimpleErrorResponse( command, projectEvent->EventID, senderID, "Unknown command" );
    }

}

// ***********************
//
void SceneEventsHandlers::LightsManagement    ( IEventPtr evt )
{
    assert( evt->GetEventType() == LightEvent::Type() );

    auto lightEvent = std::static_pointer_cast< LightEvent >( evt );

    auto command      = lightEvent->SceneCommand;
    auto sceneName    = lightEvent->SceneName;
    auto lightType    = lightEvent->LightType;
    auto lightIdx     = lightEvent->LightIndex;
    auto timelinePath = lightEvent->TimelinePath;

    auto editor = m_appLogic->GetBVProject()->GetProjectEditor();
    
    bool result = false;

    if( command == LightEvent::Command::AddLight )
    {
        result = editor->AddLight( sceneName, lightType, timelinePath );
    }
    else if( command == LightEvent::Command::RemoveLight )
    {
        result = editor->RemoveLight( sceneName, lightIdx );
    }

    SendSimpleResponse( command, lightEvent->EventID, lightEvent->SocketID, result );
}

// ***********************
//
void SceneEventsHandlers::CameraManagement    ( bv::IEventPtr evt )
{
    assert( evt->GetEventType() == CameraEvent::Type() );

    auto cameraEvent = std::static_pointer_cast< CameraEvent >( evt );

    auto command      = cameraEvent->CameraCommand;
    auto sceneName    = cameraEvent->SceneName;
    auto cameraIdx    = cameraEvent->CameraIndex;
    //auto timelinePath = cameraEvent->TimelinePath;

    auto editor = m_appLogic->GetBVProject()->GetProjectEditor();
    
    bool result = false;

    if( command == CameraEvent::Command::AddCamera )
    {
        result = editor->AddCamera( sceneName );
    }
    else if( command == CameraEvent::Command::RemoveCamera )
    {
        result = editor->RemoveCamera( sceneName, cameraIdx );
    }
    else if( command == CameraEvent::Command::SetCurrentCamera )
    {
        result = editor->SetCurrentCamera( sceneName, cameraIdx );
    }

    SendSimpleResponse( command, cameraEvent->EventID, cameraEvent->SocketID, result );
}

// ***********************
//
void        SceneEventsHandlers::SceneVariable       ( bv::IEventPtr evt )
{
    assert( evt->GetEventType() == bv::SceneVariableEvent::Type() );

    SceneVariableEventPtr sceneVarEvent = std::static_pointer_cast< bv::SceneVariableEvent >( evt );
    std::string & sceneName         = sceneVarEvent->SceneName;
    std::string & variableName      = sceneVarEvent->VariableName;
    std::string & variableContent   = sceneVarEvent->VariableContent;
    auto command                    = sceneVarEvent->VariableCommand;

    auto sceneModel = m_appLogic->GetBVProject()->GetProjectEditor()->GetModelScene( sceneName );
    if( sceneModel == nullptr )
    {
        SendSimpleErrorResponse( command, sceneVarEvent->EventID, sceneVarEvent->SocketID, "Scene not found" );
        return;
    }

    auto & variablesCollection = sceneModel->GetSceneVariables();

    if( command == SceneVariableEvent::Command::AddVariable )
    {
        bool result = variablesCollection.AddVariable( variableName, variableContent );
        SendSimpleResponse( command, sceneVarEvent->EventID, sceneVarEvent->SocketID, result );
    }
    else if( command == SceneVariableEvent::Command::GetVariable )
    {
        SimpleJsonSerializeObject ser;
        Expected< std::string > varContent = variablesCollection.GetVariable( variableName );

        if( !varContent.IsValid() )
        {
            SendSimpleErrorResponse( command, sceneVarEvent->EventID, sceneVarEvent->SocketID, "Variable not found" );
            return;
        }

        ser.SetAttribute( "VariableContent", varContent );
        
        PrepareResponseTemplate( ser, command, sceneVarEvent->EventID, true );
        SendResponse( ser, sceneVarEvent->SocketID, sceneVarEvent->EventID );
    }
    else if( command == SceneVariableEvent::Command::DeleteVariable )
    {
        bool result = variablesCollection.DeleteVariable( variableName );
        SendSimpleResponse( command, sceneVarEvent->EventID, sceneVarEvent->SocketID, result );
    }
    else
        SendSimpleErrorResponse( command, sceneVarEvent->EventID, sceneVarEvent->SocketID, "Unknown command" );
}

// ***********************
//
void        SceneEventsHandlers::GridLines           ( bv::IEventPtr evt )
{
    assert( evt->GetEventType() == bv::GridLineEvent::Type() );

    GridLineEventPtr gridLineEvent = std::static_pointer_cast< bv::GridLineEvent >( evt );

    auto command        = gridLineEvent->GridLineCommand;
    auto gridPosition   = gridLineEvent->GridLinePosition;
    auto & nodeName     = gridLineEvent->NodeName;
    auto & sceneName    = gridLineEvent->SceneName;
    auto & gridName     = gridLineEvent->GridLineName;
    auto gridLineIdx    = gridLineEvent->GridLineIndex;
    auto gridType       = Convert::String2T( gridLineEvent->GridLineType, model::GridLineType::TST_Horizontal );
    auto alignement     = Convert::String2T( gridLineEvent->AlignementType, model::GridLineAlignement::TSA_WeightCenter );

    auto scene = m_appLogic->GetBVProject()->GetModelScene( sceneName );
    if( scene == nullptr )
    {
        SendSimpleErrorResponse( command, gridLineEvent->EventID, gridLineEvent->SocketID, "Scene not found" );
        return;
    }

    auto & gridLinesLogic = scene->GetGridLinesLogic();
    bool result = true;

    if( command == GridLineEvent::Command::SetGridLinePosition )
    {
        gridLinesLogic.MoveGridLine( gridType, gridLineIdx, gridPosition );
    }
    else if( command == GridLineEvent::Command::RenameGridLine )
    {
        gridLinesLogic.RenameGridLine( gridType, gridLineIdx, gridName );
    }
    else if( command == GridLineEvent::Command::RemoveGridLine )
    {
        gridLinesLogic.RemoveGridLine( gridType, gridLineIdx );
    }
    else if( command == GridLineEvent::Command::AlignToGridLine )
    {
        auto editor = m_appLogic->GetBVProject()->GetProjectEditor();
        auto modelScene = m_appLogic->GetBVProject()->GetModelScene( sceneName );

        auto node = editor->GetNode( sceneName, nodeName );
    
        if( node != nullptr )
            SendSimpleErrorResponse( command, gridLineEvent->EventID, gridLineEvent->SocketID, "Node not found" );

        auto basicNode = std::static_pointer_cast< model::BasicNode >( node );
        result = gridLinesLogic.AlignNodeToGridLine( gridType, gridLineIdx, basicNode, alignement );
    }
    else if( command == GridLineEvent::Command::ShowGridLines )
    {
        gridLinesLogic.ShowGridLines( scene, m_appLogic->GetBVProject()->GetProjectEditor(), true );
    }
    else if( command == GridLineEvent::Command::HideGridLines )
    {
        gridLinesLogic.ShowGridLines( scene, m_appLogic->GetBVProject()->GetProjectEditor(), false );
    }
    else
        SendSimpleErrorResponse( command, gridLineEvent->EventID, gridLineEvent->SocketID, "Unknown command" );

    SendSimpleResponse( command, gridLineEvent->EventID, gridLineEvent->SocketID, result );
}


// ========================================================================= //
// Thumbnails functions
// ========================================================================= //

// ***********************
//
void        SceneEventsHandlers::RequestThumbnail       ( model::SceneModelPtr scene, const std::string & saveTo, ThumbnailType thumbnailType )
{
    // Save state and make all scenes invisible.
    SaveVisibilityState( scene->GetName() );

    std::string sceneName = scene->GetName();

    Path prefixDir = GetPrefixDir( thumbnailType );

    Path sceneScreenShot( saveTo );
    auto parentPath = sceneScreenShot.ParentPath();     // Extract directory
    sceneScreenShot = File::GetFileName( sceneScreenShot.Str(), true ) + ".bmp";   
    sceneScreenShot = ProjectManager::GetInstance()->GetRootDir() / prefixDir / parentPath / sceneScreenShot;

    m_thumbnailTypeMap[ sceneScreenShot.Str() ] = thumbnailType;

    m_appLogic->GetRenderMode().MakeScreenShot( sceneScreenShot.Str(), true, false );

    GetDefaultEventManager().LockEvents( 1 );   // Lock events for one frame, to protect scenes from changeing visibility and other parameters.
}

// ***********************
//
void        SceneEventsHandlers::ThumbnailRendered   ( bv::IEventPtr evt )
{
    if( evt->GetEventType() != bv::ScreenShotRenderedEvent::Type() )
        return;
    
    ScreenShotRenderedEventPtr screenShotEvent = std::static_pointer_cast< ScreenShotRenderedEvent >( evt );

    if( screenShotEvent->Result )
    {
        // FIXME: Load image, resize to 128,128 and create thumbnail
        UInt32 height;
        UInt32 width;
        UInt32 bpp;
        UInt32 channelNum;

        auto chunk = bv::image::LoadImage( screenShotEvent->FilePath, &width, &height, &bpp, &channelNum );
        auto resizedChunk = image::MakeThumbnai( chunk, width, height, bpp, 128 );
        auto compresed = image::SaveTGAToHandle( resizedChunk, 128, 128, 32 );

        std::string thumbName = std::string( screenShotEvent->FilePath.begin(), screenShotEvent->FilePath.begin() + ( screenShotEvent->FilePath.find_last_of( ".bmp" ) - 3 ));

        auto thumbnailType = m_thumbnailTypeMap[ screenShotEvent->FilePath ];

        //image::SaveBMPImage( screenShotEvent->FilePath, resizedChunk, 128, 128, bpp );

        auto thumb = CreateThumbnail( thumbnailType, compresed );

        SimpleJsonSerializeObject ser;
        thumb->Serialize( ser );        
        
        ser.Save( AssetAccessor::GetThumbnailPath( thumbName ).Str() );
        Path::Remove( screenShotEvent->FilePath );

        auto editor = m_appLogic->GetBVProject()->GetProjectEditor();
        if( ( thumbnailType == ThumbnailType::Preset && m_closeSavedPreset )
                || thumbnailType == ThumbnailType::MeshAsset )
        {
            editor->RemoveScene( m_savedScene );
            m_scenesVisibilityState.erase( m_savedScene );
        }
    }

    RestoreVisibilityState();
}

// ***********************
//
void        SceneEventsHandlers::SaveVisibilityState     ( const std::string & sceneName )
{
    m_scenesVisibilityState.clear();
    auto editor = m_appLogic->GetBVProject()->GetProjectEditor();

    for( auto scene : m_appLogic->GetBVProject()->GetModelScenes() )
    {
        m_scenesVisibilityState[ scene ] = scene->GetRootNode()->IsVisible();
        scene->GetRootNode()->SetVisible( false );
    }

    auto scene = editor->GetModelScene( sceneName );
    scene->GetRootNode()->SetVisible( true );

    m_savedScene = scene;
}

// ***********************
//
void        SceneEventsHandlers::RestoreVisibilityState  ()
{
    for( auto scene : m_scenesVisibilityState )
    {
        auto root = scene.first->GetRootNode();
        
        // For some reason root node can not exist anymore.
        if( root )
        {
            root->SetVisible( scene.second );
        }
    }

    m_scenesVisibilityState.clear();
    m_savedScene = nullptr;
}

// ***********************
//
ThumbnailConstPtr       SceneEventsHandlers::CreateThumbnail        ( ThumbnailType thumbnailType, MemoryChunkConstPtr data )
{
    switch( thumbnailType )
    {
    case ThumbnailType::Scene:
        return SceneThumbnail::Create( data );
    case ThumbnailType::Preset:
        return PresetThumbnail::Create( data );
    case ThumbnailType::MeshAsset:
        return MeshAssetThumbnail::Create( data );
    default:
        return nullptr;
    }
}

// ***********************
//
std::string             SceneEventsHandlers::GetPrefixDir           ( ThumbnailType thumbnailType )
{
    switch( thumbnailType )
    {
    case ThumbnailType::Scene:
        return "scenes";
    case ThumbnailType::Preset:
        return "presets";
    case ThumbnailType::MeshAsset:
        return "meshes";
    default:
        return "";
    }
}

// ***********************
//
void        SceneEventsHandlers::GenerateMeshThumbnail               ( const std::string & meshPath, const std::string & destPath )
{
    auto editor = m_appLogic->GetBVProject()->GetProjectEditor();

    auto sceneName = File::GetFileName( meshPath );

    editor->AddScene( sceneName );

    auto scene = editor->GetModelScene( sceneName );
    auto timeline = editor->GetTimeEvaluator( sceneName );
    auto root = scene->GetRootNode();

    // add default light
    editor->AddLight( scene, LightType::LT_DIRECTIONAL, timeline );

    auto meshLoader = nodelogic::MeshLoader::Create( root, timeline, meshPath );
    meshLoader->EnableTextures( true );
    meshLoader->EnableMaterials( true );

    meshLoader->Load( scene, editor );
    
    auto bb = root->GetBoundingBoxRecursive();
    auto & cameraLogic = scene->GetCamerasLogic();
    auto & camera = cameraLogic.GetCurrentCamera();

    auto meshPos = GetMeshTranslationToFitCamera( camera, bb );

    SetParameterTranslation( root->GetPlugin( "transform" )->GetParamTransform(), 0.f, meshPos );

    RequestThumbnail( scene, destPath, ThumbnailType::MeshAsset );
}

// ***********************
//
glm::vec3   SceneEventsHandlers::GetMeshTranslationToFitCamera            ( model::CameraModelPtr camera, const mathematics::Box & boundingBox )
{
    //FIXME: probably it would be better to move camera than mesh but there is no appropriate API
    auto bbCenter = boundingBox.Center();
    auto bbRadius = 0.f;
    for( auto vert : boundingBox.GetVerticies() )
    {
        auto dist = glm::distance( vert, bbCenter );
        bbRadius = (std::max)( dist, bbRadius );
    }

    auto camFOV = glm::radians( model::QueryTypedParam< model::ParamFloatPtr >( camera->GetParameter( model::CameraModel::PARAMETERS::FOV ) )->Evaluate() );
    auto camPos = model::QueryTypedParam< model::ParamVec3Ptr >( camera->GetParameter( model::CameraModel::PARAMETERS::POSITION ) )->Evaluate();
    auto camDir = model::QueryTypedParam< model::ParamVec3Ptr >( camera->GetParameter( model::CameraModel::PARAMETERS::DIRECTION ) )->Evaluate();

    auto height = ( float )ApplicationContext::Instance().GetHeight();
    auto width = ( float )ApplicationContext::Instance().GetWidth();

    auto ratio = ( Float32 )(std::max)( width, height ) / ( Float32 )(std::min)( width, height );

    auto camDist = ratio * bbRadius / ( 2.f * glm::tan( camFOV / 2.0f ) );

    SetParameter( camera->GetParameter( model::CameraModel::PARAMETERS::FAR_CLIPPING_PLANE ), 0.f, camDist + (std::max)( boundingBox.Width(), boundingBox.Height() ) / 2.f );

    return camPos + camDir * camDist - bbCenter;
}

} //bv
