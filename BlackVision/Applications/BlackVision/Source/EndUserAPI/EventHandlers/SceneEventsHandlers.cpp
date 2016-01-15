#include "SceneEventsHandlers.h"

#include "Engine/Models/BVProjectEditor.h"
#include "../../BVAppLogic.h"
#include "../../UseLoggerBVAppModule.h"

#include "ProjectManager.h"
#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Engine/Events/EventHelpers.h"
#include "Engine/Models/Plugins/Simple/DefaultTextPlugin.h"

#include "EventHandlerHelpers.h"

#include <limits>
#undef max

namespace bv
{


// ***********************
//
SceneEventsHandlers::SceneEventsHandlers( BVAppLogic* logic )
    : m_appLogic( logic )
{}

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
	std::string& request		= structureEvent->Request;
    auto attachIndex			= structureEvent->AttachIndex;
    auto eventID                = structureEvent->EventID;

    auto command = structureEvent->SceneCommand;

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
		//FIXME: replace with sth more generic
		auto destSceneName = GetRequestParamValue( request )[ "SrcSceneName" ].asString();
		auto destNodePath = GetRequestParamValue( request )[ "DestPath" ].asString();
		auto destIdx = GetRequestParamValue( request )[ "DestIndex" ].asUInt();
		auto srcSceneName = GetRequestParamValue( request )[ "SrcSceneName" ].asString();
		auto srcNodePath = GetRequestParamValue( request )[ "SrcPath" ].asString();
		
		result = editor->MoveNode( destSceneName, destNodePath, destIdx, srcSceneName, srcNodePath );
	}
	else if( command == NodeStructureEvent::Command::CopyNode )
	{
		//FIXME: replace with sth more generic
		auto destSceneName = GetRequestParamValue( request )[ "SrcSceneName" ].asString();
		auto destNodePath = GetRequestParamValue( request )[ "DestPath" ].asString();
		auto srcSceneName = GetRequestParamValue( request )[ "SrcSceneName" ].asString();
		auto srcNodePath = GetRequestParamValue( request )[ "SrcPath" ].asString();

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
	std::string& request		= structureEvent->Request;
    unsigned int attachIndex	= structureEvent->AttachIndex;
    auto command				= structureEvent->PluginCommand;
    auto eventID                = structureEvent->EventID;

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
		//FIXME: replace with sth more generic
		auto destSceneName = GetRequestParamValue( request )[ "SrcSceneName" ].asString();
		auto destNodePath = GetRequestParamValue( request )[ "DestPath" ].asString();
		auto destIdx = GetRequestParamValue( request )[ "DestIndex" ].asUInt();
		auto srcSceneName = GetRequestParamValue( request )[ "SrcSceneName" ].asString();
		auto srcNodePath = GetRequestParamValue( request )[ "SrcPath" ].asString();
		auto srcPluginName = GetRequestParamValue( request )[ "SrcName" ].asString();

		auto pluginPtr = editor->AddPluginCopy( destSceneName, destNodePath, destIdx, srcSceneName, srcNodePath, srcPluginName );
        if( pluginPtr == nullptr )
            result = false;
	}
	else if( command == PluginStructureEvent::Command::MovePlugin )
	{
		//FIXME: replace with sth more generic
		auto destSceneName = GetRequestParamValue( request )[ "SrcSceneName" ].asString();
		auto destNodePath = GetRequestParamValue( request )[ "DestPath" ].asString();
		auto destIdx = GetRequestParamValue( request )[ "DestIndex" ].asUInt();
		auto srcSceneName = GetRequestParamValue( request )[ "SrcSceneName" ].asString();
		auto srcNodePath = GetRequestParamValue( request )[ "SrcPath" ].asString();
		auto srcPluginName = GetRequestParamValue( request )[ "SrcName" ].asString();

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

    std::string& request = projectEvent->Request;
    auto command = projectEvent->ProjectCommand;
    int senderID = projectEvent->SocketID;

    if( command == ProjectEvent::Command::NewProject )
    {
        auto name = GetRequestParamValue( request )[ "projectName" ].asString();

        pm->AddNewProject( name );

        SendSimpleResponse( command, projectEvent->EventID, senderID, true );
    }
    else if( command == ProjectEvent::Command::SetCurrentProject )
    {
        auto projName = GetRequestParamValue( request )[ "projectName" ].asString();

        pm->SetCurrentProject( projName );

        SendSimpleResponse( command, projectEvent->EventID, senderID, true );
    }
    else if( command == ProjectEvent::Command::LoadProject )
    {
        auto projName = std::string( request.begin(), request.end() );

        auto projectScenesNames = pm->ListScenesNames( projName );

        bool status = false;

        if( !projectScenesNames.empty() )
        {
            UpdatersManager::Get().RemoveAllUpdaters();
                
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
        auto categoryName = GetRequestParamValue( request )[ "category" ].asString();

        auto srcAssetName = GetRequestParamValue( request )[ "srcAssetName" ].asString();
        auto dstAssetName = GetRequestParamValue( request )[ "dstAssetName" ].asString();

        pm->CopyAsset( "", categoryName, srcAssetName, "", dstAssetName );

        SendSimpleResponse( command, projectEvent->EventID, senderID, true );
    }
    else if( command == ProjectEvent::Command::MoveAsset )
    {
        auto categoryName = GetRequestParamValue( request )[ "category" ].asString();

        auto srcAssetName = GetRequestParamValue( request )[ "srcAssetName" ].asString();
        auto dstAssetName = GetRequestParamValue( request )[ "dstAssetName" ].asString();

        pm->MoveAsset( "", categoryName, srcAssetName, "", dstAssetName );

        SendSimpleResponse( command, projectEvent->EventID, senderID, true );
    }
    else if( command == ProjectEvent::Command::RemoveAsset )
    {
        auto categoryName = GetRequestParamValue( request )[ "category" ].asString();

        auto assetName = GetRequestParamValue( request )[ "assetName" ].asString();

        pm->RemoveAsset( "", categoryName, assetName );

        SendSimpleResponse( command, projectEvent->EventID, senderID, true );
    }
    else if( command == ProjectEvent::Command::ImportAsset )
    {
        auto categoryName = GetRequestParamValue( request )[ "category" ].asString();

        auto assetFilePath = GetRequestParamValue( request )[ "assetFilePath" ].asString();

        auto dstAssetPath = GetRequestParamValue( request )[ "dstAssetPath" ].asString();

        pm->ImportAssetFromFile( "", categoryName, dstAssetPath, assetFilePath );

        SendSimpleResponse( command, projectEvent->EventID, senderID, true );
    }
    else if( command == ProjectEvent::Command::SaveScene )
    {
        auto sceneName = GetRequestParamValue( request )[ "sceneName" ].asString();

        auto saveTo = GetRequestParamValue( request )[ "saveTo" ].asString();

        auto forceSaveStr = GetRequestParamValue( request )[ "forceSave" ].asString();

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
    else if( command == ProjectEvent::Command::LoadScene )
    {
        auto sceneName = GetRequestParamValue( request )[ "sceneName" ].asString();

        auto scene = pm->LoadScene( "", sceneName );

        if( scene )
        {
            m_appLogic->GetBVProject()->GetProjectEditor()->AddScene( scene );
            SendSimpleResponse( command, projectEvent->EventID, senderID, false );
        }
        else
        {
            SendSimpleErrorResponse( command, projectEvent->EventID, senderID, "Scene not found." );
        }
    }
    else if( command == ProjectEvent::Command::RemoveScene )
    {
        auto sceneName = GetRequestParamValue( request )[ "sceneName" ].asString();
        auto projectName = GetRequestParamValue( request )[ "projectName" ].asString();

        pm->RemoveScene( projectName, sceneName );

        SendSimpleResponse( command, projectEvent->EventID, senderID, true );
    }
    else if( command == ProjectEvent::Command::CopyScene )
    {
        auto sceneName = GetRequestParamValue( request )[ "sceneName" ].asString();
        auto destSceneName = GetRequestParamValue( request )[ "destToSceneName" ].asString();

        pm->CopyScene( "", sceneName, "", destSceneName );

        SendSimpleResponse( command, projectEvent->EventID, senderID, true );
    }
    else if( command == ProjectEvent::Command::MoveScene )
    {
        auto sceneName = GetRequestParamValue( request )[ "sceneName" ].asString();
        auto destSceneName = GetRequestParamValue( request )[ "destSceneName" ].asString();

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
        auto inSceneName = GetRequestParamValue( request )[ "inSceneName" ].asString();
        auto projectName = GetRequestParamValue( request )[ "projectName" ].asString();
        auto outSceneName = GetRequestParamValue( request )[ "outSceneName" ].asString();

        pm->CopyScene( "", inSceneName, projectName, outSceneName );

        SendSimpleResponse( command, projectEvent->EventID, senderID, true );
    }
    else if( command == ProjectEvent::Command::RemoveSceneFromProject )
    {
        // Already implemented by event RemoveScene. Use that event.
        assert( false );
    }
    else if( command == ProjectEvent::Command::CreateSceneInProject )
    {
        auto inSceneName = GetRequestParamValue( request )[ "inSceneName" ].asString();
        auto projectName = GetRequestParamValue( request )[ "projectName" ].asString();

        auto scene = SceneModel::Create( ( Path( projectName ) / Path( inSceneName ) ).Str(), nullptr );

        pm->AddScene( scene, projectName, inSceneName );

        SendSimpleResponse( command, projectEvent->EventID, senderID, true );
    }
    else if( command == ProjectEvent::Command::CreateFolder )
    {
        auto categoryName = GetRequestParamValue( request )[ "categoryName" ].asString();
        auto path = GetRequestParamValue( request )[ "path" ].asString();

        auto success = pm->CreateAssetDir( categoryName, path );

        SendSimpleResponse( command, projectEvent->EventID, senderID, success );
    }
    else if( command == ProjectEvent::Command::DeleteFolder )
    {
        auto categoryName = GetRequestParamValue( request )[ "categoryName" ].asString();
        auto path = GetRequestParamValue( request )[ "path" ].asString();

        auto success = pm->RemoveAssetDir( categoryName, path );

        SendSimpleResponse( command, projectEvent->EventID, senderID, success );
    }
    else
    {
        SendSimpleErrorResponse( command, projectEvent->EventID, senderID, "Unknown command" );
    }

}



} //bv
