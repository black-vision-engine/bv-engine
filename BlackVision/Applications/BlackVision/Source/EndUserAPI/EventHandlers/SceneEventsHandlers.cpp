#include "SceneEventsHandlers.h"

#include "Engine/Models/BVSceneEditor.h"
#include "Engine/Models/ModelNodeEditor.h"
#include "../../BVAppLogic.h"
#include "../../UseLogger.h"
#include "Tools/IncludeJSON.h"

#include "ProjectManager.h"
#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Widgets/Crawler/CrawlerEvents.h"
#include "Widgets/Counter/Counter.h"
#include "Engine/Events/EventHelpers.h"
#include "Engine/Models/Plugins/Simple/DefaultTextPlugin.h"

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
void SceneEventsHandlers::NodeStructure      ( bv::IEventPtr evt )
{
    if( evt->GetEventType() != bv::NodeStructureEvent::Type() )
        return;
    bv::NodeStructureEventPtr structureEvent = std::static_pointer_cast<bv::NodeStructureEvent>( evt );

    std::string& nodeName = structureEvent->NodeName;
    std::string& newNodeName = structureEvent->NewNodeName;
    std::string& pluginName = structureEvent->PluginName;
    auto command = structureEvent->SceneCommand;

    auto root = m_appLogic->GetBVScene()->GetModelSceneRoot();
    auto node = root->GetNode( nodeName );
    if( node == nullptr )
    {
        if( root->GetName() == nodeName )
            node = root;
        else
        {
            LOG_MESSAGE( SeverityLevel::error ) << "SceneStructureEvent() node ["+ nodeName +"] not found";
            return;
        }
    }

    if( command == NodeStructureEvent::Command::AddNode )
    {
        auto newNode = model::BasicNode::Create( newNodeName, m_appLogic->GetTimelineManager()->GetRootTimeline() );
		newNode->AddPlugin( "DEFAULT_TRANSFORM", "transform", m_appLogic->GetTimelineManager()->GetRootTimeline() ); 

		m_appLogic->GetBVScene()->GetSceneEditor()->AddChildNode( node, newNode );
    }
    else if( command == NodeStructureEvent::Command::RemoveNode )
    {
        auto parentNodeName = nodeName.substr( 0, nodeName.find_last_of("/") );
        auto childNode = nodeName.substr( nodeName.find_last_of("/") + 1 );
        auto parentNode = root->GetNode( parentNodeName );
			
        m_appLogic->GetBVScene()->GetSceneEditor()->DeleteChildNode( parentNode, childNode );
    }
    else if( command == NodeStructureEvent::Command::AttachPlugin )
    {
        bv::model::BasicNodePtr basicNode = std::static_pointer_cast< bv::model::BasicNode >( node );
        
        unsigned int endIndex = std::numeric_limits<unsigned int>::max();
        basicNode->GetModelNodeEditor()->AttachPlugin( endIndex );
    }
    else if( command == NodeStructureEvent::Command::DetachPlugin )
    {
        bv::model::BasicNodePtr basicNode = std::static_pointer_cast< bv::model::BasicNode >( node );
        basicNode->GetModelNodeEditor()->DetachPlugin( pluginName );
    }
    else if( command == NodeStructureEvent::Command::SetNodeVisible )
        node->SetVisible( true );
    else if( command == NodeStructureEvent::Command::SetNodeInvisible )
        node->SetVisible( false );

}


// *********************************
//
Json::Value ToJSONArray( const PathVec & v )
{
    Json::Value root;

    for( auto pn : v )
    {
        root.append( pn.Str() );
    }

    return root;
}

// *********************************
//
Json::Value ToJSONArray( const StringVector & v )
{
    Json::Value root;

    for( auto s : v )
    {
        root.append( s );
    }

    return root;
}
// *********************************
//
Json::Value Str2Json( const std::string & data )
{
    return Json::Value( data );
}

// *********************************
//
Json::Value GetRequestParamValue( std::string& request )
{
    return Str2Json( std::string( request.begin(), request.end() ) );
}

// *********************************
//
void SendOnSceneStructureResponse( int socketID, const std::string & cmd, const std::string & msgKey, const Json::Value & msgVal )
{
    LOG_MESSAGE( SeverityLevel::info ) << cmd << " OK";

    Json::Value scenes;

    scenes[ "cmd" ] = cmd;
    scenes[ msgKey ] = msgVal;

    std::string S = scenes.toStyledString();

    wstring WS = wstring( S.begin(), S.end() );

    ResponseEventPtr responseEvent = std::make_shared<ResponseEvent>();
    responseEvent->Response = WS;
    responseEvent->SocketID = socketID;
    GetDefaultEventManager().QueueResponse( responseEvent );
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

    if( command == ProjectEvent::Command::ListProjectNames )
    {
        auto pns = pm->ListProjectsNames();

        auto pList = ToJSONArray( pns );

        SendOnSceneStructureResponse( senderID, "ListProjectNames", "list", pList );
    }
    else if( command == ProjectEvent::Command::NewProject )
    {
        auto name = GetRequestParamValue( request )[ "projectName" ].asString();

        pm->AddNewProject( name );

        SendOnSceneStructureResponse( senderID, "NewProject", "status", "OK" );
    }
    else if( command == ProjectEvent::Command::ListScenes )
    {
        auto name = GetRequestParamValue( request )[ "projectName" ].asString();
        auto sns = pm->ListScenesNames( name );

        auto pList = ToJSONArray( sns );

        SendOnSceneStructureResponse( senderID, "ListScenes", "list", pList );
    }
    else if( command == ProjectEvent::Command::ListAssetsPaths )
    {
        auto projName = GetRequestParamValue( request )[ "projectName" ].asString();
        auto catName = GetRequestParamValue( request )[ "categoryName" ].asString();

        auto sns = pm->ListAssetsPaths( projName, catName );

        auto pList = ToJSONArray( sns );

        SendOnSceneStructureResponse( senderID, "ListAssetPaths", "list", pList );
    }
    else if( command == ProjectEvent::Command::ListCategoriesNames )
    {
        auto sns = pm->ListCategoriesNames();

        auto pList = ToJSONArray( sns );

        SendOnSceneStructureResponse( senderID, "ListCategoriesNames", "list", pList );
    }
    else if( command == ProjectEvent::Command::SetCurrentProject )
    {
        auto projName = GetRequestParamValue( request )[ "projectName" ].asString();

        pm->SetCurrentProject( projName );

        SendOnSceneStructureResponse( senderID, "SetCurrentProject", "status", "OK" );
    }
    else if( command == ProjectEvent::Command::ListProjects )
    {
        auto pns = pm->ListProjectsNames();

        Json::Value list;

        for( auto p : pns )
        {
            auto scenesCount = pm->ListScenesNames( p ).size();

            Json::Value entry;
            entry[ "name" ] = p.Str();
            entry[ "scenes_count" ] = scenesCount;

            list.append( entry );
        }

        SendOnSceneStructureResponse( senderID, "ListProjects", "list", list );
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
//     else if( command == ProjectEvent::Command::SaveScene )
//     {
//         auto root = m_appLogic->GetBVScene()->GetModelSceneRoot();
//         auto node = root->GetNode( nodeName );

		//if( node == nullptr && root->GetName() == nodeName )
		//{
		//	//Log::A( "OK", "root node is node you're looking for [" + nodeName + "] Applying jedi fix now." );
		//	node = root;
		//}

//         auto basicNode = std::static_pointer_cast< model::BasicNode >( node );

//         auto projName = std::string( request.begin(), request.end() );

//         pm->AddScene( basicNode, "proj01", "dupa.scn" );


//         SendOnSceneStructureResponse( "SAVE_SCENE", "status", "OK" );
//     }
}

// ***********************
//
void SceneEventsHandlers::TimelineHandler     ( bv::IEventPtr evt )
{
    if( evt->GetEventType() == bv::TimeLineEvent::Type() )
    {
        bv::TimeLineEventPtr timelineEvent = std::static_pointer_cast<bv::TimeLineEvent>( evt );

        std::string& timeLineName = timelineEvent->TimelineName;
        float time = timelineEvent->Time;
        TimeLineEvent::Command command = timelineEvent->TimelineCommand;

        auto scene = m_appLogic->GetBVScene()->GetScene( timelineEvent->SceneName );

        if( scene )
        {
            auto timeline = scene->m_pTimelineManager->GetTimeline( timeLineName );

            if( timeline == nullptr )
            {
                LOG_MESSAGE( SeverityLevel::error ) << "Timeline ["+ timeLineName + "] does not exist.";
                return;
            }
            bv::model::ITimeline* timelineTyped = static_cast<bv::model::ITimeline*>(timeline.get());


            if( command == TimeLineEvent::Command::Play )
            {
                timelineTyped->SetPlayDirection( bv::TimelinePlayDirection::TPD_FORWAD );
                timelineTyped->Play();
            }
            else if( command == TimeLineEvent::Command::Stop )
            {
                timelineTyped->Stop();
            }
            else if( command == TimeLineEvent::Command::PlayReverse )
            {
                timelineTyped->SetPlayDirection( bv::TimelinePlayDirection::TPD_BACKWARD );
                timelineTyped->Play();
            }
            else if( command == TimeLineEvent::Command::Goto )
            {
                timelineTyped->SetPlayDirection( bv::TimelinePlayDirection::TPD_FORWAD );
                timelineTyped->SetTimeAndStop( (bv::TimeType)time );
            }
            else if( command == TimeLineEvent::Command::GotoAndPlay )
            {
                timelineTyped->SetPlayDirection( bv::TimelinePlayDirection::TPD_FORWAD );
                timelineTyped->SetTimeAndPlay( (bv::TimeType)time );
            }
       
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
    auto root = m_appLogic->GetBVScene()->GetModelSceneRoot();
        
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

                model::DefaultTextPlugin::SetText( pl, L"nowa wiadomoœæ "+to_wstring(examplesIndex) );
                examplesIndex=(examplesIndex+1)%20;

				typedEvent->GetCrawler()->EnqueueNode( n );
			}
		}
	}
}

} //bv
