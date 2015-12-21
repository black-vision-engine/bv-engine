#include "QueryHandlers.h"

#include "../../BVAppLogic.h"
#include "../../UseLoggerBVAppModule.h"
#include "Tools/IncludeJSON.h"
#include "PerformanceMonitor.h"
#include "Engine/Models/BasicNode.h"
#include "Engine/Events/EventHelpers.h"             // wstring to string conversions and vice versa
#include "EventHandlerHelpers.h"
#include "Assets/AssetDescsWithUIDs.h"
#include "ProjectManager.h"
#include "Engine/Models/BVProjectEditor.h"

#include "Serialization/Json/JsonDeserializeObject.h"
#include "Serialization/BVSerializeContext.h"

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
        std::string& request = infoEvent->Request;
        unsigned int requestID = infoEvent->EventID;

        wstring responseMessage;

        if( command == InfoEvent::Command::TreeStructure )
            responseMessage = toWString( TreeStructureInfo( request, requestID ) );
        else if( command == InfoEvent::Command::ListAssets )
            responseMessage = toWString( ListAssets( request, requestID ) );
        else if( command == InfoEvent::Command::ListAssetsPaths )
            responseMessage = ListAssetsPaths( request, requestID );
        else if( command == InfoEvent::Command::ListCategoriesNames )
            responseMessage = ListCategoriesNames( request, requestID );
        else if( command == InfoEvent::Command::ListProjectNames )
            responseMessage = ListProjectNames( request, requestID );
        else if( command == InfoEvent::Command::ListProjects )
            responseMessage = ListProjects( request, requestID );
        else if( command == InfoEvent::Command::ListScenes )
            responseMessage = ListScenes( request, requestID );
        else if( command == InfoEvent::Command::Performance )
            responseMessage = toWString( PerformanceInfo( request, requestID ) );
        else if( command == InfoEvent::Command::Timelines )
            responseMessage = toWString( GetTimeLinesInfo( request, requestID ) );
        else if( command == InfoEvent::Command::NodeInfo )
            responseMessage = toWString( GetNodeInfo( request, requestID ) );
        else if( command == InfoEvent::Command::MinimalSceneInfo )
            responseMessage = toWString( GetMinimalSceneInfo( request, requestID ) );
        else if( command == InfoEvent::Command::Videocards )
            responseMessage = toWString( VideoCardsInfo( request, requestID ) );
        else if( command == InfoEvent::Command::CheckTimelineTime )
            responseMessage = toWString( CheckTimelineTime( request, requestID ) );
        
        ResponseEventPtr msg = std::make_shared<ResponseEvent>();
        msg->Response = responseMessage;
        msg->SocketID = infoEvent->SocketID;
        GetDefaultEventManager().QueueResponse( msg );
    }
}


// ***********************
//
std::string QueryHandlers::ListAssets  ( const std::string& request, unsigned int /*requestID*/ )
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
std::string QueryHandlers::VideoCardsInfo      ( const std::string& /*request*/, unsigned int /*requestID*/ )
{
    Json::Value val;
    val[ "cmd" ]        = "videocards";
    val[ "visible" ]    = " no diggy diggy ";

    return val.toStyledString();
}

// ***********************
//
std::string QueryHandlers::GetNodeInfo         ( const std::string& request, unsigned int requestID )
{
    JsonDeserializeObject deser;
    JsonSerializeObject ser;
    deser.Load( request );

    std::string sceneName = deser.GetAttribute( "SceneName" );
    std::string nodePath = deser.GetAttribute( "NodePath" );
    
    auto node = m_appLogic->GetBVProject()->GetProjectEditor()->GetNode( sceneName, nodePath );
    if( node == nullptr )
    {
        PrepareResponseTemplate( ser, InfoEvent::Command::NodeInfo, requestID, false );
        return ser.GetString();
    }

    // Prevent srialization from serializing child nodes.
    auto context = static_cast<BVSerializeContext*>( ser.GetSerializeContext() );
    context->recursive = false;

    PrepareResponseTemplate( ser, InfoEvent::Command::NodeInfo, requestID, true );
    std::static_pointer_cast< model::BasicNode >( node )->Serialize( ser );

    return ser.GetString();
}

// ***********************
//
std::string QueryHandlers::GetMinimalSceneInfo  ( const std::string& request, unsigned int /*requestID*/ )
{
    JsonDeserializeObject deser;
    JsonSerializeObject ser;
    deser.Load( request );

    std::string sceneName = deser.GetAttribute( "SceneName" );
    
    ser.SetAttribute( "cmd", toString( SerializationHelper::T2WString( InfoEvent::Command::MinimalSceneInfo ) ) );

    auto scene = m_appLogic->GetBVProject()->GetProjectEditor()->GetScene( sceneName );
    if( scene == nullptr )
    {
        ser.SetAttribute( "scene", SerializationHelper::EMPTY_STRING );
        return ser.GetString();
    }

    // Prevent srialization from serializing child nodes.
    auto context = static_cast<BVSerializeContext*>( ser.GetSerializeContext() );
    context->detailedInfo = false;

    scene->Serialize( ser );

    return ser.GetString();
}

// ***********************
//
std::string QueryHandlers::GetTimeLinesInfo    ( const std::string& /*request*/, unsigned int requestID )
{
    JsonSerializeObject ser;
    PrepareResponseTemplate( ser, InfoEvent::Command::Timelines, requestID, true );

    ser.EnterArray( "scenes" );
    for( auto s : m_appLogic->GetBVProject()->GetScenes() )
    {
        ser.EnterChild( "scene" );
        ser.SetAttribute( "name", s->GetName() );
        ser.EnterChild( "timelines" );
        s->GetTimeline()->Serialize(ser);
    }

    return ser.GetString();
}

// ***********************
//
std::string QueryHandlers::PerformanceInfo  ( const std::string& /*request*/, unsigned int /*requestID*/ )
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
std::string QueryHandlers::TreeStructureInfo   ( const std::string& /*request*/, unsigned int /*requestID*/ )
{
    // Fixme: There's no need to use JsonCpp and JsonSerializeObject at the same time.
    // Rewrite this part of code to use only JsonSerializeObject.
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
std::wstring QueryHandlers::ListProjectNames    ( const std::string& /*request*/, unsigned int /*requestID*/ )
{
    auto pm = ProjectManager::GetInstance();
    
    auto pns = pm->ListProjectsNames();
    auto pList = ToJSONArray( pns );

    return MakeSceneStructureResponse( "ListProjectNames", "list", pList );
}

// ***********************
//
std::wstring QueryHandlers::ListScenes          ( const std::string& request, unsigned int /*requestID*/ )
{
    auto pm = ProjectManager::GetInstance();

    auto name = GetRequestParamValue( request )[ "projectName" ].asString();
    auto sns = pm->ListScenesNames( name );

    auto pList = ToJSONArray( sns );

    return MakeSceneStructureResponse( "ListScenes", "list", pList );
}

// ***********************
//
std::wstring QueryHandlers::ListAssetsPaths     ( const std::string& request, unsigned int /*requestID*/ )
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
std::wstring QueryHandlers::ListCategoriesNames ( const std::string& /*request*/, unsigned int /*requestID*/ )
{
    auto pm = ProjectManager::GetInstance();

    auto sns = pm->ListCategoriesNames();
    auto pList = ToJSONArray( sns );

    return MakeSceneStructureResponse( "ListCategoriesNames", "list", pList );
}

// ***********************
//
std::wstring QueryHandlers::ListProjects        ( const std::string& /*request*/, unsigned int /*requestID*/ )
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

// ***********************
//
std::string QueryHandlers::CheckTimelineTime   ( const std::string& request, unsigned int requestID )
{
    JsonDeserializeObject deser;
    JsonSerializeObject ser;
    deser.Load( request );

    std::string sceneName = deser.GetAttribute( "SceneName" );
    std::string timelineName = deser.GetAttribute( "TimelineName" );

    auto scene = m_appLogic->GetBVProject()->GetScene( sceneName );
    if( scene == nullptr )
    {
        PrepareResponseTemplate( ser, InfoEvent::Command::CheckTimelineTime, requestID, false );
        return ser.GetString();
    }

    auto sceneTimeline = scene->GetTimeline();
    auto checkedTimeline = sceneTimeline->GetChild( timelineName );
    if( checkedTimeline == nullptr )
    {
        PrepareResponseTemplate( ser, InfoEvent::Command::CheckTimelineTime, requestID, false );
        return ser.GetString();
    }

    PrepareResponseTemplate( ser, InfoEvent::Command::CheckTimelineTime, requestID, true );
    TimeType time = checkedTimeline->GetLocalTime();
    ser.SetAttribute( "Time", toString( time ) );

    return ser.GetString();
}

} //bv
