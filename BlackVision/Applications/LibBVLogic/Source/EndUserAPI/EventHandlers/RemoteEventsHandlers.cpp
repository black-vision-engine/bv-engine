#include "stdafxBVApp.h"

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
#include "VideoCardEventsHandlers.h"
#include "GenericEventsHandlers.h"

#include "Engine/Events/EventManager.h"


namespace bv {

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
        m_nodeLogicHandlers( nullptr ),
        m_videoCardHandlers( nullptr ),
        m_genericHandlers( nullptr )
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
    if( m_videoCardHandlers )
        delete m_videoCardHandlers;
    if( m_genericHandlers )
        delete m_genericHandlers;
}

// ***********************
//
void RemoteEventsHandlers::InitializeHandlers      ( BVAppLogic * appLogic )
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
    m_videoCardHandlers = new VideoCardEventsHandlers( appLogic );
    m_genericHandlers   = new GenericEventsHandlers( appLogic );

    
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_pluginEvents, &PluginEventsHandlers::ParamHandler ), ParamKeyEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_pluginEvents, &PluginEventsHandlers::TimerHandler ), TimerEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_pluginEvents, &PluginEventsHandlers::ParamDescHandler ), ParamDescriptorEvent::Type() );

    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_sceneEvents, &SceneEventsHandlers::NodeStructure ), NodeStructureEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_sceneEvents, &SceneEventsHandlers::PluginStructure ), PluginStructureEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_sceneEvents, &SceneEventsHandlers::ProjectStructure ), ProjectEvent::Type() );
	GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_sceneEvents, &SceneEventsHandlers::SceneStructure ), SceneEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_sceneEvents, &SceneEventsHandlers::SceneVariable ), SceneVariableEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_sceneEvents, &SceneEventsHandlers::LightsManagement ), LightEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_sceneEvents, &SceneEventsHandlers::CameraManagement ), CameraEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_sceneEvents, &SceneEventsHandlers::GridLines ), GridLineEvent::Type() );

    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_queryEvents, &QueryHandlers::Info ), InfoEvent::Type() );

    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_engineStateEvents, &EngineStateHandlers::EngineStateHandler ), EngineStateEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_engineStateEvents, &EngineStateHandlers::MouseInteraction ), MouseEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_engineStateEvents, &EngineStateHandlers::ConfigManagment ), ConfigEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_engineStateEvents, &EngineStateHandlers::UndoRedoEvent ), UndoRedoEvent::Type() );

	GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_assetEvents, &AssetHandlers::CacheHandler ), AssetEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_assetEvents, &AssetHandlers::LoadAsset ), LoadAssetEvent::Type() );

    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_globalEffectEvents, &GlobalEffectHandler::GlobalEffectEventHandler ), GlobalEffectEvent::Type() );

    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_timelineHandlers, &TimelineHandlers::TimelineKeyframe ), TimelineKeyframeEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_timelineHandlers, &TimelineHandlers::TimelineHandler ), TimeLineEvent::Type() );

    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_nodeLogicHandlers, &NodeLogicHandlers::WidgetHandler ), NodeLogicEvent::Type() );

    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_videoCardHandlers, &VideoCardEventsHandlers::EventHandler ), VideoCardEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_genericHandlers, &GenericEventsHandlers::EventHandler ), GenericEvent::Type() );
}

// ***********************
//
void RemoteEventsHandlers::UpdateHM                ()
{
    m_heightmapEvents->UpdateHM();
}
} //bv
