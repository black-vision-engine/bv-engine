#include "SceneEventsHandlers.h"

#include "Engine/Models/BVProjectEditor.h"
#include "BVAppLogic.h"
#include "UseLoggerBVAppModule.h"

#include "ProjectManager.h"
#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Engine/Models/Plugins/Simple/DefaultTextPlugin.h"

#include "EventHandlerHelpers.h"

#include "System/Path.h"
#include "IO/FileIO.h"

#include <limits>
#undef max

namespace bv
{


// ***********************
//
SceneEventsHandlers::SceneEventsHandlers( BVAppLogic* logic )
    : m_appLogic( logic )
{
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &SceneEventsHandlers::ThumbnailRendered ), ScreenShotRenderedEvent::Type() );
}

SceneEventsHandlers::~SceneEventsHandlers()
{}

// ***********************
//
void SceneEventsHandlers::SceneStructure    ( bv::IEventPtr evt )
{
    if( evt->GetEventType() != bv::SceneEvent::Type() )
        return;
    bv::SceneEventPtr sceneEvent = std::static_pointer_cast< bv::SceneEvent >( evt );

	std::string & sceneName		= sceneEvent->SceneName;
    std::string & newSceneName	= sceneEvent->NewSceneName;
    auto attachIndex			= sceneEvent->AttachIndex;
    auto command                = sceneEvent->SceneCommand;
    auto eventID                = sceneEvent->EventID;

    bool result = true;
	auto editor = m_appLogic->GetBVProject()->GetProjectEditor();

    if( command == SceneEvent::Command::AddScene )
	{
		editor->AddScene( newSceneName );
	}
    else if( command == SceneEvent::Command::RemoveScene )
	{
		result = editor->RemoveScene( sceneName );
	}
    else if( command == SceneEvent::Command::RemoveAllScenes )
    {
        editor->RemoveAllScenes();
    }
    else if( command == SceneEvent::Command::SetSceneVisible )
	{
		result = editor->SetSceneVisible( sceneName, true );
	}
    else if( command == SceneEvent::Command::SetSceneInvisible )
	{
		result = editor->SetSceneVisible( sceneName, false );
	}
    else if( command == SceneEvent::Command::RenameScene )
	{
		result = editor->RenameScene( sceneName, newSceneName );
	}
    else if( command == SceneEvent::Command::AttachScene )
	{
		result = editor->AttachScene( sceneName, attachIndex );
	}
    else if( command == SceneEvent::Command::DetachScene )
	{
		result = editor->DetachScene( sceneName );
	}
    else if( command == SceneEvent::Command::MoveScene )
	{
		result = editor->MoveScene( sceneName, attachIndex );
	}
    else if( command == SceneEvent::Command::CopyScene )
	{
		auto sceneCopy = editor->AddSceneCopy( sceneName );
        if( sceneCopy == nullptr )
            result = false;
	}
    else
        result = false;

    SendSimpleResponse( command, eventID, sceneEvent->SocketID, result );
}

// ***********************
//
void SceneEventsHandlers::NodeStructure      ( bv::IEventPtr evt )
{
    if( evt->GetEventType() != bv::NodeStructureEvent::Type() )
        return;
    bv::NodeStructureEventPtr structureEvent = std::static_pointer_cast<bv::NodeStructureEvent>( evt );

	std::string& sceneName		= structureEvent->SceneName;
    std::string& nodePath		= structureEvent->NodePath;
    std::string& newNodeName	= structureEvent->NewNodeName;	
    auto attachIndex			= structureEvent->AttachIndex;
    auto eventID                = structureEvent->EventID;
    auto command                = structureEvent->SceneCommand;

    IDeserializer * request		= structureEvent->Request;

    bool result = true;
	auto editor = m_appLogic->GetBVProject()->GetProjectEditor();

    if( command == NodeStructureEvent::Command::AddNode )
    {
		editor->AddChildNode( sceneName, nodePath, newNodeName );
    }
    else if( command == NodeStructureEvent::Command::RemoveNode )
    {
		result = editor->DeleteChildNode( sceneName, nodePath );
    }
    else if( command == NodeStructureEvent::Command::SetNodeVisible )
	{
		result = editor->SetNodeVisible( sceneName, nodePath, true );
	}
    else if( command == NodeStructureEvent::Command::SetNodeInvisible )
	{
		result = editor->SetNodeVisible( sceneName, nodePath, false );
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
        auto destIdx = SerializationHelper::String2T< UInt32 >( request->GetAttribute( "DestIndex" ), 0 );
		auto srcSceneName = request->GetAttribute( "SrcSceneName" );
		auto srcNodePath = request->GetAttribute( "SrcPath" );
		
		result = editor->MoveNode( destSceneName, destNodePath, destIdx, srcSceneName, srcNodePath );
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

		auto copyPtr = editor->AddNodeCopy( destSceneName, destNodePath, srcSceneName, srcNodePath );
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
    if( evt->GetEventType() != bv::PluginStructureEvent::Type() )
        return;
    bv::PluginStructureEventPtr structureEvent = std::static_pointer_cast<bv::PluginStructureEvent>( evt );

    std::string& nodePath		= structureEvent->NodePath;
    std::string& sceneName		= structureEvent->SceneName;
    std::string& pluginName		= structureEvent->PluginName;
    std::string& pluginUID		= structureEvent->PluginUID;
	std::string& timelinePath	= structureEvent->TimelinePath;
    unsigned int attachIndex	= structureEvent->AttachIndex;
    auto command				= structureEvent->PluginCommand;
    auto eventID                = structureEvent->EventID;
    IDeserializer * request		= structureEvent->Request;


    bool result = true;
	auto editor = m_appLogic->GetBVProject()->GetProjectEditor();

    if( command == PluginStructureEvent::Command::AddPlugin )
    {
		result = editor->AddPlugin( sceneName, nodePath, pluginUID, pluginName, timelinePath, attachIndex );
    }
    else if( command == PluginStructureEvent::Command::RemovePlugin )
	{
		result = editor->DeletePlugin( sceneName, nodePath, pluginName );
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
        auto destIdx = SerializationHelper::String2T< UInt32 >( request->GetAttribute( "DestIndex" ), 0 );
		auto srcSceneName = request->GetAttribute( "SrcSceneName" );
		auto srcNodePath = request->GetAttribute( "SrcPath" );
		auto srcPluginName = request->GetAttribute( "SrcName" );

		auto pluginPtr = editor->AddPluginCopy( destSceneName, destNodePath, destIdx, srcSceneName, srcNodePath, srcPluginName );
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
        auto destIdx = SerializationHelper::String2T< UInt32 >( request->GetAttribute( "DestIndex" ), 0 );
		auto srcSceneName = request->GetAttribute( "SrcSceneName" );
		auto srcNodePath = request->GetAttribute( "SrcPath" );
		auto srcPluginName = request->GetAttribute( "SrcName" );

		result = editor->MovePlugin( destSceneName, destNodePath, destIdx, srcSceneName, srcNodePath, srcPluginName );
	}
    else result = false;

    SendSimpleResponse( command, eventID, structureEvent->SocketID, result );
}

// ***********************
//
void SceneEventsHandlers::ProjectStructure    ( bv::IEventPtr evt )
{
    if( evt->GetEventType() != bv::ProjectEvent::Type() )
        return;
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

        auto fileName = File::GetFileName( saveTo );

        auto ext = File::GetExtension( fileName );

        if( ext.empty() || ext != "scn" )
        {
            saveTo += ".scn";
        }

        if( saveTo.empty() || Path::IsValisPathName( saveTo ) )
        {
            auto forceSaveStr = request.GetAttribute( "forceSave" );

            bool forceSave = false;

            if( forceSaveStr == "true" )
            {
                forceSave = true;
            }

            auto scene = m_appLogic->GetBVProject()->GetScene( sceneName );

            if( scene )
            {
                if( forceSave )
                {
                    if( saveTo.empty() )
                    {
                        pm->AddScene( scene, "", scene->GetName() );
                    }
                    else
                    {
                        pm->AddScene( scene, "", saveTo );
                    }

                    SendSimpleResponse( command, projectEvent->EventID, senderID, true );
                
                    Path sceneScreenShot( saveTo );
                    sceneScreenShot = sceneScreenShot / scene->GetName();
                    sceneScreenShot = ProjectManager::GetInstance()->ToAbsPath( sceneScreenShot );
                    m_appLogic->GetRenderMode().MakeScreenShot( sceneScreenShot.Str(), true ); 
                }
                else
                {
                    SendSimpleResponse( command, projectEvent->EventID, senderID, false );
                    assert( false );
                    // TODO: Implement
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
        auto sceneName = request.GetAttribute( "sceneName" );

        auto scene = pm->LoadScene( "", sceneName );

        if( scene )
        {
            m_appLogic->GetBVProject()->GetProjectEditor()->AddScene( scene );
            SendSimpleResponse( command, projectEvent->EventID, senderID, true );
        }
        else
        {
            SendSimpleErrorResponse( command, projectEvent->EventID, senderID, "Scene not found." );
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

        auto scene = SceneModel::Create( ( Path( projectName ) / Path( inSceneName ) ).Str(), nullptr );

        pm->AddScene( scene, projectName, inSceneName );

        SendSimpleResponse( command, projectEvent->EventID, senderID, true );
    }
    else if( command == ProjectEvent::Command::SavePreset )
    {
        auto destProjectName = request.GetAttribute( "DestProjectName" );
        auto destPath = request.GetAttribute( "DestPath" );
        auto sceneName = request.GetAttribute( "SceneName" );
        auto nodePath = request.GetAttribute( "NodePath" );

        auto editor = m_appLogic->GetBVProject()->GetProjectEditor();

        auto n = editor->GetNode( sceneName, nodePath );
        assert( std::dynamic_pointer_cast< model::BasicNode >( n ) );
        auto bn = std::static_pointer_cast< model::BasicNode >( n );

        if( bn != nullptr )
        {
            pm->SavePreset( bn, destProjectName, destPath );
            SendSimpleResponse( command, projectEvent->EventID, senderID, true );
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
        auto scene = editor->GetScene( sceneName );

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
        auto scene = editor->GetScene( sceneName );

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

        auto success = pm->CreateAssetDir( categoryName, path, recursive );

        SendSimpleResponse( command, projectEvent->EventID, senderID, success );
    }
    else if( command == ProjectEvent::Command::DeleteFolder )
    {
        auto categoryName = request.GetAttribute( "categoryName" );
        auto path = request.GetAttribute( "path" );

        auto success = pm->RemoveAssetDir( categoryName, path );

        SendSimpleResponse( command, projectEvent->EventID, senderID, success );
    }
	 else if( command == ProjectEvent::Command::RenameFolder )
    {
        auto categoryName = request.GetAttribute( "categoryName" );
        auto path = request.GetAttribute( "path" );
		auto path2 = request.GetAttribute( "newName" );

        auto success = pm->RenameAssetDir( categoryName, path,path2 );

        SendSimpleResponse( command, projectEvent->EventID, senderID, success );
    }
    else
    {
        SendSimpleErrorResponse( command, projectEvent->EventID, senderID, "Unknown command" );
    }

}

// ***********************
//
void        SceneEventsHandlers::SceneVariable       ( bv::IEventPtr evt )
{
    if( evt->GetEventType() != bv::SceneVariableEvent::Type() )
        return;

    SceneVariableEventPtr sceneVarEvent = std::static_pointer_cast< bv::SceneVariableEvent >( evt );
    std::string & sceneName         = sceneVarEvent->SceneName;
    std::string & variableName      = sceneVarEvent->VariableName;
    std::string & variableContent   = sceneVarEvent->VariableContent;
    auto command                    = sceneVarEvent->VariableCommand;

    auto sceneModel = m_appLogic->GetBVProject()->GetProjectEditor()->GetScene( sceneName );
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
        JsonSerializeObject ser;
        Expected< std::string > varContent = variablesCollection.GetVariable( variableName );

        if( !varContent.isValid )
        {
            SendSimpleErrorResponse( command, sceneVarEvent->EventID, sceneVarEvent->SocketID, "Variable not found" );
            return;
        }

        ser.SetAttribute( "VariableContent", varContent.ham );
        
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
void        SceneEventsHandlers::ThumbnailRendered   ( bv::IEventPtr evt )
{
    if( evt->GetEventType() != bv::ScreenShotRenderedEvent::Type() )
        return;
    
    ScreenShotRenderedEventPtr screenShotEvent = std::static_pointer_cast< ScreenShotRenderedEvent >( evt );

    if( screenShotEvent->Result )
    {
        // FIXME: Load image, resize to 128,128 and create thumbnail
    }
}

} //bv
