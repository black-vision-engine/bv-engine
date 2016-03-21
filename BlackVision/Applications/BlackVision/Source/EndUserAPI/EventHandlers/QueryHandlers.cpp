#include "QueryHandlers.h"

#include "BVAppLogic.h"
#include "UseLoggerBVAppModule.h"
#include "Tools/IncludeJSON.h"
#include "PerformanceMonitor.h"
#include "Engine/Models/BasicNode.h"
#include "EventHandlerHelpers.h"
#include "Assets/AssetDescsWithUIDs.h"
#include "ProjectManager.h"
#include "Engine/Models/BVProjectTools.h"
#include "Engine/Models/BVProjectEditor.h"

#include "Serialization/Json/JsonDeserializeObject.h"
#include "Serialization/BV/BVSerializeContext.h"
#include "Serialization/BV/BVDeserializeContext.h"


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
        auto request = infoEvent->Request;
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
        else if( command == InfoEvent::Command::ListPresets )
            ListPresets( responseJSON, request, eventID );
        else if( command == InfoEvent::Command::ListAllFolders )
            ListAllFolders( responseJSON, request, eventID );
        else if( command == InfoEvent::Command::GetAssetDescriptor )
            GetAssetDescriptor( responseJSON, request, eventID );
        else if( command == InfoEvent::Command::GetAssetThumbnail )
            GetAssetThumbnail( responseJSON, request, eventID );
        //else if( command == InfoEvent::Command::ListResourcesInFolders )
        //    ListResourcesInFolders( responseJSON, request, eventID );
        //else if( command == InfoEvent::Command::ListAllResources )
        //    ListAllResources( responseJSON, request, eventID );
        else if( command == InfoEvent::Command::Performance )
            PerformanceInfo( responseJSON, request, eventID );
        else if( command == InfoEvent::Command::Timelines )
            GetTimeLinesInfo( responseJSON, request, eventID );
        else if( command == InfoEvent::Command::ListTimelineKeyframes )
            ListTimelineKeyframes( responseJSON, request, eventID );
        else if( command == InfoEvent::Command::ListTimelinesParams )
            ListTimelinesParams( responseJSON, request, eventID );
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
void         QueryHandlers::ListSceneAssets          ( JsonSerializeObject & ser, IDeserializer * request, int eventID )
{
    assert( request != nullptr );
    if( request == nullptr )
    {
        ErrorResponseTemplate( ser, InfoEvent::Command::ListSceneAssets, eventID, "Not valid request." );
        return;
    }

    auto bvDeserCo = Cast< BVDeserializeContext* >( request->GetDeserializeContext() );

    std::string category = request->GetAttribute( "CategoryName" );

    PrepareResponseTemplate( ser, InfoEvent::Command::ListSceneAssets, eventID, true );

    ser.EnterArray( "assets" );
    for( auto scene : m_appLogic->GetBVProject()->GetScenes() )
    {
        auto sceneRoot = scene->GetRootNode();

        auto assets = std::make_shared< AssetDescsWithUIDs >();
        GetAssetsWithUIDs( *assets, sceneRoot.get(), true );
        bvDeserCo->SetAssets( assets );

        auto descriptors = assets->GetAssetsDescs();

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
void         QueryHandlers::VideoCardsInfo               ( JsonSerializeObject & ser, IDeserializer * /*request*/, int eventID )
{
    PrepareResponseTemplate( ser, InfoEvent::Command::Videocards, eventID, true );

    ser.SetAttribute( "visible", " no diggy diggy " );
}

// ***********************
//
void         QueryHandlers::GetNodeInfo                  ( JsonSerializeObject & ser, IDeserializer * request, int eventID )
{
    assert( request != nullptr );
    if( request == nullptr )
    {
        ErrorResponseTemplate( ser, InfoEvent::Command::NodeInfo, eventID, "Not valid request." );
        return;
    }

    std::string sceneName = request->GetAttribute( "SceneName" );
    std::string nodePath = request->GetAttribute( "NodePath" );
    
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
void         QueryHandlers::GetMinimalSceneInfo          ( JsonSerializeObject & ser, IDeserializer * request, int eventID )
{
    assert( request != nullptr );
    if( request == nullptr )
    {
        ErrorResponseTemplate( ser, InfoEvent::Command::MinimalSceneInfo, eventID, "Not valid request." );
        return;
    }

    std::string sceneName = request->GetAttribute( "SceneName" );
    
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
    ser.SetAttribute( "preset", SerializationHelper::T2String( IsPresetScene( scene->GetName() ) ) );
    PrepareResponseTemplate( ser, InfoEvent::Command::MinimalSceneInfo, eventID, true );
}

// ***********************
//
void         QueryHandlers::GetTimeLinesInfo         ( JsonSerializeObject & ser, IDeserializer * /*request*/, int eventID )
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
void         QueryHandlers::PerformanceInfo          ( JsonSerializeObject & ser, IDeserializer * /*request*/, int eventID )
{       
    auto & frameStats = m_appLogic->FrameStats();
    auto & sections = frameStats.RegisteredSections();

    PerformanceMonitor::Calculate( m_appLogic->GetStatsCalculator() );

    PrepareResponseTemplate( ser, InfoEvent::Command::Performance, eventID, true );

    ser.SetAttribute( "fps", SerializationHelper::T2String( PerformanceMonitor::Stats.fps ) );
    ser.SetAttribute( "fps_avg", SerializationHelper::T2String( PerformanceMonitor::Stats.fps_avg ) );
    ser.SetAttribute( "ram", SerializationHelper::T2String( PerformanceMonitor::Stats.ram ) );
    ser.SetAttribute( "vram", SerializationHelper::T2String( PerformanceMonitor::Stats.vram ) );
    ser.SetAttribute( "cpu", SerializationHelper::T2String( PerformanceMonitor::Stats.cpu ) );

    for( auto name : sections )
    {
        ser.EnterChild( name );

        ser.SetAttribute( "average", SerializationHelper::T2String( frameStats.ExpectedValue( name ) ) );
        ser.SetAttribute( "minVal", SerializationHelper::T2String( frameStats.MinVal( name ) ) );
        ser.SetAttribute( "maxVal", SerializationHelper::T2String( frameStats.MaxVal( name ) ) );
        ser.SetAttribute( "variance", SerializationHelper::T2String( frameStats.Variance( name ) ) );

        ser.ExitChild();
    }
}

// ***********************
//
void         QueryHandlers::TreeStructureInfo    ( JsonSerializeObject & ser, IDeserializer * /*request*/, int eventID )
{
    PrepareResponseTemplate( ser, InfoEvent::Command::TreeStructure, eventID, true );

    ser.EnterArray( "scenes" );

    for( auto sceneModel : m_appLogic->GetBVProject()->GetScenes() )
    {
        ser.EnterChild( "scene" );
            ser.SetAttribute( "preset", SerializationHelper::T2String( IsPresetScene( sceneModel->GetName() ) ) );
            sceneModel->Serialize( ser );
        ser.ExitChild();
    }

    ser.ExitChild();
}

// ***********************
//
void         QueryHandlers::ListProjectNames     ( JsonSerializeObject & ser, IDeserializer * /*request*/, int eventID )
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
void         QueryHandlers::ListScenes           ( JsonSerializeObject & ser, IDeserializer * request, int eventID )
{
    assert( request != nullptr );
    if( request == nullptr )
    {
        ErrorResponseTemplate( ser, InfoEvent::Command::ListScenes, eventID, "Not valid request." );
        return;
    }

    PrepareResponseTemplate( ser, InfoEvent::Command::ListScenes, eventID, true );

    auto pm = ProjectManager::GetInstance();

    auto name = request->GetAttribute( "projectName" );
    auto path = request->GetAttribute( "path" );

    auto recursive = false;
    auto recStr = request->GetAttribute( "recursive" );
    if( recStr == "true" )
    {
        recursive = true;
    }

    auto sns = pm->ListScenesNames( name, path, recursive );

    ser.SetAttribute( "path", path );
    ser.EnterArray( "list" );
    
    for( auto scene : sns )
    {
        ser.SetAttribute( "", scene.Str() );
    }
    ser.ExitChild();
}

// ***********************
//
void         QueryHandlers::ListPresets           ( JsonSerializeObject & ser, IDeserializer * request, int eventID )
{
    assert( request != nullptr );
    if( request == nullptr )
    {
        ErrorResponseTemplate( ser, InfoEvent::Command::ListPresets, eventID, "Not valid request." );
        return;
    }

    PrepareResponseTemplate( ser, InfoEvent::Command::ListPresets, eventID, true );

    auto pm = ProjectManager::GetInstance();

    auto name = request->GetAttribute( "projectName" );
    auto path = request->GetAttribute( "path" );

    auto recursive = false;
    auto recStr = request->GetAttribute( "recursive" );
    if( recStr == "true" )
    {
        recursive = true;
    }

    auto sns = pm->ListPresets( name, path, recursive );

    ser.SetAttribute( "path", path );
    ser.EnterArray( "list" );
    
    for( auto scene : sns )
    {
        ser.SetAttribute( "", scene.Str() );
    }
    ser.ExitChild();
}

// ***********************
//
void        QueryHandlers::ListAssetsPaths     ( JsonSerializeObject & ser, IDeserializer * request, int eventID )
{
    assert( request != nullptr );
    if( request == nullptr )
    {
        ErrorResponseTemplate( ser, InfoEvent::Command::ListAssetsPaths, eventID, "Not valid request." );
        return;
    }

    PrepareResponseTemplate( ser, InfoEvent::Command::ListAssetsPaths, eventID, true );

    auto pm = ProjectManager::GetInstance();

    auto projName = request->GetAttribute( "projectName" );
    auto catName = request->GetAttribute( "categoryName" );
    auto path = request->GetAttribute( "path" );

    auto recursive = false;
    auto recStr = request->GetAttribute( "recursive" );
    if( recStr == "true" )
    {
        recursive = true;
    }

    auto sns = pm->ListAssetsPaths( projName, catName, path, recursive );

    ser.SetAttribute( "categoryName", catName );
    ser.SetAttribute( "path", path );

    ser.EnterArray( "list" );
    for( auto assetPath : sns )
    {
        ser.SetAttribute( "", assetPath.Str() );
    }
    ser.ExitChild();
}

// ***********************
//
void        QueryHandlers::ListCategoriesNames ( JsonSerializeObject & ser, IDeserializer * /*request*/, int eventID )
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
void        QueryHandlers::ListProjects        ( JsonSerializeObject & ser, IDeserializer * /*request*/, int eventID )
{
    PrepareResponseTemplate( ser, InfoEvent::Command::ListProjects, eventID, true );

    auto pm = ProjectManager::GetInstance();
    auto pns = pm->ListProjectsNames();

    ser.EnterArray( "list" );
    for( auto p : pns )
    {
        auto scenesCount = pm->ListScenesNames( p, "", true ).size();

        ser.SetAttribute( "name", p.Str() );
        ser.SetAttribute( "scenesCount", SerializationHelper::T2String( scenesCount ) );
    }
    ser.ExitChild();
}

// ***********************
//
void         QueryHandlers::GetAssetDescriptor      ( JsonSerializeObject & ser, IDeserializer * request, int eventID )
{
    assert( request != nullptr );
    if( request == nullptr )
    {
        ErrorResponseTemplate( ser, InfoEvent::Command::GetAssetDescriptor, eventID, "Not valid request." );
        return;
    }

    auto projectName = request->GetAttribute( "projectName" );
    auto categoryName = request->GetAttribute( "categoryName" );
    auto path = request->GetAttribute( "path" );

    auto pm = ProjectManager::GetInstance();
    auto desc = pm->GetAssetDesc( projectName, categoryName, path );

    auto context = static_cast<BVSerializeContext*>( ser.GetSerializeContext() );
    context->extendedAssetData = true;      // Width, height, numFrames and so on

    if( desc )
    {
        PrepareResponseTemplate( ser, InfoEvent::Command::GetAssetDescriptor, eventID, true );
        ser.SetAttribute( "projectName", projectName );
        ser.SetAttribute( "categoryName", categoryName );
        ser.SetAttribute( "path", path );

        ser.EnterChild( "AssetData" );
        desc->Serialize( ser );
        ser.ExitChild();
    }
    else
    {
        ErrorResponseTemplate( ser, InfoEvent::Command::GetAssetDescriptor, eventID, "Cannot find asset." );
    }
}

// ***********************
//
void        QueryHandlers::GetAssetThumbnail        ( JsonSerializeObject & ser, IDeserializer * request, int eventID )
{
    assert( request != nullptr );
    if( request == nullptr )
    {
        ErrorResponseTemplate( ser, InfoEvent::Command::GetAssetThumbnail, eventID, "Not valid request." );
        return;
    }

    auto projectName = request->GetAttribute( "projectName" );
    auto categoryName = request->GetAttribute( "categoryName" );
    auto path = request->GetAttribute( "path" );

    auto pm = ProjectManager::GetInstance();
    auto aps = pm->ListAssetsPaths( projectName, categoryName, path, false );
    
    std::vector< std::pair< ThumbnailConstPtr, AssetDescConstPtr > > thumbs;

    for( auto & ap : aps )
    {
        ap = Path( ap.Str().substr( ap.Str().find_first_not_of( categoryName ) ) ); // FIXME: Ugly part of code. Add some function parsing path to category name and path.
        auto desc = pm->GetAssetDesc( projectName, categoryName, ap );

        if( desc )
        {
            auto thumb = AssetManager::GetInstance().LoadThumbnail( desc );
            
            if( thumb != nullptr )  // Fixme Inform editor that some thumbs can't be generated.
                thumbs.push_back( std::make_pair( thumb, desc ) );


            //File::Write( thumb->Data()->Get(), thumb->Data()->Size(), "font.tga", false );
        }
    }

    if( !thumbs.empty() )
    {
        PrepareResponseTemplate( ser, InfoEvent::Command::GetAssetThumbnail, eventID, true );

        ser.EnterArray( "thumbnails" );

        for( auto t : thumbs )
        {
            t.first->Serialize( ser );
            t.second->Serialize( ser );
        }

        ser.ExitChild(); // thumbnails
    }
    else
    {
        ErrorResponseTemplate( ser, InfoEvent::Command::GetAssetThumbnail, eventID, "Cannot find asset." );
    }
}

// ***********************
//
void         QueryHandlers::CheckTimelineTime    ( JsonSerializeObject & ser, IDeserializer * request, int eventID )
{
    assert( request != nullptr );
    if( request == nullptr )
    {
        ErrorResponseTemplate( ser, InfoEvent::Command::CheckTimelineTime, eventID, "Not valid request." );
        return;
    }

    std::string sceneName = request->GetAttribute( "SceneName" );
    std::string timelineName = request->GetAttribute( "TimelineName" );

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
    ser.SetAttribute( "Time", SerializationHelper::T2String( time ) );
    ser.SetAttribute( "SceneName", sceneName );
    ser.SetAttribute( "TimelineName", timelineName );
}

// ***********************
//
void         QueryHandlers::MinimalTreeStructureInfo ( JsonSerializeObject & ser, IDeserializer * /*request*/, int eventID )
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
            ser.SetAttribute( "preset", SerializationHelper::T2String( IsPresetScene( sceneModel->GetName() ) ) );
        ser.ExitChild();
    }

    ser.ExitChild();
}

// ***********************
//
void         QueryHandlers::PluginInfo           ( JsonSerializeObject & ser, IDeserializer * request, int eventID )
{
    assert( request != nullptr );
    if( request == nullptr )
    {
        ErrorResponseTemplate( ser, InfoEvent::Command::PluginInfo, eventID, "Not valid request." );
        return;
    }

    std::string nodePath = request->GetAttribute( "NodePath" );
    std::string pluginName = request->GetAttribute( "PluginName" );
    std::string sceneName = request->GetAttribute( "SceneName" );

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
void         QueryHandlers::ListTimelineKeyframes    ( JsonSerializeObject & ser, IDeserializer * request, int eventID )
{
    assert( request != nullptr );
    if( request == nullptr )
    {
        ErrorResponseTemplate( ser, InfoEvent::Command::ListTimelineKeyframes, eventID, "Not valid request." );
        return;
    }

    std::string TimelinePath = request->GetAttribute( "TimelinePath" );

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
void    QueryHandlers::ListTimelinesParams     ( JsonSerializeObject & ser, IDeserializer * request, int eventID )
{
    auto context = static_cast<BVSerializeContext*>( ser.GetSerializeContext() );
    context->recursive = false;
    context->detailedInfo = false;
    context->sceneNameInTimeline = true;

    assert( request != nullptr );
    if( request == nullptr )
    {
        ErrorResponseTemplate( ser, InfoEvent::Command::ListTimelinesParams, eventID, "Not valid request." );
        return;
    }

    std::string sceneName = request->GetAttribute( "SceneName" );
    auto scene = m_appLogic->GetBVProject()->GetProjectEditor()->GetScene( sceneName );
    if( scene == nullptr )
    {
        ErrorResponseTemplate( ser, InfoEvent::Command::ListTimelinesParams, eventID, "Scene not found" );
        return;
    }

    auto rootNode = scene->GetRootNode();
    assert( rootNode != nullptr );

    auto timelineParamsMap = BVProjectTools::GetParamsOfTimelines( rootNode, scene->GetTimeline() );

    PrepareResponseTemplate( ser, InfoEvent::Command::ListTimelinesParams, eventID, true );

    ser.EnterArray( "Timelines" );

    for( auto& timelineParams : timelineParamsMap )
    {
        ser.EnterChild( "Mapping" );
            ser.SetAttribute( "TimelinePath", timelineParams.first->GetName() );

            ser.EnterArray( "Params" );

            for( auto& timelineParamInfo : timelineParams.second )
            {
                ser.EnterChild( "Param" );
                    ser.SetAttribute( "NodePath", timelineParamInfo.nodePath );
                    ser.SetAttribute( "PluginName", timelineParamInfo.pluginName );
                    ser.SetAttribute( "ParamSubName", timelineParamInfo.paramSubName );
                
                    if( timelineParamInfo.paramOwner == ParamOwner::PO_Plugin )
                        ser.SetAttribute( "ParamOwner", SerializationHelper::T2String( ParamKeyEvent::TargetType::PluginParam ) );
                    else if( timelineParamInfo.paramOwner == ParamOwner::PO_Resource )
                        ser.SetAttribute( "ParamOwner", SerializationHelper::T2String( ParamKeyEvent::TargetType::ResourceParam ) );
                    else if( timelineParamInfo.paramOwner == ParamOwner::PO_GlobalEffect )
                        ser.SetAttribute( "ParamOwner", SerializationHelper::T2String( ParamKeyEvent::TargetType::GlobalEffectParam ) );
                
                    timelineParamInfo.param->Serialize( ser );
                ser.ExitChild();    //  Param
            }

            ser.ExitChild();    //  Params

        ser.ExitChild();    //  Mapping
    }

    ser.ExitChild();    //  Timelines

}

// ***********************
//
void    QueryHandlers::ListAllFolders          ( JsonSerializeObject & ser, IDeserializer * request, int eventID )
{
    assert( request != nullptr );
    if( request == nullptr )
    {
        ErrorResponseTemplate( ser, InfoEvent::Command::ListAllFolders, eventID, "Not valid request." );
        return;
    }

    PrepareResponseTemplate( ser, InfoEvent::Command::ListAllFolders, eventID, true );

    auto catName = request->GetAttribute( "categoryName" );
    auto path = request->GetAttribute( "path" );

    auto pm = ProjectManager::GetInstance();
    auto sns = pm->ListAssetsDirs( catName, path );

    ser.SetAttribute( "categoryName", catName );

    ser.EnterArray( "list" );
    for( auto d : sns )
    {
        ser.SetAttribute( "", d.Str() );
    }
    ser.ExitChild();
}

//// ***********************
////
//void    QueryHandlers::ListResourcesInFolders  ( JsonSerializeObject & ser, const std::string & /*request*/, int /*eventID*/ )
//{
//    { ser; }
//    // @todo Zaimplementowaæ
//    // S¹ stworzone funkcje do ustawiania domyœlnych wartoœci, jakie powinny byæ w odpowiedziach do eventów:
//    // - PrepareResponseTemplate
//    // - ErrorResponseTemplate
//    // Zobacz jak s¹ u¿ywane w innych eventach.
//}
//
//// ***********************
////
//void    QueryHandlers::ListAllResources        ( JsonSerializeObject & ser, const std::string & /*request*/, int /*eventID*/ )
//{
//    { ser; }
//    // @todo Zaimplementowaæ
//    // S¹ stworzone funkcje do ustawiania domyœlnych wartoœci, jakie powinny byæ w odpowiedziach do eventów:
//    // - PrepareResponseTemplate
//    // - ErrorResponseTemplate
//    // Zobacz jak s¹ u¿ywane w innych eventach.
//}

} //bv
