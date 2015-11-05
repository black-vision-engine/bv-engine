#include "QueryHandlers.h"

#include "../BVAppLogic.h"
#include "../UseLogger.h"
#include "Tools/IncludeJSON.h"
#include "PerformanceMonitor.h"
#include "Engine/Models/BasicNode.h"
#include "Engine/Events/EventHelpers.h"             // wstring to string conversions and vice versa


namespace bv
{

namespace 
{

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

// ***********************
//
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

// ***********************
//
void ReqPrint( model::BasicNodePtr node, int level )
{
    string temp="-";
    for( int i = 1; i < level; i++ )
    {
        temp+="-";
    }

    //Log::A("OK",temp+node->GetName());
    
    int NumChildren = (node)->GetNumChildren();
    for(int i=0;i<NumChildren;i++)
    {

		model::IModelNodePtr ptr   = node->GetChild(i);
        model::IModelNodePtr ptr2 = ptr;
        model::BasicNodePtr nod = std::static_pointer_cast< model::BasicNode >( ptr2 );
        ReqPrint(nod,level+1);
    }

}

} //anonymous

// ***********************
//
QueryHandlers::QueryHandlers( BVAppLogic* appLogic )
    :   m_appLogic( appLogic )
{}

QueryHandlers::~QueryHandlers()
{}


// ***********************
//
void QueryHandlers::Info        ( bv::IEventPtr evt )
{
    if( evt->GetEventType() == bv::NewInfoEvent::Type() )
    {
        bv::NewInfoEventPtr infoEvent = std::static_pointer_cast<bv::NewInfoEvent>( evt );

        NewInfoEvent::Command command = infoEvent->InfoRequest;
        std::string& nodeName = infoEvent->NodeName;

        if( command == NewInfoEvent::Command::TreeStructure )
        {
            //Log::A("OK","Tree structure:");
			
			
            ReqPrint( m_appLogic->GetBVScene()->GetModelSceneRoot(), 1 );
            std::string S;
			S="{\"cmd\":\"scene_tree\",\"tree\": "+SerializeNode(m_appLogic->GetBVScene()->GetModelSceneRoot())+"}";
            //Log::A("OK",S);
            wstring responseMessage = wstring(S.begin(),S.end());

            ResponseEventPtr msg = std::make_shared<ResponseEvent>();
            msg->Response = responseMessage;
            GetDefaultEventManager().QueueResponse( msg );

        }
        else if( command == NewInfoEvent::Command::Performance )
        {
            //Log::A("SENDING","Performance:");
                        
            auto& frameStats = m_appLogic->FrameStats();
            auto& sections = frameStats.RegisteredSections();

            PerformanceMonitor::Calculate( m_appLogic->GetStatsCalculator() );

            Json::Value root;
            root["cmd"] = "performance";
            root["fps"] = PerformanceMonitor::Stats.fps;
            root["fps_avg"] = PerformanceMonitor::Stats.fps_avg;
            root["ram"] = PerformanceMonitor::Stats.ram;
            root["vram"] = PerformanceMonitor::Stats.vram;
            root["cpu"] = PerformanceMonitor::Stats.cpu;

            for( auto name : sections )
            {
                root[ name ]["average"] = frameStats.ExpectedValue( name );
                root[ name ]["minVal"] = frameStats.MinVal( name );
                root[ name ]["maxVal"] = frameStats.MaxVal( name );
                root[ name ]["variance"] = frameStats.Variance( name );
            }
            
            //Log::A("OK",S);
            wstring responseMessage = toWString( root.toStyledString() );

            ResponseEventPtr responseEvent = std::make_shared<ResponseEvent>();
            responseEvent->Response = responseMessage;
            GetDefaultEventManager().QueueResponse( responseEvent );

        }
        else if( command == NewInfoEvent::Command::Timelines )
        {
            
            //Log::A("SENDING","Timelines info...:");

			model::TimelineManager * TM = m_appLogic->GetTimelineManager();

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
   
            PerformanceMonitor::Calculate( m_appLogic->GetStatsCalculator() );
            string S = "{\"cmd\":\"timelines\", \"timelines\":"+s_timelines+" }";
           
            //Log::A("SENDING",S);
            wstring responseMessage = wstring(S.begin(),S.end());

            ResponseEventPtr msg = std::make_shared<ResponseEvent>();
            msg->Response = responseMessage;
            GetDefaultEventManager().QueueResponse( msg );
        }
        else if( command == NewInfoEvent::Command::NodeInfo )
        {
		    auto root = m_appLogic->GetBVScene()->GetModelSceneRoot();
			auto node = root->GetNode( nodeName );

			if( node == nullptr && root->GetName() == nodeName )
			{
				//Log::A( "OK", "root node is node you're looking for [" + nodeName + "] Applying jedi fix now." );
				node = root;
			}

			if( node==nullptr )
			{
				//Log::A( "error", "Error NodeInfo() node [" + nodeName + "] not found" );
				return;
			}

            JsonSerializeObject ser;
            std::static_pointer_cast< model::BasicNode >( node )->Serialize( ser );

            Json::Value res;
            res[ "cmd" ] = "node_info";
            res[ "node" ] = ser.GetJson();
            res[ "node" ]["node"].removeMember( "nodes" );

            auto resStr = res.toStyledString();

            //Log::A( "SENDING", resStr );
            wstring responseMessage = wstring( resStr.begin(), resStr.end() );

            ResponseEventPtr msg = std::make_shared<ResponseEvent>();
            msg->Response = responseMessage;
            GetDefaultEventManager().QueueResponse( msg );
        }
        else if( command == NewInfoEvent::Command::Videocards )
        {
            Json::Value val;
            val[ "cmd" ]        = "videocards";
            val[ "visible" ]    = " no diggy diggy ";

            string S = val.toStyledString();
           
            //Log::A( "SENDING", S );
            wstring responseMessage = wstring( S.begin(),S.end() );

            ResponseEventPtr msg = std::make_shared<ResponseEvent>();
            msg->Response = responseMessage;
            GetDefaultEventManager().QueueResponse( msg );
        }
    }
}



} //bv
