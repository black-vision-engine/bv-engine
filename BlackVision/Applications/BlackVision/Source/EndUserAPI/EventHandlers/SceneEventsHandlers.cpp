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

        pm->RemoveScene( "", sceneName );

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

}

// ***********************
//
void SceneEventsHandlers::TimelineHandler     ( bv::IEventPtr evt )
{
    if( evt->GetEventType() == bv::TimeLineEvent::Type() )
    {
		auto editor = m_appLogic->GetBVProject()->GetProjectEditor();

        bv::TimeLineEventPtr timelineEvent = std::static_pointer_cast<bv::TimeLineEvent>( evt );

        std::string & timelinePath = timelineEvent->TimelineName; //path?
        std::string & timelineNewName = timelineEvent->NewTimelineName;

        float time = timelineEvent->Time;
        auto type = timelineEvent->TimelineType;
        auto duration = timelineEvent->Duration;
        auto wrapMethod = timelineEvent->WrapMethod;
        TimeLineEvent::Command command = timelineEvent->TimelineCommand;

        if( command == TimeLineEvent::Command::AddTimeline )
		{
			auto success = editor->AddTimeline( timelinePath, timelineNewName, type );
            SendSimpleResponse( command, timelineEvent->EventID, timelineEvent->SocketID, success );
		}
		else if( command == TimeLineEvent::Command::DeleteTimeline )
        {
			auto success = editor->DeleteTimeline( timelinePath );
			SendSimpleResponse( command, timelineEvent->EventID, timelineEvent->SocketID, success );
        }
		else if( command == TimeLineEvent::Command::ForceDeleteTimeline )
        {
			//timelineNewName is more path of new timeline here..
			auto success = editor->ForceDeleteTimeline( timelinePath, timelineNewName );
            SendSimpleResponse( command, timelineEvent->EventID, timelineEvent->SocketID, success );
        }
		else if( command == TimeLineEvent::Command::RenameTimeline )
        {
			auto success = editor->RenameTimeline( timelinePath, timelineNewName );
            SendSimpleResponse( command, timelineEvent->EventID, timelineEvent->SocketID, success );
        }
		else if( command == TimeLineEvent::Command::SetDuration )
        {
			auto success = editor->SetTimelineDuration( timelinePath, duration );
            SendSimpleResponse( command, timelineEvent->EventID, timelineEvent->SocketID, success );
        }
		else if( command == TimeLineEvent::Command::SetWrapPreBehavior )
        {
			auto success = editor->SetTimelineWrapPreBehavior( timelinePath, wrapMethod );
            SendSimpleResponse( command, timelineEvent->EventID, timelineEvent->SocketID, success );
        }
		else if( command == TimeLineEvent::Command::SetWrapPostBehavior )
        {
			auto success = editor->SetTimelineWrapPostBehavior( timelinePath, wrapMethod );
            SendSimpleResponse( command, timelineEvent->EventID, timelineEvent->SocketID, success );
        }
        else if( command == TimeLineEvent::Command::Play )
        {
            auto timeline = std::static_pointer_cast< model::ITimeline >( editor->GetTimeEvaluator( timelinePath ) );
			if( timeline == nullptr )
            {
                SendSimpleErrorResponse( command, timelineEvent->EventID, timelineEvent->SocketID, "Timeline not found" );
            }

            timeline->SetPlayDirection( bv::TimelinePlayDirection::TPD_FORWAD );
            timeline->Play();

            SendSimpleResponse( command, timelineEvent->EventID, timelineEvent->SocketID, true );
        }
        else if( command == TimeLineEvent::Command::Stop )
        {
            auto timeline = std::static_pointer_cast< model::ITimeline >( editor->GetTimeEvaluator( timelinePath ) );
			if( timeline == nullptr )
            {
                SendSimpleErrorResponse( command, timelineEvent->EventID, timelineEvent->SocketID, "Timeline not found" );
            }

            timeline->Stop();

            SendSimpleResponse( command, timelineEvent->EventID, timelineEvent->SocketID, true );
        }
        else if( command == TimeLineEvent::Command::PlayReverse )
        {
            auto timeline = std::static_pointer_cast< model::ITimeline >( editor->GetTimeEvaluator( timelinePath ) );
			if( timeline == nullptr )
            {
                SendSimpleErrorResponse( command, timelineEvent->EventID, timelineEvent->SocketID, "Timeline not found" );
            }

            timeline->SetPlayDirection( bv::TimelinePlayDirection::TPD_BACKWARD );
            timeline->Play();

            SendSimpleResponse( command, timelineEvent->EventID, timelineEvent->SocketID, true );
        }
        else if( command == TimeLineEvent::Command::Goto )
        {
            auto timeline = std::static_pointer_cast< model::ITimeline >( editor->GetTimeEvaluator( timelinePath ) );
			if( timeline == nullptr )
            {
                SendSimpleErrorResponse( command, timelineEvent->EventID, timelineEvent->SocketID, "Timeline not found" );
            }

            timeline->SetPlayDirection( bv::TimelinePlayDirection::TPD_FORWAD );
            timeline->SetTimeAndStop( ( bv::TimeType )time );

            SendSimpleResponse( command, timelineEvent->EventID, timelineEvent->SocketID, true );
        }
        else if( command == TimeLineEvent::Command::GotoAndPlay )
        {
            auto timeline = std::static_pointer_cast< model::ITimeline >( editor->GetTimeEvaluator( timelinePath ) );
			if( timeline == nullptr )
            {
                SendSimpleErrorResponse( command, timelineEvent->EventID, timelineEvent->SocketID, "Timeline not found" );
            }

            timeline->SetPlayDirection( bv::TimelinePlayDirection::TPD_FORWAD );
            timeline->SetTimeAndPlay( ( bv::TimeType )time );

            SendSimpleResponse( command, timelineEvent->EventID, timelineEvent->SocketID, true );
        }
        else
            SendSimpleErrorResponse( command, timelineEvent->EventID, timelineEvent->SocketID, "Unknown command" );
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
        
    std::string& nodeName = widgetEvent->NodeName;
    std::string& sceneName = widgetEvent->SceneName;
    std::string& action = widgetEvent->Action;
    WidgetEvent::Command command = widgetEvent->WidgetCommand;
    float time = widgetEvent->Time;

    auto node = GetNode( m_appLogic, sceneName, nodeName );
    if( node == nullptr )
        SendSimpleErrorResponse( command, widgetEvent->EventID, widgetEvent->SocketID, "Node not found" );


    BasicNodePtr basicNode = std::static_pointer_cast< bv::model::BasicNode >( node );
    INodeLogic* logic = basicNode->GetLogic().get();
	if( logic == nullptr )
	{
        LOG_MESSAGE( SeverityLevel::warning ) << "Error OnWidgetCmd () node [" + nodeName + "] , logic [] not found";
        SendSimpleErrorResponse( command, widgetEvent->EventID, widgetEvent->SocketID, "NodeLogic not found" );
	}
		

    if( command == WidgetEvent::Command::Crawl )
    {
		bv::widgets::Crawler* crawler =  (bv::widgets::Crawler*)logic;
        Json::Value parseAction( action );

        std::string crawlAction = parseAction.get( "Action", "" ).asString();
        std::string param = parseAction.get( "Param", "" ).asString();

		if( crawlAction == "stop" )
		{
			crawler->Stop();
            SendSimpleResponse( command, widgetEvent->EventID, widgetEvent->SocketID, true );
		}
		else if( crawlAction == "start" )
		{
			crawler->Start();
            SendSimpleResponse( command, widgetEvent->EventID, widgetEvent->SocketID, true );
		}
        else if( crawlAction == "add_text" )
		{
			crawler->AddMessage( toWString( param ) );
            SendSimpleResponse( command, widgetEvent->EventID, widgetEvent->SocketID, true );
		}
        else if( crawlAction == "reset" )
		{
			crawler->Reset();
            SendSimpleResponse( command, widgetEvent->EventID, widgetEvent->SocketID, true );
		}
		else if( crawlAction == "clear" )
		{
			crawler->Clear();
            SendSimpleResponse( command, widgetEvent->EventID, widgetEvent->SocketID, true );
		}
        else if( crawlAction == "set_speed" )
		{
			float speed = 0.5;
			speed = (float)atof( param.c_str() );
			crawler->SetSpeed( speed );
            SendSimpleResponse( command, widgetEvent->EventID, widgetEvent->SocketID, true );
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
            SendSimpleErrorResponse( command, widgetEvent->EventID, widgetEvent->SocketID, "Could not get parameter" );

        SetParameter( paramPtr, (bv::TimeType)time, value );
        SendSimpleResponse( command, widgetEvent->EventID, widgetEvent->SocketID, true );
	}
    else
        SendSimpleErrorResponse( command, widgetEvent->EventID, widgetEvent->SocketID, "Unknown command" );
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
