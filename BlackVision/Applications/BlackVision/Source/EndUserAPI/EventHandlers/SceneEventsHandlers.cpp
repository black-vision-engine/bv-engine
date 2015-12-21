#include "SceneEventsHandlers.h"

#include "Engine/Models/BVProjectEditor.h"
#include "../../BVAppLogic.h"
#include "../../UseLoggerBVAppModule.h"

#include "ProjectManager.h"
#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Widgets/Crawler/CrawlerEvents.h"
#include "Widgets/Counter/Counter.h"
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
		editor->SetNodeVisible( sceneName, nodePath, true );
	}
    else if( command == NodeStructureEvent::Command::SetNodeInvisible )
	{
		editor->SetNodeVisible( sceneName, nodePath, false );
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
		
		editor->MoveNode( destSceneName, destNodePath, destIdx, srcSceneName, srcNodePath );
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
	std::string & request		= structureEvent->Request;
    unsigned int attachIndex	= structureEvent->AttachIndex;
    auto command				= structureEvent->PluginCommand;

	auto editor = m_appLogic->GetBVProject()->GetProjectEditor();

    if( command == PluginStructureEvent::Command::AddPlugin )
    {
		editor->AddPlugin( sceneName, nodePath, pluginUID, pluginName, timelinePath, attachIndex );
    }
    else if( command == PluginStructureEvent::Command::RemovePlugin )
	{
		editor->DeletePlugin( sceneName, nodePath, pluginName );
	}
    else if( command == PluginStructureEvent::Command::AttachPlugin )
	{
		editor->AttachPlugin( sceneName, nodePath, attachIndex );
	}
    else if( command == PluginStructureEvent::Command::DetachPlugin )
	{
		editor->DetachPlugin( sceneName, nodePath, pluginName );
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

		editor->AddPluginCopy( destSceneName, destNodePath, destIdx, srcSceneName, srcNodePath, srcPluginName );
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

		editor->MovePlugin( destSceneName, destNodePath, destIdx, srcSceneName, srcNodePath, srcPluginName );
	}
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

        SendOnSceneStructureResponse( senderID, "NewProject", "status", "OK" );
    }
    else if( command == ProjectEvent::Command::SetCurrentProject )
    {
        auto projName = GetRequestParamValue( request )[ "projectName" ].asString();

        pm->SetCurrentProject( projName );

        SendOnSceneStructureResponse( senderID, "SetCurrentProject", "status", "OK" );
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
        {
            SendOnSceneStructureResponse( senderID, "LoadProject", "status", "OK" );
        }
        else
        {
            SendOnSceneStructureResponse( senderID, "LoadProject", "status", "ERROR" );
        }
    }
    else if( command == ProjectEvent::Command::CopyAsset )
    {
        auto categoryName = GetRequestParamValue( request )[ "category" ].asString();

        auto srcAssetName = GetRequestParamValue( request )[ "srcAssetName" ].asString();
        auto dstAssetName = GetRequestParamValue( request )[ "dstAssetName" ].asString();

        pm->CopyAsset( "", categoryName, srcAssetName, "", dstAssetName );

        SendOnSceneStructureResponse( senderID, "CopyAsset", "status", "OK" );
    }
    else if( command == ProjectEvent::Command::MoveAsset )
    {
        auto categoryName = GetRequestParamValue( request )[ "category" ].asString();

        auto srcAssetName = GetRequestParamValue( request )[ "srcAssetName" ].asString();
        auto dstAssetName = GetRequestParamValue( request )[ "dstAssetName" ].asString();

        pm->MoveAsset( "", categoryName, srcAssetName, "", dstAssetName );

        SendOnSceneStructureResponse( senderID, "MoveAsset", "status", "OK" );
    }
    else if( command == ProjectEvent::Command::RemoveAsset )
    {
        auto categoryName = GetRequestParamValue( request )[ "category" ].asString();

        auto assetName = GetRequestParamValue( request )[ "assetName" ].asString();

        pm->RemoveAsset( "", categoryName, assetName );

        SendOnSceneStructureResponse( senderID, "RemoveAsset", "status", "OK" );
    }
    else if( command == ProjectEvent::Command::ImportAsset )
    {
        auto categoryName = GetRequestParamValue( request )[ "category" ].asString();

        auto assetFilePath = GetRequestParamValue( request )[ "assetFilePath" ].asString();

        auto dstAssetPath = GetRequestParamValue( request )[ "dstAssetPath" ].asString();

        pm->ImportAssetFromFile( "", categoryName, dstAssetPath, assetFilePath );

        SendOnSceneStructureResponse( senderID, "ImportAsset", "status", "OK" );
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

                SendOnSceneStructureResponse( senderID, "SaveScene", "status", "OK" );
            }
            else
            {
                SendOnSceneStructureResponse( senderID, "SaveScene", "status", "ERROR" );
                assert( false );
                // TODO: Implement
            }
        }
        else
        {
            SendOnSceneStructureResponse( senderID, "SaveScene", "status", "ERROR" );
        }
    }
    else if( command == ProjectEvent::Command::LoadScene )
    {
        auto sceneName = GetRequestParamValue( request )[ "sceneName" ].asString();

        auto scene = pm->LoadScene( "", sceneName );

        if( scene )
        {
            m_appLogic->GetBVProject()->GetProjectEditor()->AddScene( scene );
            SendOnSceneStructureResponse( senderID, "LoadScene", "status", "OK" );
        }
        else
        {
            SendOnSceneStructureResponse( senderID, "LoadScene", "status", "ERROR" );
        }
    }
    else if( command == ProjectEvent::Command::RemoveScene )
    {
        auto sceneName = GetRequestParamValue( request )[ "sceneName" ].asString();

        pm->RemoveScene( "", sceneName );

        SendOnSceneStructureResponse( senderID, "CopyScene", "status", "OK" );
    }
    else if( command == ProjectEvent::Command::CopyScene )
    {
        auto sceneName = GetRequestParamValue( request )[ "sceneName" ].asString();
        auto destSceneName = GetRequestParamValue( request )[ "destToSceneName" ].asString();

        pm->CopyScene( "", sceneName, "", destSceneName );

        SendOnSceneStructureResponse( senderID, "CopyScene", "status", "OK" );
    }
    else if( command == ProjectEvent::Command::MoveScene )
    {
        auto sceneName = GetRequestParamValue( request )[ "sceneName" ].asString();
        auto destSceneName = GetRequestParamValue( request )[ "destSceneName" ].asString();

        pm->MoveScene( "", sceneName, "", destSceneName );

        SendOnSceneStructureResponse( senderID, "MoveScene", "status", "OK" );
    }

}

// ***********************
//
void SceneEventsHandlers::TimelineHandler     ( bv::IEventPtr evt )
{
    if( evt->GetEventType() == bv::TimeLineEvent::Type() )
    {
		auto editor = m_appLogic->GetBVProject()->GetProjectEditor();

        bv::TimeLineEventPtr timelineEvent = std::static_pointer_cast<bv::TimeLineEvent>( evt );

        std::string& sceneName = timelineEvent->SceneName;
        std::string& timeLineName = timelineEvent->TimelineName; //path?
        std::string& timelineNewName = timelineEvent->NewTimelineName;

        float time = timelineEvent->Time;
        auto duration = timelineEvent->Duration;
        auto wrapMethod = timelineEvent->WrapMethod;

        TimeLineEvent::Command command = timelineEvent->TimelineCommand;

        if( command == TimeLineEvent::Command::AddTimeline )
		{
			editor->AddTimeline( sceneName, timelineNewName, duration, wrapMethod, wrapMethod );
		}
		else if( command == TimeLineEvent::Command::DeleteTimeline )
        {
			auto success = editor->DeleteTimeline( timeLineName );
			int senderID = timelineEvent->SocketID;
			if( success )
			{
				SendOnSceneStructureResponse( senderID, "DeleteTimeline", "status", "OK" );
			}
			else
			{
				SendOnSceneStructureResponse( senderID, "DeleteTimeline", "status", "ERROR" );
			}
        }
		else if( command == TimeLineEvent::Command::ForceDeleteTimeline )
        {
			//timelineNewName is more path of new timeline here..
			editor->ForceDeleteTimeline( timeLineName, timelineNewName );
        }
		else if( command == TimeLineEvent::Command::RenameTimeline )
        {
			editor->RenameTimeline( timeLineName, timelineNewName );
        }
		else if( command == TimeLineEvent::Command::SetDuration )
        {
			editor->SetTimelineDuration( timeLineName, duration );
        }
		else if( command == TimeLineEvent::Command::SetWrapPreBehavior )
        {
			editor->SetTimelineWrapPreBehavior( timeLineName, wrapMethod );
        }
		else if( command == TimeLineEvent::Command::SetWrapPostBehavior )
        {
			editor->SetTimelineWrapPostBehavior( timeLineName, wrapMethod );
        }
        else if( command == TimeLineEvent::Command::Play )
        {
			auto timeline = TimelineManager::GetInstance()->GetTimeline( timeLineName );
			if( timeline == nullptr )
			{
				LOG_MESSAGE( SeverityLevel::error ) << "Timeline ["+ timeLineName + "] does not exist.";
				return;
			}

            timeline->SetPlayDirection( bv::TimelinePlayDirection::TPD_FORWAD );
            timeline->Play();
        }
        else if( command == TimeLineEvent::Command::Stop )
        {
			auto timeline = TimelineManager::GetInstance()->GetTimeline( timeLineName );
			if( timeline == nullptr )
			{
				LOG_MESSAGE( SeverityLevel::error ) << "Timeline ["+ timeLineName + "] does not exist.";
				return;
			}

            timeline->Stop();
        }
        else if( command == TimeLineEvent::Command::PlayReverse )
        {
			auto timeline = TimelineManager::GetInstance()->GetTimeline( timeLineName );
			if( timeline == nullptr )
			{
				LOG_MESSAGE( SeverityLevel::error ) << "Timeline ["+ timeLineName + "] does not exist.";
				return;
			}

            timeline->SetPlayDirection( bv::TimelinePlayDirection::TPD_BACKWARD );
            timeline->Play();
        }
        else if( command == TimeLineEvent::Command::Goto )
        {
			auto timeline = TimelineManager::GetInstance()->GetTimeline( timeLineName );
			if( timeline == nullptr )
			{
				LOG_MESSAGE( SeverityLevel::error ) << "Timeline ["+ timeLineName + "] does not exist.";
				return;
			}

            timeline->SetPlayDirection( bv::TimelinePlayDirection::TPD_FORWAD );
            timeline->SetTimeAndStop( (bv::TimeType)time );
        }
        else if( command == TimeLineEvent::Command::GotoAndPlay )
        {
			auto timeline = TimelineManager::GetInstance()->GetTimeline( timeLineName );
			if( timeline == nullptr )
			{
				LOG_MESSAGE( SeverityLevel::error ) << "Timeline ["+ timeLineName + "] does not exist.";
				return;
			}

            timeline->SetPlayDirection( bv::TimelinePlayDirection::TPD_FORWAD );
            timeline->SetTimeAndPlay( (bv::TimeType)time );
        }
    }
}

// ***********************
//
void SceneEventsHandlers::WidgetHandler       ( bv::IEventPtr evt )
{
	if( evt->GetEventType() == bv::WidgetEvent::Type() )
        return;

	bv::WidgetEventPtr widgetEvent = std::static_pointer_cast<bv::WidgetEvent>( evt );        
    auto root = m_appLogic->GetBVProject()->GetModelSceneRoot();
        
    std::string nodeName = widgetEvent->NodeName;
    std::string action = widgetEvent->Action;
    WidgetEvent::Command command = widgetEvent->WidgetCommand;
    float time = widgetEvent->Time;


    BasicNodePtr node = std::static_pointer_cast< bv::model::BasicNode >( root->GetNode( nodeName ) );
    if( node == nullptr && root->GetName() == nodeName )
    {
        LOG_MESSAGE( SeverityLevel::info ) << "root node is node you're looking for [" + nodeName + "] Applying jedi fix now.";
        node = root;
    }
    if( node == nullptr )
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Error OnSetParam() node [" + nodeName + "] not found";
        return;
    }
		
    INodeLogic* logic = node->GetLogic().get();
	if( logic == nullptr )
	{
        LOG_MESSAGE( SeverityLevel::error ) << "Error OnWidgetCmd () node [" + nodeName + "] , logic [] not found";
        return;
	}
		

    if( command == WidgetEvent::Command::Crawl )
    {
		bv::widgets::Crawler* crawler =  (bv::widgets::Crawler*)logic;
        Json::Value parseAction( action );

        std::string crawlAction = parseAction.get( "Action", "" ).asString();
        std::string param = parseAction.get( "Param", "" ).asString();

		if( crawlAction == "stop" )
		{
			//Log::A("OK","crawl stop...");
			crawler->Stop();
		}
		else if( crawlAction == "start" )
		{
			//Log::A("OK","crawl start...");
			crawler->Start();
		}
        else if( crawlAction == "add_text" )
		{
			//Log::A(L"OK",L"crawl add text..."+ widgetEvent->Param);
			crawler->AddMessage( toWString( param ) );
		}
        else if( crawlAction == "reset" )
		{
			//Log::A(L"OK",L"crawl reset...");
			crawler->Reset();
		}
		else if( crawlAction == "clear" )
		{
			//Log::A(L"OK",L"crawl clear...");
			crawler->Clear();
		}
        else if( crawlAction == "set_speed" )
		{
			//Log::A(L"OK",L"crawl set speed.."+ widgetEvent->Param);
			float speed = 0.5;
			speed = (float)atof( param.c_str() );
			crawler->SetSpeed( speed );
		}
	}
    else if( command == WidgetEvent::Command::Counter )
    {
		bv::widgets::WidgetCounter* counter = (bv::widgets::WidgetCounter*)logic;
        
        Json::Value parseAction( action );
        std::string param = parseAction.get( "Param", "" ).asString();
        float value = parseAction.get( "Value", 1.0f ).asFloat();


		auto paramPtr = counter->GetValueParam();
		if( paramPtr == nullptr )
		{
			LOG_MESSAGE( SeverityLevel::error ) << "Error OnSetParam() plugin [counter] param [" + param + "] not found";
            return;
		}

        SetParameter( paramPtr, (bv::TimeType)time, value );
	}
}

// ***********************
//
void SceneEventsHandlers::OnNodeAppearing     ( IEventPtr evt )
{

}

// ***********************
//
void SceneEventsHandlers::OnNodeLeaving       ( IEventPtr evt )
{

}


// *********************************
//
namespace 
{
    const std::wstring* examples2 = new std::wstring[20];
   

    int examplesIndex=0;


	const static std::wstring examples[] = 
	{
		L"Jasiu kup kie³basê !!",
		L"wielojêzyczny projekt internetortej treœci. Funkcjonuje wykorzystuj¹c",
		L"Wikipedia powsta³a 15 stycznia ertów i nieistniej¹cej ju¿ Nupedii. ",
		L"iostrzane. Wikipedia jest jedn¹], a wiele stron uruchomi³o jej mirrory lub forki.",
		L"Wspó³za³o¿yciel Wikipedii Jimmyia wielojêzycznej",
		L"wolnej encyklopedii o najwy¿szyw³asnym jêzyku”[8].",
		L"Kontrowersje budzi wiarygodnoœæeœci artyku³ów ",
		L"i brak weryfikacji kompetencji .",
		L"Z drugiej",
		L"strony mo¿liwoœæ swobodnej dyst Ÿród³em informacji",
		L"Jasiu kup kie³basê !!",
	};

   

   

	auto exampleSize = sizeof( examples ) / sizeof( std::wstring );
}


// ***********************
//
void SceneEventsHandlers::OnNoMoreNodes       ( IEventPtr evt )
{
	auto typedEvent = std::static_pointer_cast< widgets::NoMoreNodesCrawlerEvent >( evt );
	// Remove code below. Only for testing.
	auto n = typedEvent->GetCrawler()->GetNonActiveNode();
	if( n )
	{

		auto textNode = n->GetChild( "Text" );
		if( textNode )
		{
			auto pl = textNode->GetPlugin( "text" );

			if( pl )
			{
				//model::DefaultTextPlugin::SetText( pl, examples[ i ] );

                SetParameter( pl->GetParameter( "text" ), 0.0, L"nowa wiadomoœæ "+to_wstring(examplesIndex) );
                examplesIndex=(examplesIndex+1)%20;

				typedEvent->GetCrawler()->EnqueueNode( n );
			}
		}
	}
}

} //bv
