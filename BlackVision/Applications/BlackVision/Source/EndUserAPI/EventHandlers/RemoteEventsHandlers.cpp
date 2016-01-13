#include "RemoteEventsHandlers.h"

#include "PluginEventsHandlers.h"
#include "SceneEventsHandlers.h"
#include "QueryHandlers.h"
#include "HightmapHandlers.h"
#include "EngineStateHandlers.h"
#include "AssetHandlers.h"
#include "GlobalEffectHandler.h"
#include "TimelineHandlers.h"
#include "NodeLogicHandlers.h"

#include "Engine/Events/EventManager.h"
#include "Widgets/Crawler/CrawlerEvents.h"


namespace bv
{

// ***********************
//
RemoteEventsHandlers::RemoteEventsHandlers()
    :   m_pluginEvents( nullptr ),
        m_sceneEvents( nullptr ),
        m_queryEvents( nullptr ),
        m_engineStateEvents( nullptr ),
        m_heightmapEvents( nullptr ),
		m_assetEvents( nullptr ),
        m_timelineHandlers( nullptr ),
        m_nodeLogicHandlers( nullptr )
{}

RemoteEventsHandlers::~RemoteEventsHandlers()
{
    if( m_pluginEvents )
        delete m_pluginEvents;
    if( m_sceneEvents )
        delete m_sceneEvents;
    if( m_queryEvents )
        delete m_queryEvents;
    if( m_engineStateEvents )
        delete m_engineStateEvents;
    if( m_heightmapEvents )
        delete m_heightmapEvents;
	if( m_assetEvents )
		delete m_assetEvents;
    if( m_timelineHandlers )
        delete m_timelineHandlers;
    if( m_nodeLogicHandlers )
        delete m_nodeLogicHandlers;
}

// ***********************
//
void RemoteEventsHandlers::InitializeHandlers      ( BVAppLogic* appLogic )
{
    m_pluginEvents      = new PluginEventsHandlers( appLogic );
    m_sceneEvents       = new SceneEventsHandlers( appLogic );
    m_queryEvents       = new QueryHandlers( appLogic );
    m_engineStateEvents = new EngineStateHandlers( appLogic );
    m_heightmapEvents   = new HightmapHandlers( appLogic );     // Unused for now.
	m_assetEvents	    = new AssetHandlers( appLogic );
    m_globalEffectEvents= new GlobalEffectHandler( appLogic );
    m_timelineHandlers  = new TimelineHandlers( appLogic );
    m_nodeLogicHandlers = new NodeLogicHandlers( appLogic );

    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_pluginEvents, &PluginEventsHandlers::LoadAsset ), LoadAssetEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_pluginEvents, &PluginEventsHandlers::ParamHandler ), ParamKeyEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_pluginEvents, &PluginEventsHandlers::TimerHandler ), TimerEvent::Type() );

    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_sceneEvents, &SceneEventsHandlers::NodeStructure ), NodeStructureEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_sceneEvents, &SceneEventsHandlers::PluginStructure ), PluginStructureEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_sceneEvents, &SceneEventsHandlers::ProjectStructure ), ProjectEvent::Type() );
	GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_sceneEvents, &SceneEventsHandlers::SceneStructure ), SceneEvent::Type() );

    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_queryEvents, &QueryHandlers::Info ), InfoEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_engineStateEvents, &EngineStateHandlers::RenderingModeEvent ), RenderingModeEvent::Type() );
	GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_assetEvents, &AssetHandlers::CacheHandler ), AssetEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_globalEffectEvents, &GlobalEffectHandler::GlobalEffectEventHandler ), GlobalEffectEvent::Type() );

    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_timelineHandlers, &TimelineHandlers::TimelineKeyframe ), TimelineKeyframeEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_timelineHandlers, &TimelineHandlers::TimelineHandler ), TimeLineEvent::Type() );


    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_nodeLogicHandlers, &NodeLogicHandlers::WidgetHandler ), WidgetEvent::Type() );


	GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_nodeLogicHandlers, &NodeLogicHandlers::OnNodeAppearing ), widgets::NodeAppearingCrawlerEvent::Type() );
	GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_nodeLogicHandlers, &NodeLogicHandlers::OnNodeLeaving ), widgets::NodeLeavingCrawlerEvent::Type() );
}

// ***********************
//
void RemoteEventsHandlers::UpdateHM                ()
{
    m_heightmapEvents->UpdateHM();
}
} //bv
