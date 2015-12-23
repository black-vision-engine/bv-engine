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
std::string QueryHandlers::ListAssets  ( const std::string& request, unsigned int requestID )
{
    JsonDeserializeObject deser;
    JsonSerializeObject ser;
    deser.Load( request );

    std::string category = deser.GetAttribute( "CategoryName" );

    PrepareResponseTemplate( ser, InfoEvent::Command::ListAssets, requestID, true );

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
std::string QueryHandlers::VideoCardsInfo      ( const std::string& /*request*/, unsigned int requestID )
{
    JsonSerializeObject ser;
    PrepareResponseTemplate( ser, InfoEvent::Command::Videocards, requestID, true );

    ser.SetAttribute( "visible", " no diggy diggy " );

    return ser.GetString();
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
        ErrorResponseTemplate( ser, InfoEvent::Command::NodeInfo, requestID, "Node not found." );
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
std::string QueryHandlers::GetMinimalSceneInfo  ( const std::string& request, unsigned int requestID )
{
    JsonDeserializeObject deser;
    JsonSerializeObject ser;
    deser.Load( request );

    std::string sceneName = deser.GetAttribute( "SceneName" );
    
    auto scene = m_appLogic->GetBVProject()->GetProjectEditor()->GetScene( sceneName );
    if( scene == nullptr )
    {
        ErrorResponseTemplate( ser, InfoEvent::Command::MinimalSceneInfo, requestID, "Scene not found." );
        return ser.GetString();
    }

    // Prevent srialization from serializing child nodes.
    auto context = static_cast<BVSerializeContext*>( ser.GetSerializeContext() );
    context->detailedInfo = false;

    scene->Serialize( ser );
    PrepareResponseTemplate( ser, InfoEvent::Command::MinimalSceneInfo, requestID, true );

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

        ser.ExitChild();
    }
    ser.ExitChild();

    return ser.GetString();
}

// ***********************
//
std::string QueryHandlers::PerformanceInfo  ( const std::string& /*request*/, unsigned int requestID )
{       
    auto& frameStats = m_appLogic->FrameStats();
    auto& sections = frameStats.RegisteredSections();

    PerformanceMonitor::Calculate( m_appLogic->GetStatsCalculator() );
    JsonSerializeObject ser;

    PrepareResponseTemplate( ser, InfoEvent::Command::Performance, requestID, true );

    ser.SetAttribute( "fps", toString( PerformanceMonitor::Stats.fps ) );
    ser.SetAttribute( "fps_avg", toString( PerformanceMonitor::Stats.fps_avg ) );
    ser.SetAttribute( "ram", toString( PerformanceMonitor::Stats.ram ) );
    ser.SetAttribute( "vram", toString( PerformanceMonitor::Stats.vram ) );
    ser.SetAttribute( "cpu", toString( PerformanceMonitor::Stats.cpu ) );

    for( auto name : sections )
    {
        ser.EnterChild( name );

        ser.SetAttribute( "average", toString( frameStats.ExpectedValue( name ) ) );
        ser.SetAttribute( "minVal", toString( frameStats.MinVal( name ) ) );
        ser.SetAttribute( "maxVal", toString( frameStats.MaxVal( name ) ) );
        ser.SetAttribute( "variance", toString( frameStats.Variance( name ) ) );

        ser.ExitChild();
    }

    return ser.GetString();
}

// ***********************
//
std::string QueryHandlers::TreeStructureInfo   ( const std::string& /*request*/, unsigned int requestID )
{
    JsonSerializeObject ser;
    PrepareResponseTemplate( ser, InfoEvent::Command::TreeStructure, requestID, true );

    ser.EnterArray( "scenes" );

    for( auto sceneModel : m_appLogic->GetBVProject()->GetScenes() )
    {
        ser.EnterChild( "scene" );
        sceneModel->Serialize( ser );
        ser.ExitChild();
    }

    ser.ExitChild();

    return ser.GetString();
}

// ***********************
//
std::wstring QueryHandlers::ListProjectNames    ( const std::string& /*request*/, unsigned int requestID )
{
    JsonSerializeObject ser;
    PrepareResponseTemplate( ser, InfoEvent::Command::ListProjectNames, requestID, true );

    auto pm = ProjectManager::GetInstance();
    auto pns = pm->ListProjectsNames();
    
    ser.EnterArray( "Projects" );
    for( auto pn : pns )
    {
        ser.EnterChild( "name" );
        ser.SetAttribute( "Name", pn.Str() );
        ser.ExitChild();
    }
    ser.ExitChild();

    return toWString( ser.GetString() );
}

// ***********************
//
std::wstring QueryHandlers::ListScenes          ( const std::string& request, unsigned int requestID )
{
    JsonSerializeObject ser;
    PrepareResponseTemplate( ser, InfoEvent::Command::ListScenes, requestID, true );

    auto pm = ProjectManager::GetInstance();

    auto name = GetRequestParamValue( request )[ "projectName" ].asString();
    auto sns = pm->ListScenesNames( name );

    ser.EnterArray( "list" );
    for( auto scene : sns )
        ser.SetAttribute( "", scene.Str() );
    ser.ExitChild();

    return toWString( ser.GetString() );
}

// ***********************
//
std::wstring QueryHandlers::ListAssetsPaths     ( const std::string& request, unsigned int requestID )
{
    JsonSerializeObject ser;
    PrepareResponseTemplate( ser, InfoEvent::Command::ListAssetsPaths, requestID, true );

    auto pm = ProjectManager::GetInstance();

    auto projName = GetRequestParamValue( request )[ "projectName" ].asString();
    auto catName = GetRequestParamValue( request )[ "categoryName" ].asString();

    auto sns = pm->ListAssetsPaths( projName, catName );

    ser.EnterArray( "list" );
    for( auto assetPath : sns )
        ser.SetAttribute( "", assetPath.Str() );
    ser.ExitChild();

    return toWString( ser.GetString() );
}

// ***********************
//
std::wstring QueryHandlers::ListCategoriesNames ( const std::string& /*request*/, unsigned int requestID )
{
    JsonSerializeObject ser;
    PrepareResponseTemplate( ser, InfoEvent::Command::ListCategoriesNames, requestID, true );

    auto pm = ProjectManager::GetInstance();
    auto sns = pm->ListCategoriesNames();

    ser.EnterArray( "list" );
    for( auto category : sns )
        ser.SetAttribute( "", category );
    ser.ExitChild();

    return toWString( ser.GetString() );
}

// ***********************
//
std::wstring QueryHandlers::ListProjects        ( const std::string& /*request*/, unsigned int requestID )
{
    JsonSerializeObject ser;
    PrepareResponseTemplate( ser, InfoEvent::Command::ListProjects, requestID, true );

    auto pm = ProjectManager::GetInstance();
    auto pns = pm->ListProjectsNames();

    ser.EnterArray( "list" );
    for( auto p : pns )
    {
        auto scenesCount = pm->ListScenesNames( p ).size();

        ser.SetAttribute( "name", p.Str() );
        ser.SetAttribute( "scenesCount", toString( scenesCount ) );
    }
    ser.ExitChild();

    return toWString( ser.GetString() );
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
        ErrorResponseTemplate( ser, InfoEvent::Command::CheckTimelineTime, requestID, "Scene not found." );
        return ser.GetString();
    }

    auto sceneTimeline = scene->GetTimeline();
    auto checkedTimeline = sceneTimeline->GetChild( timelineName );
    if( checkedTimeline == nullptr )
    {
        ErrorResponseTemplate( ser, InfoEvent::Command::CheckTimelineTime, requestID, "Timeline not found." );
        return ser.GetString();
    }

    PrepareResponseTemplate( ser, InfoEvent::Command::CheckTimelineTime, requestID, true );
    TimeType time = checkedTimeline->GetLocalTime();
    ser.SetAttribute( "Time", toString( time ) );

    return ser.GetString();
}

} //bv
