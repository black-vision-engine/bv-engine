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
QueryHandlers::QueryHandlers    ( BVAppLogic * appLogic )
    :   m_appLogic( appLogic )
{
}

// ***********************
//
QueryHandlers::~QueryHandlers   ()
{
}

// ***********************
//
void QueryHandlers::Info        ( bv::IEventPtr evt )
{
    if( evt->GetEventType() == bv::InfoEvent::Type() )
    {
        bv::InfoEventPtr infoEvent = std::static_pointer_cast<bv::InfoEvent>( evt );

        InfoEvent::Command command = infoEvent->InfoCommand;
        auto & request = infoEvent->Request;
        auto eventID = infoEvent->EventID;

        JsonSerializeObject responseJSON;

        if( command == InfoEvent::Command::TreeStructure )
            TreeStructureInfo( responseJSON, request, eventID );
        else if( command == InfoEvent::Command::MinimalTreeStructure )
            MinimalTreeStructureInfo( responseJSON, request, eventID );
        else if( command == InfoEvent::Command::ListSceneAssets )
            ListSceneAssets( responseJSON, request, eventID );
        else if( command == InfoEvent::Command::ListAssetsPaths )
            ListAssetsPaths( responseJSON, request, eventID );
        else if( command == InfoEvent::Command::ListCategoriesNames )
            ListCategoriesNames( responseJSON, request, eventID );
        else if( command == InfoEvent::Command::ListProjectNames )
            ListProjectNames( responseJSON, request, eventID );
        else if( command == InfoEvent::Command::ListProjects )
            ListProjects( responseJSON, request, eventID );
        else if( command == InfoEvent::Command::ListScenes )
            ListScenes( responseJSON, request, eventID );
        else if( command == InfoEvent::Command::ListAllFolders )
            ListAllFolders( responseJSON, request, eventID );
        else if( command == InfoEvent::Command::ListResourcesInFolders )
            ListResourcesInFolders( responseJSON, request, eventID );
        else if( command == InfoEvent::Command::ListAllResources )
            ListAllResources( responseJSON, request, eventID );
        else if( command == InfoEvent::Command::Performance )
            PerformanceInfo( responseJSON, request, eventID );
        else if( command == InfoEvent::Command::Timelines )
            GetTimeLinesInfo( responseJSON, request, eventID );
        else if( command == InfoEvent::Command::ListTimelineKeyframes )
            ListTimelineKeyframes( responseJSON, request, eventID );
        else if( command == InfoEvent::Command::NodeInfo )
            GetNodeInfo( responseJSON, request, eventID );
        else if ( command == InfoEvent::Command::PluginInfo )
            PluginInfo( responseJSON, request, eventID );
        else if( command == InfoEvent::Command::MinimalSceneInfo )
            GetMinimalSceneInfo( responseJSON, request, eventID );
        else if( command == InfoEvent::Command::Videocards )
            VideoCardsInfo( responseJSON, request, eventID );
        else if( command == InfoEvent::Command::CheckTimelineTime )
            CheckTimelineTime( responseJSON, request, eventID );
        else
        {
            SendSimpleErrorResponse( command, eventID, infoEvent->SocketID, "Unknown command" );
            return;
        }
        
        SendResponse( responseJSON, infoEvent->SocketID, infoEvent->EventID );
    }
}

// ***********************
//
void         QueryHandlers::ListSceneAssets          ( JsonSerializeObject & ser, const std::string & request, int eventID )
{
    JsonDeserializeObject deser;
    deser.Load( request );

    std::string category = deser.GetAttribute( "CategoryName" );

    PrepareResponseTemplate( ser, InfoEvent::Command::ListSceneAssets, eventID, true );

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
            {
                descriptor->Serialize( ser );
            }
        }
    }
    ser.ExitChild();
}

// ***********************
//
void         QueryHandlers::VideoCardsInfo               ( JsonSerializeObject & ser, const std::string & /*request*/, int eventID )
{
    PrepareResponseTemplate( ser, InfoEvent::Command::Videocards, eventID, true );

    ser.SetAttribute( "visible", " no diggy diggy " );
}

// ***********************
//
void         QueryHandlers::GetNodeInfo                  ( JsonSerializeObject & ser, const std::string & request, int eventID )
{
    JsonDeserializeObject deser;
    deser.Load( request );

    std::string sceneName = deser.GetAttribute( "SceneName" );
    std::string nodePath = deser.GetAttribute( "NodePath" );
    
    auto node = m_appLogic->GetBVProject()->GetProjectEditor()->GetNode( sceneName, nodePath );
    if( node )
    {
        // Prevent srialization from serializing child nodes.
        auto context = static_cast<BVSerializeContext*>( ser.GetSerializeContext() );
        context->recursive = false;

        PrepareResponseTemplate( ser, InfoEvent::Command::NodeInfo, eventID, true );

        ser.SetAttribute( "SceneName", sceneName );
        ser.SetAttribute( "NodePath", nodePath );

        std::static_pointer_cast< model::BasicNode >( node )->Serialize( ser );
    }
    else
    {
        ErrorResponseTemplate( ser, InfoEvent::Command::NodeInfo, eventID, "Node not found." );
    }
}

// ***********************
//
void         QueryHandlers::GetMinimalSceneInfo          ( JsonSerializeObject & ser, const std::string & request, int eventID )
{
    JsonDeserializeObject deser;
    deser.Load( request );

    std::string sceneName = deser.GetAttribute( "SceneName" );
    
    auto scene = m_appLogic->GetBVProject()->GetProjectEditor()->GetScene( sceneName );
    if( scene == nullptr )
    {
        ErrorResponseTemplate( ser, InfoEvent::Command::MinimalSceneInfo, eventID, "Scene not found." );
        return;
    }

    // Prevent srialization from serializing child nodes.
    auto context = static_cast< BVSerializeContext * >( ser.GetSerializeContext() );
    context->detailedInfo = false;

    scene->Serialize( ser );
    PrepareResponseTemplate( ser, InfoEvent::Command::MinimalSceneInfo, eventID, true );
}

// ***********************
//
void         QueryHandlers::GetTimeLinesInfo         ( JsonSerializeObject & ser, const std::string & /*request*/, int eventID )
{
    PrepareResponseTemplate( ser, InfoEvent::Command::Timelines, eventID, true );

    ser.EnterArray( "scenes" );
    for( auto s : m_appLogic->GetBVProject()->GetScenes() )
    {
        ser.EnterChild( "scene" );

        ser.SetAttribute( "name", s->GetName() );
        ser.EnterChild( "timelines" );
        s->GetTimeline()->Serialize(ser);

        ser.ExitChild();
        ser.ExitChild();
    }
}

// ***********************
//
void         QueryHandlers::PerformanceInfo          ( JsonSerializeObject & ser, const std::string & /*request*/, int eventID )
{       
    auto & frameStats = m_appLogic->FrameStats();
    auto & sections = frameStats.RegisteredSections();

    PerformanceMonitor::Calculate( m_appLogic->GetStatsCalculator() );

    PrepareResponseTemplate( ser, InfoEvent::Command::Performance, eventID, true );

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
}

// ***********************
//
void         QueryHandlers::TreeStructureInfo    ( JsonSerializeObject & ser, const std::string & /*request*/, int eventID )
{
    PrepareResponseTemplate( ser, InfoEvent::Command::TreeStructure, eventID, true );

    ser.EnterArray( "scenes" );

    for( auto sceneModel : m_appLogic->GetBVProject()->GetScenes() )
    {
        ser.EnterChild( "scene" );
        sceneModel->Serialize( ser );
        ser.ExitChild();
    }

    ser.ExitChild();
}

// ***********************
//
void         QueryHandlers::ListProjectNames     ( JsonSerializeObject & ser, const std::string & /*request*/, int eventID )
{
    PrepareResponseTemplate( ser, InfoEvent::Command::ListProjectNames, eventID, true );

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
}

// ***********************
//
void         QueryHandlers::ListScenes           ( JsonSerializeObject & ser, const std::string & request, int eventID )
{
    PrepareResponseTemplate( ser, InfoEvent::Command::ListScenes, eventID, true );

    auto pm = ProjectManager::GetInstance();

    auto name = GetRequestParamValue( request )[ "projectName" ].asString();
    auto sns = pm->ListScenesNames( name );

    ser.EnterArray( "list" );
    for( auto scene : sns )
    {
        ser.SetAttribute( "", scene.Str() );
    }
    ser.ExitChild();
}

// ***********************
//
void        QueryHandlers::ListAssetsPaths     ( JsonSerializeObject & ser, const std::string & request, int eventID )
{
    PrepareResponseTemplate( ser, InfoEvent::Command::ListAssetsPaths, eventID, true );

    auto pm = ProjectManager::GetInstance();

    auto projName = GetRequestParamValue( request )[ "projectName" ].asString();
    auto catName = GetRequestParamValue( request )[ "categoryName" ].asString();

    auto sns = pm->ListAssetsPaths( projName, catName );

    ser.EnterArray( "list" );
    for( auto assetPath : sns )
    {
        ser.SetAttribute( "", assetPath.Str() );
    }
    ser.ExitChild();
}

// ***********************
//
void        QueryHandlers::ListCategoriesNames ( JsonSerializeObject & ser, const std::string & /*request*/, int eventID )
{
    PrepareResponseTemplate( ser, InfoEvent::Command::ListCategoriesNames, eventID, true );

    auto pm = ProjectManager::GetInstance();
    auto sns = pm->ListCategoriesNames();

    ser.EnterArray( "list" );
    for( auto category : sns )
    {
        ser.SetAttribute( "", category );
    }
    ser.ExitChild();
}

// ***********************
//
void        QueryHandlers::ListProjects        ( JsonSerializeObject & ser, const std::string & /*request*/, int eventID )
{
    PrepareResponseTemplate( ser, InfoEvent::Command::ListProjects, eventID, true );

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
}

// ***********************
//
void         QueryHandlers::CheckTimelineTime    ( JsonSerializeObject & ser, const std::string & request, int eventID )
{
    JsonDeserializeObject deser;
    deser.Load( request );

    std::string sceneName = deser.GetAttribute( "SceneName" );
    std::string timelineName = deser.GetAttribute( "TimelineName" );

    auto scene = m_appLogic->GetBVProject()->GetScene( sceneName );
    if( scene == nullptr )
    {
        ErrorResponseTemplate( ser, InfoEvent::Command::CheckTimelineTime, eventID, "Scene not found." );
        return;
    }

    auto sceneTimeline = scene->GetTimeline();
    auto checkedTimeline = sceneTimeline->GetChild( timelineName );
    if( checkedTimeline == nullptr )
    {
        ErrorResponseTemplate( ser, InfoEvent::Command::CheckTimelineTime, eventID, "Timeline not found." );
        return;
    }

    PrepareResponseTemplate( ser, InfoEvent::Command::CheckTimelineTime, eventID, true );
    TimeType time = checkedTimeline->GetLocalTime();
    ser.SetAttribute( "Time", toString( time ) );
    ser.SetAttribute( "SceneName", sceneName );
    ser.SetAttribute( "TimelineName", timelineName );
}

// ***********************
//
void         QueryHandlers::MinimalTreeStructureInfo ( JsonSerializeObject & ser, const std::string & /*request*/, int eventID )
{
    auto context = static_cast<BVSerializeContext*>( ser.GetSerializeContext() );
    context->recursive = true;
    context->detailedInfo = false;
    context->pluginsInfo = true;
    
    PrepareResponseTemplate( ser, InfoEvent::Command::TreeStructure, eventID, true );

    ser.EnterArray( "scenes" );

    for( auto sceneModel : m_appLogic->GetBVProject()->GetScenes() )
    {
        ser.EnterChild( "scene" );
        sceneModel->Serialize( ser );
        ser.ExitChild();
    }

    ser.ExitChild();
}

// ***********************
//
void         QueryHandlers::PluginInfo           ( JsonSerializeObject & ser, const std::string & request, int eventID )
{
    JsonDeserializeObject deser;
    deser.Load( request );


    std::string nodePath = deser.GetAttribute( "NodePath" );
    std::string pluginName = deser.GetAttribute( "PluginName" );
    std::string sceneName = deser.GetAttribute( "SceneName" );

    auto node = m_appLogic->GetBVProject()->GetProjectEditor()->GetNode( sceneName, nodePath );
    if( node == nullptr )
    {
        ErrorResponseTemplate( ser, InfoEvent::Command::PluginInfo, eventID, "Node not found" );
        return;
    }

    auto iplugin = node->GetPlugin( pluginName );
    if( iplugin == nullptr )
    {
        ErrorResponseTemplate( ser, InfoEvent::Command::PluginInfo, eventID, "Plugin not found" );
        return;
    }

    PrepareResponseTemplate( ser, InfoEvent::Command::PluginInfo, eventID, true );

    auto plugin = std::static_pointer_cast< BasePlugin< IPlugin > >( iplugin );
    plugin->Serialize( ser );
}

// ***********************
//
void         QueryHandlers::ListTimelineKeyframes    ( JsonSerializeObject & ser, const std::string& request, int eventID )
{
    JsonDeserializeObject deser;
    deser.Load( request );

    std::string TimelinePath = deser.GetAttribute( "TimelinePath" );

    auto editor = m_appLogic->GetBVProject()->GetProjectEditor();
    auto timeEvaluator = editor->GetTimeEvaluator( TimelinePath );
    if( timeEvaluator == nullptr )
    {
        ErrorResponseTemplate( ser, InfoEvent::Command::ListTimelineKeyframes, eventID, "Timeline not found" );
        return;
    }

    if( timeEvaluator->GetType() != DefaultTimeline::Type() )
    {
        ErrorResponseTemplate( ser, InfoEvent::Command::ListTimelineKeyframes, eventID, "Time evaluator can't be casted to DefaultTimeline" );
        return;
    }
    auto timeline = std::static_pointer_cast<DefaultTimeline>( timeEvaluator );
    
    PrepareResponseTemplate( ser, InfoEvent::Command::ListTimelineKeyframes, eventID, true );

    ser.EnterArray( "Keyframes" );
    for( int i = 0; i < timeline->NumKeyFrames(); ++i )
    {
        auto keyframeEvent = timeline->GetKeyFrameEvent( i );
        keyframeEvent->Serialize( ser );
    }
    ser.ExitChild();

    ser.SetAttribute( "TimelinePath", TimelinePath );
}

// ***********************
//
void    QueryHandlers::ListAllFolders          ( JsonSerializeObject & ser, const std::string & , int /*eventID*/ )
{
    { ser; }
    //PrepareResponseTemplate( ser, InfoEvent::Command::ListAllFolders, eventID, true );

    //auto pm = ProjectManager::GetInstance();
    //auto sns = pm->ListCategoriesNames();

    //ser.EnterArray( "list" );
    //for( auto category : sns )
    //    ser.SetAttribute( "", category );
    //ser.ExitChild();
}

// ***********************
//
void    QueryHandlers::ListResourcesInFolders  ( JsonSerializeObject & ser, const std::string & /*request*/, int /*eventID*/ )
{
    { ser; }
    // @todo Zaimplementowaæ
    // S¹ stworzone funkcje do ustawiania domyœlnych wartoœci, jakie powinny byæ w odpowiedziach do eventów:
    // - PrepareResponseTemplate
    // - ErrorResponseTemplate
    // Zobacz jak s¹ u¿ywane w innych eventach.
}

// ***********************
//
void    QueryHandlers::ListAllResources        ( JsonSerializeObject & ser, const std::string & /*request*/, int /*eventID*/ )
{
    { ser; }
    // @todo Zaimplementowaæ
    // S¹ stworzone funkcje do ustawiania domyœlnych wartoœci, jakie powinny byæ w odpowiedziach do eventów:
    // - PrepareResponseTemplate
    // - ErrorResponseTemplate
    // Zobacz jak s¹ u¿ywane w innych eventach.
}

} //bv
