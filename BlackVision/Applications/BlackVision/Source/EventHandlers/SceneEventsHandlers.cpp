#include "SceneEventsHandlers.h"

#include "Engine/Models/BVSceneEditor.h"
#include "Engine/Models/ModelNodeEditor.h"
#include "../BVAppLogic.h"
#include "../UseLogger.h"
#include "ProjectManager.h"
#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Tools/IncludeJSON.h"

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

        SendOnSceneStructureResponse( senderID, "LIST_PROJECTS_NAMES", "list", pList );
    }
    else if( command == ProjectEvent::Command::NewProject )
    {
        auto name = GetRequestParamValue( request )[ "projectName" ].asString();

        pm->AddNewProject( name );

        SendOnSceneStructureResponse( senderID, "NEW_PROJECT", "status", "OK" );
    }
    else if( command == ProjectEvent::Command::ListScenes )
    {
        auto name = GetRequestParamValue( request )[ "projectName" ].asString();
        auto sns = pm->ListScenesNames( name );

        auto pList = ToJSONArray( sns );

        SendOnSceneStructureResponse( senderID, "LIST_SCENES", "list", pList );
    }
    else if( command == ProjectEvent::Command::ListAssetsPaths )
    {
        auto projName = GetRequestParamValue( request )[ "projectName" ].asString();
        auto catName = GetRequestParamValue( request )[ "categoryName" ].asString();

        auto sns = pm->ListAssetsPaths( projName, catName );

        auto pList = ToJSONArray( sns );

        SendOnSceneStructureResponse( senderID, "LIST_ASSETS_PATHS", "list", pList );
    }
    else if( command == ProjectEvent::Command::ListCategoriesNames )
    {
        auto sns = pm->ListCategoriesNames();

        auto pList = ToJSONArray( sns );

        SendOnSceneStructureResponse( senderID, "LIST_CATEGORIES_NAMES", "list", pList );
    }
    else if( command == ProjectEvent::Command::SetCurrentProject )
    {
        auto projName = GetRequestParamValue( request )[ "projectName" ].asString();

        pm->SetCurrentProject( projName );

        SendOnSceneStructureResponse( senderID, "SET_CURRENT_PROJECT", "status", "OK" );
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

        SendOnSceneStructureResponse( senderID, "LIST_PROJECTS", "list", list );
    }
    else if( command == ProjectEvent::Command::LoadProject )
    {
        auto projName = std::string( request.begin(), request.end() );

        auto projectScenesNames = pm->ListScenesNames( projName );

        bool status = false;

        if( !projectScenesNames.empty() )
        {
            UpdatersManager::Get().RemoveAllUpdaters();
                
            auto node = m_appLogic->LoadScenes( projectScenesNames );
            if( node )
            {
                status = true;
            }
        }

        if( status )
        {
            SendOnSceneStructureResponse( senderID, "LOAD_PROJECT", "status", "OK" );
        }
        else
        {
            SendOnSceneStructureResponse( senderID, "LOAD_PROJECT", "status", "ERROR" );
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
    //printf("timeline evt\n");
    if( evt->GetEventType() == bv::TimeLineEvent::Type() )
    {
        bv::TimeLineEventPtr timelineEvent = std::static_pointer_cast<bv::TimeLineEvent>( evt );

        std::string& timeLineName = timelineEvent->TimelineName;
        float time = timelineEvent->Time;
        TimeLineEvent::Command command = timelineEvent->TimelineCommand;

        auto timeline = m_appLogic->GetTimelineManager()->GetTimeline( timeLineName );
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

// ***********************
//
void SceneEventsHandlers::WidgetHandler       ( bv::IEventPtr evt )
{
	//if( evt->GetEventType() == bv::WidgetEvent::Type() )
 //       return;

	//bv::WidgetEventPtr widgetEvent = std::static_pointer_cast<bv::WidgetEvent>( evt );        
 //   auto root = m_appLogic->GetBVScene()->GetModelSceneRoot();
 //       

 //   //todo: //fixme: wstring -> string
 //   wstring NodeName =  widgetEvent->NodeName;
 //   string NodeNameStr( NodeName.begin(), NodeName.end() );
 //   //todo: //fixme: wstring -> string
 //   wstring param = widgetEvent->Param;
 //   //string TexturePathStr( TexturePath.begin(), TexturePath.end() );
	//	

 //   auto node = root->GetNode(NodeNameStr);
 //   if(node==nullptr &&root->GetName()==NodeNameStr)
 //   {
 //       Log::A("OK", "root node is node you're looking for ["+ NodeNameStr+"] Applying jedi fix now.");
 //       node = root;
 //   }
 //   if(node==nullptr)
 //   {
 //       Log::A("error", "Error OnSetParam() node ["+ NodeNameStr+"] not found");
 //       return;
 //   }
	//	
	//BasicNodePtr nod      = std::static_pointer_cast< bv::model::BasicNode >(node);
	//		
	//	
 //   if(nod == nullptr)
 //   {
 //           Log::A("error", "Error OnWidgetCmd () node ["+ NodeNameStr+"] not found");
 //           return;
 //   }

	//INodeLogicPtr logic = nod->GetLogic();
	//if(logic==nullptr)
	//{
	//		Log::A("error", "Error OnWidgetCmd () node ["+ NodeNameStr+"] , logic [] not found");
 //           return;

	//}
	//	
	//INodeLogic* logic__ptr = logic.get();


 //   if(widgetEvent->WidgetName == L"crawl")
 //   {
	//	bv::widgets::Crawler* crawler =  (bv::widgets::Crawler*)logic__ptr;
	//	if(widgetEvent->Action==L"stop")
	//	{
	//		Log::A("OK","crawl stop...");
	//		crawler->Stop();
	//	}
	//	else if(widgetEvent->Action==L"start")
	//	{
	//		Log::A("OK","crawl start...");
	//		crawler->Start();
	//	}else if(widgetEvent->Action==L"add_text")
	//	{
	//		Log::A(L"OK",L"crawl add text..."+ widgetEvent->Param);
	//		crawler->AddMessage(widgetEvent->Param);
	//	}else if(widgetEvent->Action==L"reset")
	//	{
	//		Log::A(L"OK",L"crawl reset...");
	//		crawler->Reset();
	//	}
	//	else if(widgetEvent->Action==L"clear")
	//	{
	//		Log::A(L"OK",L"crawl clear...");
	//		crawler->Clear();
	//	}else if(widgetEvent->Action==L"set_speed")
	//	{
	//		Log::A(L"OK",L"crawl set speed.."+ widgetEvent->Param);
	//		float speed = 0.5;
	//		string s_speed( widgetEvent->Param.begin(), widgetEvent->Param.end() );
	//		speed = (float)atof(s_speed.c_str());
	//		crawler->SetSpeed(speed);

	//	}

	//}else if(widgetEvent->WidgetName == L"counter")
 //   {
	//	bv::widgets::WidgetCounter * counter =  (bv::widgets::WidgetCounter*)logic__ptr;

	//	string param_name_cast= string(widgetEvent->Param.begin(), widgetEvent->Param.end());

	//	auto param = counter->GetValueParam();
	//	if(param==nullptr)
	//	{
	//		Log::A("error", "Error OnSetParam() plugin [counter] param ["+param_name_cast+"] not found");

	//	}else{

	//			wstring value = widgetEvent->Value;
	//			 
	//			float float_value = 1.0f;

	//			try{
	//			float_value =  boost::lexical_cast<float>(value);
	//			}catch(boost::bad_lexical_cast&)
	//			{
	//			float_value = 0.0f;
	//			}

	//			SetParameter( param, (bv::TimeType)widgetEvent->Time, float_value);
	//	}
	//}
}

} //bv
