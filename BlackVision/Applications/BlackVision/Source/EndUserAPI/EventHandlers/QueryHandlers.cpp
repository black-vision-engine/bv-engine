#include "QueryHandlers.h"

#include "../../BVAppLogic.h"
#include "../../UseLoggerBVAppModule.h"
#include "Tools/IncludeJSON.h"
#include "PerformanceMonitor.h"
#include "Engine/Models/BasicNode.h"
#include "Engine/Events/EventHelpers.h"             // wstring to string conversions and vice versa
#include "EventHandlerHelpers.h"
#include "Assets/AssetDescsWithUIDs.h"
#include "Serialization/Json/JsonDeserializeObject.h"
#include "ProjectManager.h"

namespace bv
{

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

        InfoEvent::Command command = infoEvent->InfoCommand;
        std::string& nodeName = infoEvent->NodeName;
        std::string& request = infoEvent->Request;

        wstring responseMessage;

        if( command == InfoEvent::Command::TreeStructure )
            responseMessage = toWString( TreeStructureInfo( request ) );
        else if( command == InfoEvent::Command::ListAssets )
            responseMessage = toWString( ListAssets( request ) );
        else if( command == InfoEvent::Command::ListAssetsPaths )
            responseMessage = ListAssetsPaths( request );
        else if( command == InfoEvent::Command::ListCategoriesNames )
            responseMessage = ListCategoriesNames( request );
        else if( command == InfoEvent::Command::ListProjectNames )
            responseMessage = ListProjectNames( request );
        else if( command == InfoEvent::Command::ListProjects )
            responseMessage = ListProjects( request );
        else if( command == InfoEvent::Command::ListScenes )
            responseMessage = ListScenes( request );
        else if( command == InfoEvent::Command::Performance )
            responseMessage = toWString( PerformanceInfo( request ) );
        else if( command == InfoEvent::Command::Timelines )
            responseMessage = toWString( GetTimeLinesInfo( request ) );
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
            responseMessage = toWString( VideoCardsInfo( request ) );
        
        ResponseEventPtr msg = std::make_shared<ResponseEvent>();
        msg->Response = responseMessage;
        msg->SocketID = infoEvent->SocketID;
        GetDefaultEventManager().QueueResponse( msg );
    }
}


// ***********************
//
std::string QueryHandlers::ListAssets  ( const std::string& request )
{
    JsonDeserializeObject deser;
    JsonSerializeObject ser;
    deser.Load( request );

    std::string category = deser.GetAttribute( "CategoryName" );

    ser.EnterArray( "assets" );
    for( auto scene : m_appLogic->GetBVProject()->GetScenes() )
    {
        auto sceneRoot = scene->GetRootNode();

        AssetDescsWithUIDs assets;
        GetAssetsWithUIDs( assets, sceneRoot, true );
        AssetDescsWithUIDs::SetInstance( assets );

        auto descriptors = assets.GetAssetsDescs();

        for( auto& descriptor : descriptors )
        {
            if( descriptor->GetUID() == category )
                descriptor->Serialize( ser );
        }
    }
    ser.ExitChild();

    return ser.GetString();
}

// ***********************
//
std::string QueryHandlers::VideoCardsInfo      ( const std::string& /*request*/ )
{
    Json::Value val;
    val[ "cmd" ]        = "videocards";
    val[ "visible" ]    = " no diggy diggy ";

    return val.toStyledString();
}

// ***********************
//
std::string QueryHandlers::GetNodeInfo         ( const std::string& /*request*/ )
{
    return "";
}

// ***********************
//
std::string QueryHandlers::GetTimeLinesInfo    ( const std::string& /*request*/ )
{
    Json::Value ret;
    ret[ "command" ] = "timelines";
    ret[ "scenes" ] = Json::arrayValue;

    for( auto s : m_appLogic->GetBVProject()->GetScenes() )
    {
        Json::Value val;
		val[ "name" ] = s->GetName();
        JsonSerializeObject ser;
		s->GetTimeline()->Serialize(ser);
        val[ "timelines" ] = ser.GetJson();
        ret[ "scenes" ].append( val );
    }

    return ret.toStyledString();
}

// ***********************
//
std::string QueryHandlers::PerformanceInfo  ( const std::string& /*request*/ )
{       
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

    return root.toStyledString();
}

// ***********************
//
std::string QueryHandlers::TreeStructureInfo   ( const std::string& /*request*/ )
{
    ReqPrint( m_appLogic->GetBVProject()->GetModelSceneRoot(), 1 );

    Json::Value root;
    root[ "command" ] = "scene_tree";

    root[ "scenes" ] = Json::arrayValue;

    for( auto s : m_appLogic->GetBVProject()->GetScenes() )
        root[ "scenes" ].append( SerializeSceneModel( s ) );


    return root.toStyledString();
}

// ***********************
//
std::wstring QueryHandlers::ListProjectNames    ( const std::string& /*request*/ )
{
    auto pm = ProjectManager::GetInstance();
    
    auto pns = pm->ListProjectsNames();
    auto pList = ToJSONArray( pns );

    return MakeSceneStructureResponse( "ListProjectNames", "list", pList );
}

// ***********************
//
std::wstring QueryHandlers::ListScenes          ( const std::string& request )
{
    auto pm = ProjectManager::GetInstance();

    auto name = GetRequestParamValue( request )[ "projectName" ].asString();
    auto sns = pm->ListScenesNames( name );

    auto pList = ToJSONArray( sns );

    return MakeSceneStructureResponse( "ListScenes", "list", pList );
}

// ***********************
//
std::wstring QueryHandlers::ListAssetsPaths     ( const std::string& request )
{
    auto pm = ProjectManager::GetInstance();

    auto projName = GetRequestParamValue( request )[ "projectName" ].asString();
    auto catName = GetRequestParamValue( request )[ "categoryName" ].asString();

    auto sns = pm->ListAssetsPaths( projName, catName );

    auto pList = ToJSONArray( sns );

    return MakeSceneStructureResponse( "ListAssetPaths", "list", pList );
}

// ***********************
//
std::wstring QueryHandlers::ListCategoriesNames ( const std::string& /*request*/ )
{
    auto pm = ProjectManager::GetInstance();

    auto sns = pm->ListCategoriesNames();
    auto pList = ToJSONArray( sns );

    return MakeSceneStructureResponse( "ListCategoriesNames", "list", pList );
}

// ***********************
//
std::wstring QueryHandlers::ListProjects        ( const std::string& /*request*/ )
{
    auto pm = ProjectManager::GetInstance();

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

    return MakeSceneStructureResponse( "ListProjects", "list", list );
}

} //bv
