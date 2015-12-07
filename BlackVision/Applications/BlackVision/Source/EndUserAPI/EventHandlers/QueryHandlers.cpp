#include "QueryHandlers.h"

#include "../../BVAppLogic.h"
#include "../../UseLogger.h"
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
Json::Value SerializeSceneModel( model::SceneModelPtr sceneModel )
{
    JsonSerializeObject ser;
    sceneModel->Serialize( ser );

    return ser.GetJson();
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
    for( int i = 0; i < NumChildren; i++ )
    {

		model::IModelNodePtr ptr   = node->GetChild(i);
        model::IModelNodePtr ptr2 = ptr;
        model::BasicNodePtr nod = std::static_pointer_cast< model::BasicNode >( ptr2 );
        ReqPrint( nod, level + 1 );
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
    if( evt->GetEventType() == bv::InfoEvent::Type() )
    {
        bv::InfoEventPtr infoEvent = std::static_pointer_cast<bv::InfoEvent>( evt );

        InfoEvent::Command command = infoEvent->InfoRequest;
        std::string& nodeName = infoEvent->NodeName;

        wstring responseMessage;

        if( command == InfoEvent::Command::TreeStructure )
        {
            //Log::A("OK","Tree structure:");
            ReqPrint( m_appLogic->GetBVProject()->GetModelSceneRoot(), 1 );

            Json::Value root;
            root[ "command" ] = "scene_tree";

            root[ "scenes" ] = Json::arrayValue;

            for( auto s : m_appLogic->GetBVProject()->GetScenes() )
            {
                Json::Value val;
                root[ "scenes" ].append( SerializeSceneModel( s ) );
            }


            auto ret = root.toStyledString();
            responseMessage = toWString( root.toStyledString() );
        }
        else if( command == InfoEvent::Command::Performance )
        {
            //Log::A("SENDING","Performance:");
                        
            auto& frameStats = m_appLogic->FrameStats();
            auto& sections = frameStats.RegisteredSections();

            PerformanceMonitor::Calculate( m_appLogic->GetStatsCalculator() );

            Json::Value root;
            root["command"] = "performance";
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
            
            responseMessage = toWString( root.toStyledString() );
        }
        else if( command == InfoEvent::Command::Timelines )
        {
            Json::Value ret;
            ret[ "command" ] = "timelines";
            ret[ "scenes" ] = Json::arrayValue;

            for( auto s : m_appLogic->GetBVProject()->GetScenes() )
            {
                Json::Value val;
				val[ "name" ] = s->GetName();
                JsonSerializeObject ser;
				//FIXME: only timelines used in scene should be serialized
				TimelineManager::GetInstance()->Serialize(ser);
                val[ "timelines" ] = ser.GetJson();
                ret[ "scenes" ].append( val );
            }

            responseMessage = toWString( ret.toStyledString() );
        }
        else if( command == InfoEvent::Command::NodeInfo )
        {
		    auto root = m_appLogic->GetBVProject()->GetModelSceneRoot();
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
            responseMessage = wstring( resStr.begin(), resStr.end() );
        }
        else if( command == InfoEvent::Command::Videocards )
        {
            Json::Value val;
            val[ "cmd" ]        = "videocards";
            val[ "visible" ]    = " no diggy diggy ";

            string response = val.toStyledString();
           
            //Log::A( "SENDING", S );
            responseMessage = wstring( response.begin(),response.end() );
        }
        
        ResponseEventPtr msg = std::make_shared<ResponseEvent>();
        msg->Response = responseMessage;
        msg->SocketID = infoEvent->SocketID;
        GetDefaultEventManager().QueueResponse( msg );
    }
}



} //bv
