#include "Engine/Models/BasicNode.h"

#include "RemoteControlInterface.h"
#include "Engine/Models/Interfaces/IOverrideState.h"
#include "Engine/Models/BasicNode.h"
//#include "Engine/Models/ModelScene.h"
#include "Engine/Models/Plugins/Interfaces/IParameter.h"
#include "Engine/Models/Plugins/Simple/DefaultTextPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTimerPlugin.h"
//#include "Engine/Models/Resources/TextureHelpers.h"
#include "Engine/Models/Plugins/PluginUtils.h"
#include "Engine/Models/Plugins/Custom/DefaultHeightMapPlugin.h"

#include "structure/AssetManager.h"

#include "Engine/Events/Interfaces/IEventManager.h"
#include "Widgets/Crawler/CrawlerEvents.h"
#include "Widgets/Counter/Counter.h"
#include "Engine/Events/Events.h"
#include "ProjectManager.h"

#pragma warning(push)
#pragma warning( disable: 4512 )
#include "json/json.h"
#pragma warning(pop)

#include "Log.h"
#include "SocketWrapper.h"

#include "PerformanceMonitor.h"

#include "Engine\Models\BVSceneEditor.h"
#include "Engine\Models\ModelNodeEditor.h"

#include <iomanip>
#include <string>
#include <sstream>


namespace bv{

RemoteControlInterface::RemoteControlInterface(BVAppLogic *AppLogic)
	:m_cyclistPos( 3, 0.f )
    , m_concentrateDistMarks( 9, 0.f )
{

	m_AppLogic = AppLogic;

	// SetParamEvent
    bv::GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &RemoteControlInterface::OnSetParam ), bv::SetParamEvent::Type() );
    bv::GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &RemoteControlInterface::OnInformation ), bv::InfoEvent::Type() );
	bv::GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &RemoteControlInterface::OnSceneStructure ), bv::SceneStructureEvent::Type() );

    // timeline events
    bv::GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &RemoteControlInterface::OnTimelineCmd ), bv::TimeLineCmd::Type() );

    // timer events
    bv::GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &RemoteControlInterface::OnTimerCmd ), bv::TimerCmd::Type() );

	// widget events
    bv::GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &RemoteControlInterface::OnWidgetCmd ), bv::WidgetCmd::Type() );


	GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &RemoteControlInterface::OnNodeAppearing ), widgets::NodeAppearingCrawlerEvent::Type() );
	GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &RemoteControlInterface::OnNodeLeaving ), widgets::NodeLeavingCrawlerEvent::Type() );

}


RemoteControlInterface::~RemoteControlInterface(void)
{
	/*GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &RemoteControlInterface::OnUpdateParam ), SetTransformParamsEvent::Type() );
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &RemoteControlInterface::OnUpdateParam ), SetColorParamEvent::Type() );*/

}


std::string SerializeNode( model::BasicNodePtr node )
{
    string S="";
    string tmp="";
    string children="";
    string plugins="";
	int NumChildren = (node)->GetNumChildren();
    S="{";
    S+="\"node_name\":\""+node->GetName()+"\",";
    

    auto pluginlist = node->GetPluginList();

    for( unsigned int i = 0; i < pluginlist->NumPlugins(); ++i )
    {
        string plugin = pluginlist->GetPlugin( i )->GetName();
        if(plugins!="")plugins+=",";
        plugins            +=  "{\"plugin_name\":\""+plugin+"\"}";
        
    }

    S+="\"node_plugins\":["+plugins+"],";


    for(int i=0;i<NumChildren;i++)
    {

		model::IModelNodePtr ptr   = node->GetChild(i);
        model::IModelNodePtr ptr2    = ptr;
        model::BasicNodePtr nod      = std::static_pointer_cast< model::BasicNode >( ptr2 );
        if(children!="") children+=",";
        children            +=  SerializeNode(nod);
    }

    S+="\"node_children\":["+children+"]";
    S+="}";

    return S;
}

void ReqPrint( model::BasicNodePtr node, int level )
{
    string temp="-";
    for( int i = 1; i < level; i++ )
    {
        temp+="-";
    }

    Log::A("OK",temp+node->GetName());
    
    int NumChildren = (node)->GetNumChildren();
    for(int i=0;i<NumChildren;i++)
    {

		model::IModelNodePtr ptr   = node->GetChild(i);
        model::IModelNodePtr ptr2 = ptr;
        model::BasicNodePtr nod = std::static_pointer_cast< model::BasicNode >( ptr2 );
        ReqPrint(nod,level+1);
    }

}

// *********************************
//
namespace 
{
// *********************************
//
std::string ParamTypeToString( ModelParamType pType )
{
    switch( pType )
    {
    case ModelParamType::MPT_FLOAT:
        return "float";
    case ModelParamType::MPT_MAT2:
        return "mat2";
    case ModelParamType::MPT_VEC2:
        return "vec2";
    case ModelParamType::MPT_VEC3:
        return "vec3";
    case ModelParamType::MPT_VEC4:
        return "vec4";
    case ModelParamType::MPT_TRANSFORM:
        return "transform";
    case ModelParamType::MPT_TRANSFORM_VEC:
        return "transform_vec";
    case ModelParamType::MPT_INT:
        return "int";
    case ModelParamType::MPT_BOOL:
        return "bool";
    case ModelParamType::MPT_ENUM:
        return "enum";
    default:
        assert( !"Should never be here" );
        return "";
    }
}

namespace 
{
	// FIXME: Should be moved to some core module.
	template< typename T > 
	std::string toString( const T & t )
	{
		return std::to_string( t );
	}

    // *********************************
    //
	template<> 
	std::string toString< std::string >( const std::string & t )
	{
		return t;
	}

    // *********************************
    //
    template<> 
	std::string toString< glm::vec4 >( const glm::vec4 & v )
	{
		return toString( v[ 0 ] ) + ", " + toString( v[ 1 ] ) + ", "  + toString( v[ 2 ] ) + ", "  + toString( v[ 3 ] );
	}

    // *********************************
    //
    template<> 
	std::string toString< glm::vec3 >( const glm::vec3 & v )
	{
		return toString( v[ 0 ] ) + ", " + toString( v[ 1 ] ) + ", "  + toString( v[ 2 ] );
	}
    
    // *********************************
    //
    template<> 
	std::string toString< glm::vec2 >( const glm::vec2 & v )
	{
		return toString( v[ 0 ] ) + ", " + toString( v[ 1 ] );
	}

} // anonymous

// *********************************
//
template< typename ParamTypePtr >
Json::Value GetParamDescription( IParameterPtr p )
{
    string s_name = p->GetName();
    auto paramType = p->GetType();

    Json::Value entry;

    entry[ "name" ] = s_name;
    entry[ "type" ] = ParamTypeToString( paramType );

    Json::Value jsonKeys;

    auto typedParam = QueryTypedParam< ParamTypePtr >( p );
    auto accessIntepolator = typedParam->AccessInterpolator();
    auto keys = accessIntepolator.AccessKeys();
    for( auto & k : keys )
    {
        jsonKeys.append( toString( k.t ) );
        jsonKeys.append( toString( k.val ) );
    }

    entry[ "keys" ] = jsonKeys;

    return entry;
}

} // anonymous

// *********************************
//
void RemoteControlInterface::OnInformation ( bv::IEventPtr evt )
{
    if( evt->GetEventType() == bv::InfoEvent::m_sEventType)
    {
        bv::InfoEventPtr evtInfo = std::static_pointer_cast<bv::InfoEvent>( evt );
        if(evtInfo->request==L"tree_structure")
        {
            Log::A("OK","Tree structure:");
			
			
            ReqPrint(m_AppLogic->GetBVScene()->GetModelSceneRoot(),1);
            std::string S;
			S="{\"cmd\":\"scene_tree\",\"tree\": "+SerializeNode(m_AppLogic->GetBVScene()->GetModelSceneRoot())+"}";
            Log::A("OK",S);
            wstring WS = wstring(S.begin(),S.end());

            ResponseMsg msg;
            msg.msg     = WS;
            msg.sock_id = evtInfo->sock_id;
            SocketWrapper::AddMsg(msg);

        }else if(evtInfo->request==L"performance")
        {
            Log::A("SENDING","Performance:");
            
            PerformanceMonitor::Calculate(m_AppLogic->GetStatsCalculator());
            string S = "{\"cmd\":\"performance\",\"fps\":\""+PerformanceMonitor::Stats.fps+"\",\"fps_avg\":\""+PerformanceMonitor::Stats.fps_avg+"\",\"ram\":\""+PerformanceMonitor::Stats.ram+"\",\"vram\":\""+PerformanceMonitor::Stats.vram+"\",\"cpu\":\""+PerformanceMonitor::Stats.cpu+"\" }";
           
            Log::A("OK",S);
            wstring WS = wstring(S.begin(),S.end());

            ResponseMsg msg;
            msg.msg     = WS;
            msg.sock_id = evtInfo->sock_id;
            SocketWrapper::AddMsg(msg);

        }else if(evtInfo->request==L"timelines")
        {
            
            Log::A("SENDING","Timelines info...:");

			model::TimelineManager * TM = m_AppLogic->GetTimeLineManager();

			auto timelines = TM->GetRootTimeline()->GetChildren();
			string s_timelines;
			bool first = true;
			for( auto timeline : timelines)
			{
				if(!first)
					s_timelines +=",";

				string s_name = timeline->GetName();
				string s_time = to_string(timeline->GetLocalTime());

				bv::model::ITimeline* timeline_cast = static_cast<bv::model::ITimeline*>(timeline.get());
				
				string s_timeline = "{\"name\":\""+s_name+"\",\"time\":\""+s_time+"\",\"keyframes\":[";
				string s_keyframes="";
				for(unsigned int i=0;i<timeline_cast->NumKeyFrames();i++)
				{
					if(i>0)
						s_keyframes += ",";
					
					const model::ITimelineEvent* keyframe = timeline_cast->GetKeyFrameEvent(i);
					string s_keyframe_name = keyframe->GetName();
					TimelineEventType keyframe_type = keyframe->GetType();
					string s_keyframe_type = "NONE";
					switch(keyframe_type)
					{
						case TimelineEventType::TET_LOOP:
							s_keyframe_type = "loop";
							break;
						case TimelineEventType::TET_STOP:
							s_keyframe_type = "stop";
							break;
						case TimelineEventType::TET_NULL:
							s_keyframe_type = "null";
							break;
						case TimelineEventType::TET_TOTAL:
							s_keyframe_type = "total";
							break;
						default:
							break;
					}
					bv::TimeType f_time = keyframe->GetEventTime();
					string s_info = "-";


					string s_keyframe_code="{\"name\":\""+s_keyframe_name+"\",\"type\":\""+s_keyframe_type+"\",\"time\":\""+to_string(f_time)+"\",\"info\":\""+s_info+"\"}";

					s_keyframes+=s_keyframe_code;

				}
				s_timeline+=s_keyframes+"]}";
				
				s_timelines += s_timeline;;
				first = false;
			}
			s_timelines = "["+s_timelines+"]";
   
            PerformanceMonitor::Calculate(m_AppLogic->GetStatsCalculator());
            string S = "{\"cmd\":\"timelines\", \"timelines\":"+s_timelines+" }";
           
            Log::A("SENDING",S);
            wstring WS = wstring(S.begin(),S.end());

            ResponseMsg msg;
            msg.msg     = WS;
            msg.sock_id = evtInfo->sock_id;
            SocketWrapper::AddMsg(msg);

        }
        else if( evtInfo->request == L"node_info" )
        {
            wstring nodeName = evtInfo->NodeName;
			string nodeNameStr( nodeName.begin(), nodeName.end() );
			//todo: //fixme: wstring -> string
		    auto root = m_AppLogic->GetBVScene()->GetModelSceneRoot();
			auto node = root->GetNode( nodeNameStr );

			if( node == nullptr && root->GetName() == nodeNameStr )
			{
				Log::A( "OK", "root node is node you're looking for [" + nodeNameStr + "] Applying jedi fix now." );
				node = root;
			}

			if( node==nullptr )
			{
				Log::A( "error", "Error NodeInfo() node [" + nodeNameStr + "] not found" );
				return;
			}

			bool visible    = node->IsVisible();
			auto pluginlist = node->GetPluginList();

            Json::Value res;
            res[ "cmd" ]        = "node_info";
            res[ "visible" ]    = visible;

            Json::Value jsonParams;

			for( unsigned int i = 0; i < pluginlist->NumPlugins(); ++i )
			{
				IPluginPtr plugin = pluginlist->GetPlugin( i );
				string plugin_name = plugin->GetName();

				auto & params = plugin->GetPluginParamValModel()->GetPluginModel()->GetParameters();

				for( auto p : params )
				{
                    switch( p->GetType() )
                    {
                        case ModelParamType::MPT_FLOAT:
                            jsonParams.append( GetParamDescription< ParamFloatPtr >( p ) );
                        case ModelParamType::MPT_MAT2:
                            assert( !"Not imeplemented" );  // TODO: Implement this case.
                            //jsonParams.append( GetParamDescription< ParamMat2Ptr >( p ) );
                        case ModelParamType::MPT_VEC2:
                            jsonParams.append( GetParamDescription< ParamVec2Ptr >( p ) );
                        case ModelParamType::MPT_VEC3:
                            jsonParams.append( GetParamDescription< ParamVec3Ptr >( p ) );
                        case ModelParamType::MPT_VEC4:
                            jsonParams.append( GetParamDescription< ParamVec4Ptr >( p ) );
                        case ModelParamType::MPT_TRANSFORM:
                            assert( !"Not imeplemented" );  // TODO: Implement this case. AccessInterpolator' : is not a member of 'bv::model::ParamTransform'
                        case ModelParamType::MPT_TRANSFORM_VEC:
                            assert( !"Not imeplemented" );  // TODO: Implement this case. AccessInterpolator' : is not a member of 'bv::model::ParamTransformVec'
                        case ModelParamType::MPT_INT:
                            jsonParams.append( GetParamDescription< ParamIntPtr >( p ) );
                        case ModelParamType::MPT_BOOL:
                            jsonParams.append( GetParamDescription< ParamBoolPtr >( p ) );
                        case ModelParamType::MPT_ENUM:
                            assert( !"Not imeplemented" );  // TODO: Implement this case. No idea how TypeEnum is a tamplate class.
                    }
                }
            }
           
            res[ "params" ]     = jsonParams;

            auto resStr = res.toStyledString();

            Log::A( "SENDING", resStr );
            wstring WS = wstring( resStr.begin(), resStr.end() );

            ResponseMsg msg;
            msg.msg     = WS;
            msg.sock_id = evtInfo->sock_id;
            SocketWrapper::AddMsg(msg);
        }
        else if(evtInfo->request==L"videocards")
        {
            Json::Value val;
            val[ "cmd" ]        = "videocards";
            val[ "visible" ]    = " no diggy diggy ";

            string S = val.toStyledString();
           
            Log::A( "SENDING", S );
            wstring WS = wstring( S.begin(),S.end() );

            ResponseMsg msg;
            msg.msg     = WS;
            msg.sock_id = evtInfo->sock_id;
            SocketWrapper::AddMsg(msg);

        }
        else if( evtInfo->request == L"grab_that_frame" )
		{
            const std::wstring & path = evtInfo->GetAddStrData();
			string s_path( path.begin(),path.end() );

            m_AppLogic->GrabCurrentFrame( s_path );
		}
		else if( evtInfo->request == L"key_on" )
		{
			Log::A("KEY", "ON");
            m_AppLogic->SetKey( true );
		}
		else if( evtInfo->request == L"key_off" )
		{
			Log::A("KEY", "OFF");
			
            m_AppLogic->SetKey( false );
		}
    }
}


namespace 
{


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
Json::Value GetRequestParamValue( const bv::SceneStructureEventPtr & evtStructure )
{
    return Str2Json( std::string( evtStructure->request.begin(), evtStructure->request.end() ) );
}

// *********************************
//
void SendOnSceneStructureResponse( const bv::SceneStructureEventPtr & evtStructure, const std::string & cmd, const std::string & msgKey, const Json::Value & msgVal )
{
    Log::A( "OK", cmd );

    Json::Value scenes;

    scenes[ "cmd" ] = cmd;
    scenes[ msgKey ] = msgVal;

    std::string S = scenes.toStyledString();

    wstring WS = wstring( S.begin(), S.end() );

    ResponseMsg msg;
    msg.msg     = WS;
    msg.sock_id = evtStructure->sock_id;
    SocketWrapper::AddMsg( msg );
}

}

// *********************************
//
void RemoteControlInterface::OnSceneStructure ( bv::IEventPtr evt )
{
    if( evt->GetEventType() == bv::SceneStructureEvent::m_sEventType)
    {
		bv::SceneStructureEventPtr evtStructure = std::static_pointer_cast<bv::SceneStructureEvent>( evt );

        auto pm = ProjectManager::GetInstance( m_AppLogic->GetTimeLineManager() );

        if(evtStructure->command==L"REMOVE_NODE")
        {
            wstring NodeName = evtStructure->NodeName;
			string NodeNameStr( NodeName.begin(), NodeName.end() );
		    auto root = m_AppLogic->GetBVScene()->GetModelSceneRoot();
			auto node = root->GetNode(NodeNameStr);
			
			if(node==nullptr &&root->GetName()==NodeNameStr)
			{
				Log::A("OK", "root node is node you're looking for ["+ NodeNameStr+"] Applying jedi fix now.");
				node = root;
			}
			if(node==nullptr)
			{
				Log::A("error", "Error NodeInfo() node ["+ NodeNameStr+"] not found");
				return;
			}

			auto nodeName_parrent = NodeNameStr.substr(0,NodeNameStr.find_last_of("/"));
			auto node_child= NodeNameStr.substr(NodeNameStr.find_last_of("/")+1);
			auto node_parrent = root->GetNode(nodeName_parrent);
			
			m_AppLogic->GetBVScene()->GetSceneEditor()->DeleteChildNode( node_parrent, node_child );
        }
		if(evtStructure->command==L"ADD_NODE")
        {
            wstring NodeName = evtStructure->NodeName;
			string NodeNameStr( NodeName.begin(), NodeName.end() );
			wstring NodeName2 = evtStructure->NodeName2;
			string NodeNameStr2( NodeName2.begin(), NodeName2.end() );
		    auto root = m_AppLogic->GetBVScene()->GetModelSceneRoot();
			auto node = root->GetNode(NodeNameStr);
			
			if(node==nullptr &&root->GetName()==NodeNameStr)
			{
				Log::A("OK", "root node is node you're looking for ["+ NodeNameStr+"] Applying jedi fix now.");
				node = root;
			}
			if(node==nullptr)
			{
				Log::A("error", "Error NodeInfo() node ["+ NodeNameStr+"] not found");
				return;
			}


			auto newNode = model::BasicNode::Create(NodeNameStr2,m_AppLogic->GetTimeLineManager()->GetRootTimeline());
			newNode->AddPlugin( "DEFAULT_TRANSFORM", "transform",m_AppLogic->GetTimeLineManager()->GetRootTimeline() ); 

			m_AppLogic->GetBVScene()->GetSceneEditor()->AddChildNode(node,newNode);
        }
		if( evtStructure->command == L"DETACH_PLUGIN" )
        {
            wstring NodeName = evtStructure->NodeName;
			string NodeNameStr( NodeName.begin(), NodeName.end() );
			
		    auto root = m_AppLogic->GetBVScene()->GetModelSceneRoot();
			auto node = root->GetNode(NodeNameStr);
			
			if(node==nullptr &&root->GetName()==NodeNameStr)
			{
				Log::A("OK", "root node is node you're looking for ["+ NodeNameStr+"] Applying jedi fix now.");
				node = root;
			}
			if(node==nullptr)
			{
				Log::A("error", "Error NodeInfo() node ["+ NodeNameStr+"] not found");
				return;
			}

			bv::model::BasicNodePtr node_ptr      = std::static_pointer_cast< bv::model::BasicNode >(node);

			node_ptr->GetModelNodeEditor()->DetachPlugin("text");
			
        }
        else if( evtStructure->command == L"LIST_PROJECTS_NAMES" )
        {
            auto pns = pm->ListProjectsNames();

            auto pList = ToJSONArray( pns );

            SendOnSceneStructureResponse( evtStructure, "LIST_PROJECTS_NAMES", "list", pList );
        }
        else if( evtStructure->command == L"NEW_PROJECT" )
        {
            auto name = GetRequestParamValue( evtStructure )[ "projectName" ].asString();

            pm->AddNewProject( name );

            SendOnSceneStructureResponse( evtStructure, "NEW_PROJECT", "status", "OK" );
        }
        else if( evtStructure->command == L"LIST_SCENES" )
        {
            auto name = GetRequestParamValue( evtStructure )[ "projectName" ].asString();
            auto sns = pm->ListScenesNames( name );

            auto pList = ToJSONArray( sns );

            SendOnSceneStructureResponse( evtStructure, "LIST_SCENES", "list", pList );
        }
        else if( evtStructure->command == L"LIST_ASSETS_PATHS" )
        {
            auto projName = GetRequestParamValue( evtStructure )[ "projectName" ].asString();
            auto catName = GetRequestParamValue( evtStructure )[ "categoryName" ].asString();

            auto sns = pm->ListAssetsPaths( projName, catName );

            auto pList = ToJSONArray( sns );

            SendOnSceneStructureResponse( evtStructure, "LIST_ASSETS_PATHS", "list", pList );
        }
        else if( evtStructure->command == L"LIST_CATEGORIES_NAMES" )
        {
            auto sns = pm->ListCategoriesNames();

            auto pList = ToJSONArray( sns );

            SendOnSceneStructureResponse( evtStructure, "LIST_CATEGORIES_NAMES", "list", pList );
        }
        else if( evtStructure->command == L"LIST_CATEGORIES_NAMES" )
        {
            auto sns = pm->ListCategoriesNames();

            auto pList = ToJSONArray( sns );

            SendOnSceneStructureResponse( evtStructure, "LIST_CATEGORIES_NAMES", "list", pList );
        }
        else if( evtStructure->command == L"SET_CURRENT_PROJECT" )
        {
            auto projName = GetRequestParamValue( evtStructure )[ "projectName" ].asString();

            pm->SetCurrentProject( projName );

            SendOnSceneStructureResponse( evtStructure, "SET_CURRENT_PROJECT", "status", "OK" );
        }
        else if( evtStructure->command == L"LIST_PROJECTS" )
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

            SendOnSceneStructureResponse( evtStructure, "LIST_PROJECTS", "list", list );
            // [czesio]
            // [{"name":"czesio", "scenes_count":123},{...},...]
        }
    }
}


//***************************************
//
void RemoteControlInterface::OnTimelineCmd ( bv::IEventPtr evt )
{
    printf("timeline evt\n");
    if( evt->GetEventType() == bv::TimeLineCmd::m_sEventType)
    {
        bv::TimeLineCmdPtr evtTimeline = std::static_pointer_cast<bv::TimeLineCmd>( evt );

        //todo: patch string converting
        string timeline_name(evtTimeline->TimelineName.begin(),evtTimeline->TimelineName.end());

        auto timeline = m_AppLogic->GetTimeLineManager()->GetTimeline(timeline_name);
        if(timeline==nullptr)
        {
            Log::A(L"ERROR",L"timeline ["+evtTimeline->TimelineName+L"] does not exist :(");
            return;
        }
        bv::model::ITimeline* timeline_cast = static_cast<bv::model::ITimeline*>(timeline.get());

        if(evtTimeline->cmd==L"play")
        {
            timeline_cast->SetPlayDirection(bv::TimelinePlayDirection::TPD_FORWAD);
            timeline_cast->Play();
            
            
            printf("play\n");
        }else if(evtTimeline->cmd==L"stop")
        {
            timeline_cast->Stop();
            printf("stop\n");
        }else if(evtTimeline->cmd==L"play_reverse")
        {
            timeline_cast->SetPlayDirection(bv::TimelinePlayDirection::TPD_BACKWARD);
            timeline_cast->Play();
            
            printf("play reverse\n");
        }else if(evtTimeline->cmd==L"goto")
        {
            timeline_cast->SetPlayDirection(bv::TimelinePlayDirection::TPD_FORWAD);
            timeline_cast->SetTimeAndStop((bv::TimeType)evtTimeline->value);
            
            printf("goto\n");
        }
        else if(evtTimeline->cmd==L"gotoandplay")
        {
            timeline_cast->SetPlayDirection(bv::TimelinePlayDirection::TPD_FORWAD);
            timeline_cast->SetTimeAndPlay((bv::TimeType)evtTimeline->value);
            //timeline_cast->
            printf("gotoandplay\n");
        }
       
    }
    
}
    

//***************************************
//
void RemoteControlInterface::OnTimerCmd ( bv::IEventPtr evt )
{
    printf("timer evt\n");
    if( evt->GetEventType() == bv::TimerCmd::m_sEventType)
    {
        bv::TimerCmdPtr evtTimer = std::static_pointer_cast<bv::TimerCmd>( evt );

		BVScenePtr modelScene = m_AppLogic->GetBVScene();

        auto root = modelScene->GetModelSceneRoot();
        

        //todo: //fixme: wstring -> string
        wstring NodeName = evtTimer->NodeName;
        string NodeNameStr( NodeName.begin(), NodeName.end() );
        //todo: //fixme: wstring -> string
        printf("#%s#\n",NodeNameStr.c_str());
       if(NodeNameStr=="root/Timer")
       {
            if(evtTimer->cmd==L"hm_enable")
            {
                printf("hm enable\n");
                 modelScene->GetModelSceneRoot()->GetChild(".")->SetVisible(true);
				 
                //hm_enable = true;
            
            }else if(evtTimer->cmd==L"hm_start")
            {
                printf("hm start\n");
                //this->HMStart(evtTimer->H);
            }else if(evtTimer->cmd==L"hm_reset")
            {
                modelScene->GetModelSceneRoot()->GetChild(".")->SetVisible(false);
               // hm_enable = false;
                //this->HMReset();
            }else if(evtTimer->cmd==L"hm_show")
            {
                printf("hm show\n");
                //this->HMShow();
            }else if(evtTimer->cmd==L"hm_zoom")
            {
                printf("hm zoom\n");
                //FIXME: USE NEW API
               // this->HMZoomIn( evtTimer->H,evtTimer->M,evtTimer->S,2.0f );
            }else if(evtTimer->cmd==L"hm_zoom_out")
            {
                printf("hm zoom out\n");
                //FIXME: USE NEW API
                //this->HMZoomOutFromCurrent( evtTimer->H );
            }else if(evtTimer->cmd==L"hm_set")
            {
                printf("hm set\n");
                //HMSetCyclistPosition( 1, evtTimer->H );
                //HMSetCyclistPosition( 2, evtTimer->M );
               // HMSetCyclistPosition( 3, evtTimer->S );
            }else if(evtTimer->cmd==L"hm_anim")
            {
                printf("hm anim\n");
                 //this->HMConcentratedDistanceAnimStart(9);
            }else if(evtTimer->cmd==L"hm_anim2")
            {
                printf("hm anim2\n");
                // this->HMConcentratedDistanceAnimStart2(9);
            }else if(evtTimer->cmd==L"hm_set3")
            {
               // this->HMStart( 150 );
            }

       }else{
            auto node = root->GetNode(NodeNameStr);

            if(node==nullptr &&root->GetName()==NodeNameStr)
            {
                Log::A("OK", "root node is node you're looking for ["+ NodeNameStr+"] Applying jedi fix now.");
                node = root;
            }
            if(node==nullptr)
            {
                Log::A("error", "Error OnTimer() node ["+ NodeNameStr+"] not found");
                return;
            }
       
            auto timerplugin = node->GetPlugin("timer");

            //DefaultTimerPlugin * timer = dynamic_cast< DefaultTimerPlugin* >( node->GetPlugin("timer").get() );

            wprintf(L"timer cmd: %s\n",evtTimer->cmd.c_str());
            if(evtTimer->cmd==L"start")
            {
                printf("start\n");
                StartTimerPlugin( timerplugin );
            }
            else if(evtTimer->cmd==L"stop")
            {
                StopTimerPlugin( timerplugin );
            }
            else if(evtTimer->cmd==L"reset")
            {
                //FIXME: PAMIETAC o ResetTimerPlugin :P
                //timer->Reset(0.0f);
            }
            else if(evtTimer->cmd==L"set_time")
            {
                //timer->Stop();
                printf("set time\n");
                TimeType t = ( evtTimer->H * 3600.0f + evtTimer->M * 60.0f + evtTimer->S ) + evtTimer->MS * 0.001f;
                SetTimeTimerPlugin(timerplugin, t);
            }else if(evtTimer->cmd==L"set_time_start")
            {
                //timer->Stop();
                //timer->SetTime(evtTimer->H,evtTimer->M,evtTimer->S,evtTimer->MS);
                //timer->Start();
                 TimeType t = ( evtTimer->H * 3600.0f + evtTimer->M * 60.0f + evtTimer->S ) + evtTimer->MS * 0.001f;
                SetTimeTimerPlugin(timerplugin, t);
                StartTimerPlugin( timerplugin );
            }else if(evtTimer->cmd==L"set_time_stop")
            {
                //timer->Stop();
                //timer->SetTime(evtTimer->H,evtTimer->M,evtTimer->S,evtTimer->MS);
                //timer->Start();
                 TimeType t = ( evtTimer->H * 3600.0f + evtTimer->M * 60.0f + evtTimer->S ) + evtTimer->MS * 0.001f;
                SetTimeTimerPlugin(timerplugin, t);
                StopTimerPlugin( timerplugin );
            }
       }


    }
    
}

// *********************************
//
void RemoteControlInterface::OnSetParam ( bv::IEventPtr evt )
{
    if( evt->GetEventType() == bv::SetParamEvent::m_sEventType)
    {
        bv::SetParamEventPtr evtSetParam = std::static_pointer_cast<bv::SetParamEvent>( evt );
        
        auto root = m_AppLogic->GetBVScene()->GetModelSceneRoot();
        

        //todo: //fixme: wstring -> string
        wstring NodeName = evtSetParam->NodeName;
        string NodeNameStr( NodeName.begin(), NodeName.end() );
        //todo: //fixme: wstring -> string
        wstring TexturePath = evtSetParam->Value;
        string TexturePathStr( TexturePath.begin(), TexturePath.end() );

        auto node = root->GetNode(NodeNameStr);
        if(node==nullptr &&root->GetName()==NodeNameStr)
        {
            Log::A("OK", "root node is node you're looking for ["+ NodeNameStr+"] Applying jedi fix now.");
            node = root;
        }
        if(node==nullptr)
        {
            Log::A("error", "Error OnSetParam() node ["+ NodeNameStr+"] not found");
            return;
        }
        if(evtSetParam->PluginName == L"texture")
        {
            auto plugin = node->GetPlugin( "texture" );

            if(plugin == nullptr)
            {
                 Log::A("error", "Error OnSetParam() node ["+ NodeNameStr+"] , plugin [texture] not found");
                 return;
            }
				ifstream f(BB::AssetManager::GetTexture(TexturePathStr ).c_str());
                if (f.good()) {
                    f.close();
                     bool result = model::LoadTexture( plugin, BB::AssetManager::GetTexture(TexturePathStr ) );
					 {result;}
                } else {
                    f.close();
					 bool result = model::LoadTexture( plugin, BB::AssetManager::GetBlankTexture() );
					 {result;}
                    Log::A("error", "Error OnSetParam() texture ["+  BB::AssetManager::GetTexture(TexturePathStr )+"] not found");
                    return;
                }   

           
            Log::A("OK", "OK OnSetParam() node ["+ NodeNameStr+"] has new image, yo!");

        }else if(evtSetParam->PluginName == L"transform"){
            
            if(evtSetParam->ParamName == L"translation")
            {
                 auto param = node->GetPlugin("transform")->GetParameter("simple_transform");
                 assert( param );

                 wstring value = evtSetParam->Value;
                 
               
				 Log::A("OK", "node ["+ NodeNameStr+"] translation: ("+to_string(evtSetParam->x)+", "+to_string(evtSetParam->y)+", "+to_string(evtSetParam->z)+") time: "+to_string(evtSetParam->time));

                 SetParameterTranslation ( param, 0, (bv::TimeType)evtSetParam->time, glm::vec3( evtSetParam->x,evtSetParam->y,evtSetParam->z));
            }else  if(evtSetParam->ParamName == L"scale")
            {
                 auto param = node->GetPlugin("transform")->GetParameter("simple_transform");
                 assert( param );

                 wstring value = evtSetParam->Value;
				 Log::A("OK", "node ["+ NodeNameStr+"] scale: ("+to_string(evtSetParam->x)+", "+to_string(evtSetParam->y)+", "+to_string(evtSetParam->z)+") time: "+to_string(evtSetParam->time));
                 
                 SetParameterScale ( param, 0, (bv::TimeType)evtSetParam->time, glm::vec3( evtSetParam->x,evtSetParam->y,evtSetParam->z));
            }

		}else if(evtSetParam->PluginName == L"alpha"){
            
            if(evtSetParam->ParamName == L"alpha")
            {
				 auto state = node->GetOverrideState();
				 auto alpha_param = state->GetAlphaParam();

				 wstring value = evtSetParam->Value;
				 
				 float float_value = 1.0f;

				 try{
					float_value =  boost::lexical_cast<float>(value);
				 }catch(boost::bad_lexical_cast&)
				 {
					float_value = 0.0f;
				 }

				 SetParameter( alpha_param, (bv::TimeType)evtSetParam->time, float_value);
            }
        }else if(evtSetParam->PluginName == L"text"){
            
            if(evtSetParam->ParamName == L"text")
            {
                DefaultTextPlugin* txt = dynamic_cast< DefaultTextPlugin* >( node->GetPlugin("text").get() );
                wstring text( evtSetParam->Value.begin(), evtSetParam->Value.end() );

                if( txt != nullptr )
                {
                    model::DefaultTextPlugin::SetText( node->GetPlugin("text"), text );
                }
                else
                {
                    Log::A("error", "Error OnSetParam() node ["+ NodeNameStr+"] , plugin [text] not found");
                }
            }

        }else if(evtSetParam->PluginName == L"visibility"){
            
            if(evtSetParam->ParamName == L"visible")
            {
                if(evtSetParam->Value == L"true")
                {
                    node->SetVisible(true);
                }else{
                    node->SetVisible(false);
                }
            }

        
        } else {
			string plugin_name_cast= string(evtSetParam->PluginName.begin(), evtSetParam->PluginName.end());
			string param_name_cast= string(evtSetParam->ParamName.begin(), evtSetParam->ParamName.end());
            auto plugin = node->GetPlugin( plugin_name_cast ).get();
			if(plugin==nullptr)
			{
				Log::A("error", "Error OnSetParam() plugin ["+ plugin_name_cast+"] not found");

			}else{
				auto param = plugin->GetParameter( param_name_cast );
				if(param==nullptr)
				{
					Log::A("error", "Error OnSetParam() plugin ["+ plugin_name_cast+"] param ["+param_name_cast+"] not found");

				}else{

					 wstring value = evtSetParam->Value;
				 
					 float float_value = 1.0f;

					 try{
						float_value =  boost::lexical_cast<float>(value);
					 }catch(boost::bad_lexical_cast&)
					 {
						float_value = 0.0f;
					 }

					 SetParameter( param, (bv::TimeType)evtSetParam->time, float_value);
				}

			}

        }   
    }
}


// *********************************
//
void RemoteControlInterface::OnWidgetCmd ( bv::IEventPtr evt )
{
	if( evt->GetEventType() == bv::WidgetCmd::m_sEventType)
    {
		bv::WidgetCmdPtr evtWidget = std::static_pointer_cast<bv::WidgetCmd>( evt );
        
        auto root = m_AppLogic->GetBVScene()->GetModelSceneRoot();
        

        //todo: //fixme: wstring -> string
        wstring NodeName =  evtWidget->NodeName;
        string NodeNameStr( NodeName.begin(), NodeName.end() );
        //todo: //fixme: wstring -> string
        wstring param = evtWidget->Param;
        //string TexturePathStr( TexturePath.begin(), TexturePath.end() );
		

        auto node = root->GetNode(NodeNameStr);
        if(node==nullptr &&root->GetName()==NodeNameStr)
        {
            Log::A("OK", "root node is node you're looking for ["+ NodeNameStr+"] Applying jedi fix now.");
            node = root;
        }
        if(node==nullptr)
        {
            Log::A("error", "Error OnSetParam() node ["+ NodeNameStr+"] not found");
            return;
        }
		
		BasicNodePtr nod      = std::static_pointer_cast< bv::model::BasicNode >(node);
			
		
        if(nod == nullptr)
        {
                Log::A("error", "Error OnWidgetCmd () node ["+ NodeNameStr+"] not found");
                return;
        }

		INodeLogicPtr logic = nod->GetLogic();
		if(logic==nullptr)
		{
			 Log::A("error", "Error OnWidgetCmd () node ["+ NodeNameStr+"] , logic [] not found");
                return;

		}
		
		INodeLogic* logic__ptr = logic.get();


        if(evtWidget->WidgetName == L"crawl")
        {
			bv::widgets::Crawler* crawler =  (bv::widgets::Crawler*)logic__ptr;
			if(evtWidget->Action==L"stop")
			{
				Log::A("OK","crawl stop...");
				crawler->Stop();
			}
			else if(evtWidget->Action==L"start")
			{
				Log::A("OK","crawl start...");
				crawler->Start();
			}else if(evtWidget->Action==L"add_text")
			{
				Log::A(L"OK",L"crawl add text..."+ evtWidget->Param);
				crawler->AddMessage(evtWidget->Param);
			}else if(evtWidget->Action==L"reset")
			{
				Log::A(L"OK",L"crawl reset...");
				crawler->Reset();
			}
			else if(evtWidget->Action==L"clear")
			{
				Log::A(L"OK",L"crawl clear...");
				crawler->Clear();
			}else if(evtWidget->Action==L"set_speed")
			{
				Log::A(L"OK",L"crawl set speed.."+ evtWidget->Param);
				float speed = 0.5;
				string s_speed( evtWidget->Param.begin(), evtWidget->Param.end() );
				speed = (float)atof(s_speed.c_str());
				crawler->SetSpeed(speed);

			}

		}else if(evtWidget->WidgetName == L"counter")
        {
			bv::widgets::WidgetCounter * counter =  (bv::widgets::WidgetCounter*)logic__ptr;

			string param_name_cast= string(evtWidget->Param.begin(), evtWidget->Param.end());

			auto param = counter->GetValueParam();
			if(param==nullptr)
			{
				Log::A("error", "Error OnSetParam() plugin [counter] param ["+param_name_cast+"] not found");

			}else{

					wstring value = evtWidget->Value;
				 
					float float_value = 1.0f;

					try{
					float_value =  boost::lexical_cast<float>(value);
					}catch(boost::bad_lexical_cast&)
					{
					float_value = 0.0f;
					}

					SetParameter( param, (bv::TimeType)evtWidget->Time, float_value);
			}
		}
    }
}


// *********************************
//
void            RemoteControlInterface::OnNodeAppearing   ( IEventPtr evt )
{
    
}

// *********************************
//
void            RemoteControlInterface::OnNodeLeaving   ( IEventPtr evt )
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


void            RemoteControlInterface::OnNoMoreNodes   ( IEventPtr evt )
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

// *********************************
//
void            RemoteControlInterface::HMSetCyclistPosition    ( int cyclistPos, float km )
{
    assert( cyclistPos > 0 );
    assert( size_t(cyclistPos) <= m_cyclistPos.size() );

    if(cyclistPos==1)
    {
		auto root = m_AppLogic->GetBVScene()->GetModelSceneRoot()->GetChild(".");
        model::SetParameter( root->GetPlugin( "height map" )->GetParameter( "curDistanceInMeters" ), TimeType( 0.f ), km * 1000.0f );
    }

    m_cyclistPos[ cyclistPos - 1 ] = km;
}

// *********************************
//
void            RemoteControlInterface::UpdateCyclistPosition    ( int cyclistPos, float km )
{
    assert( cyclistPos > 0 );
    auto root = m_AppLogic->GetBVScene()->GetModelSceneRoot()->GetChild(".");
    auto hmplugin = root->GetPlugin( "height map" );

    model::DefaultHeightMapPlugin *  tp = static_cast< model::DefaultHeightMapPlugin * > ( hmplugin.get() );

    auto cyclistNode = root->GetChild( "cyclistNode" + to_string( cyclistPos - 1 ) );
    if( cyclistNode )
    {
        //if( cyclistPos == 1 )
///model::SetParameter( root->GetPlugin( "height map" )->GetParameter( "curDistanceInMeters" ), TimeType( 0.f ), km * 1000.f );

        auto transformPlugin = cyclistNode->GetPlugin( "transform" );
        auto param = transformPlugin->GetParameter( "simple_transform" );

        auto pos = tp->QueryEdgePosition( km * 1000.f );
        float x = 1920.f / 1080.f * ( 2.f * pos.x - 1.f );
        float y = ( 2.f * pos.y - 1.f );

        SetParameterTranslation( param, 0, 0.0f, glm::vec3( x, y, 0.f ) );

    }
}


// *********************************
//
void            RemoteControlInterface::HMShow                  ()
{
    auto root = m_AppLogic->GetBVScene()->GetModelSceneRoot()->GetChild(".");
    auto hmplugin = root->GetPlugin( "height map" );

    auto timeline = m_AppLogic->GetTimeLineManager()->GetTimeline( "hm_timeline" );
    if( timeline == nullptr )
    {
        Log::A( L"ERROR", L"timeline [ height map ] does not exist :(" );
        return;
    }

    auto tr = root->GetPlugin( "transform" )->GetParameter( "simple_transform" );
    //auto time1 = timeline->GetKeyFrameEvent( "stop0" )->GetEventTime();

    auto time2 = timeline->GetKeyFrameEvent( "stop5" )->GetEventTime();

    
    
    SetParameterTranslation( tr, 0, time2, glm::vec3(0,0,0) );

    timeline->Play();
}

// *********************************
//
void            RemoteControlInterface::HMReset                  ()
{
   
    auto timeline = m_AppLogic->GetTimeLineManager()->GetTimeline( "hm_timeline" );
    timeline->SetTimeAndStop(0.0f);
}

// *********************************
//

// *********************************
//
void            RemoteControlInterface::HMStart                 ( float km )
{
    auto root = m_AppLogic->GetBVScene()->GetModelSceneRoot()->GetChild(".");
    auto hmplugin = root->GetPlugin( "height map" );

    auto timeline = m_AppLogic->GetTimeLineManager()->GetTimeline( "hm_timeline" );
    if( timeline == nullptr )
    {
        Log::A( L"ERROR", L"timeline [ height map ] does not exist :(" );
        return;
    }

    auto time1 = timeline->GetKeyFrameEvent( "stop1" )->GetEventTime();
    auto time2 = timeline->GetKeyFrameEvent( "stop2" )->GetEventTime();


    model::SetParameter( root->GetPlugin( "height map" )->GetParameter( "curDistanceInMeters" ), TimeType( 0.f ), km * 1000.0f ); // 80km // 220863.f * 0.5f //pionowa kreska
    
    model::SetParameter( hmplugin->GetParameter( "coveredDistShowFactor" ), time1, 0.0f );
    model::SetParameter( hmplugin->GetParameter( "coveredDistShowFactor" ), time2, 1.f );

    timeline->Play();
}

// *********************************
//FIXME: uses hardcoded totalDistance
void        RemoteControlInterface::HMZoomIn                ( float km, float leftKM, float rightKM, float yScale )
{
    auto root = m_AppLogic->GetBVScene()->GetModelSceneRoot()->GetChild(".");
    auto hmplugin = root->GetPlugin( "height map" );

    auto timeline = m_AppLogic->GetTimeLineManager()->GetTimeline( "hm_timeline" );
    if( timeline == nullptr )
    {
        Log::A( L"ERROR", L"timeline [ height map ] does not exist :(" );
        return;
    }

    float visibleZoomedKm = leftKM + rightKM; //powinno sie nazywac totalzoomedkm

    auto time2 = timeline->GetKeyFrameEvent( "stop2" )->GetEventTime();
    auto time3 = timeline->GetKeyFrameEvent( "stop3" )->GetEventTime();
    auto time5 = timeline->GetKeyFrameEvent( "stop5" )->GetEventTime();

    //FIXME: totalDistance in meters- should be set globally or read from scene parameters
    float totalDistance = 173707.f;
    
    float xscl = totalDistance / (visibleZoomedKm * 500.f);
    float ixscl = 1.0f / xscl;
    
    float u1 = ( km * 1000.f ) / totalDistance;
    float u0 = leftKM / ( leftKM + rightKM );

    float xSnapNormalized = ( u1 - u0 * ixscl ) / ( 1.0f - ixscl );
    float xSnapInMeters = xSnapNormalized * totalDistance;

    //model::SetParameter( root->GetPlugin( "height map" )->GetParameter( "curDistanceInMeters" ), TimeType( 0.f ), km * 1000.f );
    model::SetParameter( root->GetPlugin( "height map" )->GetParameter( "snapToMeter" ), TimeType( 0.f ), xSnapInMeters );
    model::SetParameter( root->GetPlugin( "height map" )->GetParameter( "snapToMeter" ), TimeType( time5 ), xSnapInMeters );
    
    model::SetParameter( root->GetPlugin( "height map" )->GetParameter( "scale" ), TimeType( time2 ), glm::vec2( 1.f, 1.f ) ); //yScale == 2.0f w starej wersji
    model::SetParameter( root->GetPlugin( "height map" )->GetParameter( "scale" ), TimeType( time3 ), glm::vec2( xscl, yScale ) ); // totalLen / scale.x == liczba widocznych kilometrow 216227 / 2
    model::SetParameter( root->GetPlugin( "height map" )->GetParameter( "scale" ), TimeType( time5 ), glm::vec2( xscl, yScale ) ); // totalLen / scale.x == liczba widocznych kilometrow 216227 / 2

    m_concentrateDistMarks[ 0 ] = km - 20;
    m_concentrateDistMarks[ 1 ] = km - 15;
    m_concentrateDistMarks[ 2 ] = km - 10;
    m_concentrateDistMarks[ 3 ] = km - 5;
    m_concentrateDistMarks[ 4 ] = km;
    m_concentrateDistMarks[ 5 ] = km + 5;
    m_concentrateDistMarks[ 6 ] = km + 10;
    m_concentrateDistMarks[ 7 ] = km + 15;
    m_concentrateDistMarks[ 8 ] = km + 20;

    timeline->Play();
}

// *********************************
//FIXME: uses hardcoded totalDistance
void            RemoteControlInterface::HMZoomOutFromCurrent    (float scale)
{
    auto root = m_AppLogic->GetBVScene()->GetModelSceneRoot()->GetChild(".");
     auto hmplugin = root->GetPlugin( "height map" );

    auto timeline = m_AppLogic->GetTimeLineManager()->GetTimeline( "hm_timeline" );
    if( timeline == nullptr )
    {
        Log::A( L"ERROR", L"timeline [ height map ] does not exist :(" );
        return;
    }

    
    auto tr = root->GetPlugin( "height map" )->GetParameter( "simple_transform" );
    auto time3 = timeline->GetKeyFrameEvent( "stop6" )->GetEventTime();

    
    //model::SetParameter( root->GetPlugin( "height map" )->GetParameter( "snapToMeter" ), TimeType( time3 ), 174000.f );

    //SetParameterTranslation( tr, 0, time3, glm::vec3(-1,0,0) );


   
    

    model::SetParameter( root->GetPlugin( "height map" )->GetParameter( "scale" ), TimeType( time3 ), glm::vec2( scale, 1.0f ) ); // totalLen / scale.x == liczba widocznych kilometrow 216227 / 2

    timeline->Play();
}

// *********************************
//FIXME: uses hardcoded totalDistance
void        RemoteControlInterface::HMZoomInFake                ( float km, float leftKM, float rightKM, float yScale )
{
    auto root = m_AppLogic->GetBVScene()->GetModelSceneRoot()->GetChild(".");
    auto hmplugin = root->GetPlugin( "height map" );

    auto timeline = m_AppLogic->GetTimeLineManager()->GetTimeline( "hm_timeline" );
    if( timeline == nullptr )
    {
        Log::A( L"ERROR", L"timeline [ height map ] does not exist :(" );
        return;
    }

    float visibleZoomedKm = leftKM + rightKM; //powinno sie nazywac totalzoomedkm

    auto time2 = timeline->GetKeyFrameEvent( "stop4" )->GetEventTime();
    auto time3 = timeline->GetKeyFrameEvent( "stop5" )->GetEventTime();

    //FIXME: totalDistance in meters- should be set globally or read from scene parameters
    float totalDistance = 173707.f;
    
    float xscl = totalDistance / (visibleZoomedKm * 500.f);
    float ixscl = 1.0f / xscl;
    
    float u1 = ( km * 1000.f ) / totalDistance;
    float u0 = leftKM / ( leftKM + rightKM );

    float xSnapNormalized = ( u1 - u0 * ixscl ) / ( 1.0f - ixscl );
    float xSnapInMeters = xSnapNormalized * totalDistance;

    //model::SetParameter( root->GetPlugin( "height map" )->GetParameter( "curDistanceInMeters" ), TimeType( 0.f ), km * 1000.f );
    model::SetParameter( root->GetPlugin( "height map" )->GetParameter( "snapToMeter" ), TimeType( 0.f ), xSnapInMeters );
    
    model::SetParameter( root->GetPlugin( "height map" )->GetParameter( "scale" ), TimeType( time2 ), glm::vec2( 1.f, 1.f ) ); //yScale == 2.0f w starej wersji
    model::SetParameter( root->GetPlugin( "height map" )->GetParameter( "scale" ), TimeType( time3 ), glm::vec2( xscl, yScale ) ); // totalLen / scale.x == liczba widocznych kilometrow 216227 / 2

    m_concentrateDistMarks[ 0 ] = km - 20;
    m_concentrateDistMarks[ 1 ] = km - 15;
    m_concentrateDistMarks[ 2 ] = km - 10;
    m_concentrateDistMarks[ 3 ] = km - 5;
    m_concentrateDistMarks[ 4 ] = km;
    m_concentrateDistMarks[ 5 ] = km + 5;
    m_concentrateDistMarks[ 6 ] = km + 10;
    m_concentrateDistMarks[ 7 ] = km + 15;
    m_concentrateDistMarks[ 8 ] = km + 20;

    timeline->Play();
}

// *********************************
//
void            RemoteControlInterface::HMConcentratedDistanceAnimStart( int num )
{
	{num;}
    auto root = m_AppLogic->GetBVScene()->GetModelSceneRoot()->GetChild(".");
    auto hmplugin = root->GetPlugin( "height map" );

    model::DefaultHeightMapPlugin *  tp = static_cast< model::DefaultHeightMapPlugin * > ( hmplugin.get() );

	{tp;}

    auto timeline = m_AppLogic->GetTimeLineManager()->GetTimeline( "hm_timeline" );
    if( timeline == nullptr )
    {
        Log::A( L"ERROR", L"timeline [ height map ] does not exist :(" );
        return;
    }
       
    auto time3 = timeline->GetKeyFrameEvent( "stop3" )->GetEventTime();
    auto time4 = timeline->GetKeyFrameEvent( "stop4" )->GetEventTime();

    {
        auto markTextConcentratedRoot = root->GetChild( "markTextConcentratedRoot" );

        auto transformPlugin = markTextConcentratedRoot->GetPlugin( "transform" );
        auto param = transformPlugin->GetParameter( "simple_transform" );

        model::SetParameterTranslation( param, 0, time3, glm::vec3( 0.f, -1.1f, 0.f ) );

        model::SetParameterTranslation( param, 0, time4, glm::vec3( 0.f, -0.87f, 0.f ) );
    }

    {
        auto markTextRoot = root->GetChild( "markTextRoot" );

        auto transformPlugin = markTextRoot->GetPlugin( "transform" );
        auto param = transformPlugin->GetParameter( "simple_transform" );

        model::SetParameterTranslation( param, 0, time4, glm::vec3( 0.f, -1.1f, 0.f ) );

        model::SetParameterTranslation( param, 0, time3, glm::vec3( 0.f, -0.87f, 0.f ) );
    }

    timeline->Play();
}
// *********************************
//
void            RemoteControlInterface::HMConcentratedDistanceAnimStart2( int num )
{
	{num;}

    auto root = m_AppLogic->GetBVScene()->GetModelSceneRoot()->GetChild(".");
    auto hmplugin = root->GetPlugin( "height map" );

    model::DefaultHeightMapPlugin *  tp = static_cast< model::DefaultHeightMapPlugin * > ( hmplugin.get() );
	{tp;}
    auto timeline = m_AppLogic->GetTimeLineManager()->GetTimeline( "hm_timeline" );
    if( timeline == nullptr )
    {
        Log::A( L"ERROR", L"timeline [ height map ] does not exist :(" );
        return;
    }
       
    auto time3 = timeline->GetKeyFrameEvent( "stop4" )->GetEventTime();
    auto time4 = timeline->GetKeyFrameEvent( "stop5" )->GetEventTime();

    {
        auto markTextConcentratedRoot = root->GetChild( "markTextConcentratedRoot" );

        auto transformPlugin = markTextConcentratedRoot->GetPlugin( "transform" );
        auto param = transformPlugin->GetParameter( "simple_transform" );

        model::SetParameterTranslation( param, 0, time4, glm::vec3( 0.f, -1.1f, 0.f ) );

        model::SetParameterTranslation( param, 0, time3, glm::vec3( 0.f, -0.87f, 0.f ) );
    }

    {
        auto markTextRoot = root->GetChild( "markTextRoot" );

        auto transformPlugin = markTextRoot->GetPlugin( "transform" );
        auto param = transformPlugin->GetParameter( "simple_transform" );

        model::SetParameterTranslation( param, 0, time3, glm::vec3( 0.f, -1.1f, 0.f ) );

        model::SetParameterTranslation( param, 0, time4, glm::vec3( 0.f, -0.87f, 0.f ) );
    }

    timeline->Play();
}



// *********************************
//
void    RemoteControlInterface::UpdateHM        ()
{
	bool hm_enable=false;
    if(hm_enable){
                
        auto root = m_AppLogic->GetBVScene()->GetModelSceneRoot()->GetChild(".");
        auto hmplugin = root->GetPlugin( "height map" );
        //auto rctn = root->GetChild( "rct" );
        //auto rect = rctn->GetPlugin( "transform" );
        //auto param = rect->GetParameter( "simple_transform" );
                
        model::DefaultHeightMapPlugin *  tp = static_cast< model::DefaultHeightMapPlugin * > ( hmplugin.get() );

        //kreska
        //auto pos = tp->QueryEdgePosition( 73000.f );
        //float x = 1920.f / 1080.f * ( 2.f * pos.x - 1.f );
        ////SetParameterTranslation( param, 0, 0.0f, glm::vec3( x, 2.f * 98.f / 1080.f, 0.f ) );
        //SetParameterTranslation( param, 0, 0.0f, glm::vec3( x, -( pos.y + 98.0f / 1080.f ) * 2.f, 0.f ) );
        //SetParameterScale( param, 0, 0.0f, glm::vec3( 1.f, pos.y, 1.f ) );

        //auto pos = tp->QueryEdgePosition( 73000.f );
        //float x = 1920.f / 1080.f * ( 2.f * pos.x - 1.f );
        //float y = ( 2.f * pos.y - 1.f );
        //SetParameterTranslation( param, 0, 0.0f, glm::vec3( x, y, 0.f ) );


        auto startFinishRoot = root->GetChild( "startFinishRoot" );

        if( startFinishRoot )
        {
            {
                auto startTexture = startFinishRoot->GetChild( "startTexture" );

                auto transformPlugin = startTexture->GetPlugin( "transform" );
                auto param = transformPlugin->GetParameter( "simple_transform" );
                auto pos = tp->QueryEdgePosition( 6000.f );
                float x = 1920.f / 1080.f * ( 2.f * pos.x - 1.f );
                float y = ( 2.f * pos.y - 1.f );

                SetParameterTranslation( param, 0, 0.0f, glm::vec3( x, y + 180.f / 1080.f , 0.f ) );
            }

            {
                auto finishTexture = startFinishRoot->GetChild( "finishTexture" );

                auto transformPlugin = finishTexture->GetPlugin( "transform" );
                auto param = transformPlugin->GetParameter( "simple_transform" );
                auto pos = tp->QueryEdgePosition( 170707.f );
                float x = 1920.f / 1080.f * ( 2.f * pos.x - 1.f );
                float y = ( 2.f * pos.y - 1.f );

                SetParameterTranslation( param, 0, 0.0f, glm::vec3( x, y + 180.f / 1080.f, 0.f ) );
            }



            //    auto distPoint = markTextRoot->GetChild( "markText" + to_string( i ) );
            //    if( distPoint )
            //    {
            //        auto transformPlugin = distPoint->GetPlugin( "transform" );
            //        auto param = transformPlugin->GetParameter( "simple_transform" );

            //        auto textPlugin = std::static_pointer_cast< model::DefaultTextPlugin >( distPoint->GetPlugin( "text" ) );

            //        auto text = textPlugin->GetText();
            //        auto distansInKm = _wtoi( text.c_str() ) * 1000;

            //        auto pos = tp->QueryEdgePosition( distansInKm );
            //        float x = 1920.f / 1080.f * ( 2.f * pos.x - 1.f );
            //        float y = ( 2.f * pos.y - 1.f );

            //        SetParameterTranslation( param, 0, 0.0f, glm::vec3( x, 0.f, 0.f ) );
            //    }
            //    else
            //    {
            //        break;
            //    }
            //}
        }


        auto markTextRoot = root->GetChild( "markTextRoot" );

        if( markTextRoot )
        {
            for( int i = 0; i < 100; ++i )
            {
                auto distPoint = markTextRoot->GetChild( "markText" + to_string( i ) );
                if( distPoint )
                {
                    auto transformPlugin = distPoint->GetPlugin( "transform" );
                    auto param = transformPlugin->GetParameter( "simple_transform" );

                    auto textPlugin = std::static_pointer_cast< model::DefaultTextPlugin >( distPoint->GetPlugin( "text" ) );

                    auto text = textPlugin->GetText();
                    auto distansInKm = _wtoi( text.c_str() ) * 1000;

                    auto pos = tp->QueryEdgePosition( (float)distansInKm );
                    float x = 1920.f / 1080.f * ( 2.f * pos.x - 1.f );
                    //float y = ( 2.f * pos.y - 1.f );

                    SetParameterTranslation( param, 0, 0.0f, glm::vec3( x, 0.f, 0.f ) );
                }
                else
                {
                    break;
                }
            }
        }


        auto markTextConcentratedRoot = root->GetChild( "markTextConcentratedRoot" );

        if( markTextConcentratedRoot )
        {
            for( int i = 0; i < 9; ++i )
            {
                auto markConcentratedNode = markTextConcentratedRoot->GetChild( "markTextConcentrated" + to_string( i ) );
     
                float p = m_concentrateDistMarks[ i ];

                model::DefaultTextPlugin::SetText( markConcentratedNode->GetPlugin( "text" ), to_wstring( int( p ) ) + L" km" );

                auto pos = tp->QueryEdgePosition( p * 1000.f );
                float x = 1920.f / 1080.f * ( 2.f * pos.x - 1.f );
                //float y = ( 2.f * pos.y - 1.f );

                auto transformPlugin = markConcentratedNode->GetPlugin( "transform" );
                auto param = transformPlugin->GetParameter( "simple_transform" );

                SetParameterTranslation( param, 0, 0.0f, glm::vec3( x, 0.f, 0.f ) );

            }
        }

        for( int i = 0; i < 3; ++i )
        {
            UpdateCyclistPosition( i + 1, m_cyclistPos[ i ] );
        }

    }
}

}


