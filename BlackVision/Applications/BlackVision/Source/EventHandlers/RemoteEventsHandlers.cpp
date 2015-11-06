#include "RemoteEventsHandlers.h"

#include "PluginEventsHandlers.h"
#include "SceneEventsHandlers.h"
#include "QueryHandlers.h"
#include "Engine/Events/EventManager.h"

namespace bv
{

// ***********************
//
RemoteEventsHandlers::RemoteEventsHandlers()
    :   m_pluginEvents( nullptr ),
        m_sceneEvents( nullptr ),
        m_queryEvents( nullptr )
{}

RemoteEventsHandlers::~RemoteEventsHandlers()
{
    if( m_pluginEvents )
        delete m_pluginEvents;
    if( m_sceneEvents )
        delete m_sceneEvents;
    if( m_queryEvents )
        delete m_queryEvents;
}

// ***********************
//
void RemoteEventsHandlers::InitializeHandlers      ( BVAppLogic* appLogic )
{
    m_pluginEvents = new PluginEventsHandlers( appLogic );
    m_sceneEvents = new SceneEventsHandlers( appLogic );
    m_queryEvents = new QueryHandlers( appLogic );

    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_pluginEvents, &PluginEventsHandlers::LoadAsset ), LoadAssetEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_pluginEvents, &PluginEventsHandlers::AddParamKey ), ParamKeyEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_sceneEvents, &SceneEventsHandlers::NodeStructure ), NodeStructureEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_sceneEvents, &SceneEventsHandlers::ProjectStructure ), ProjectEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_sceneEvents, &SceneEventsHandlers::TimelineHandler ), TimeLineEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_queryEvents, &QueryHandlers::Info ), NewInfoEvent::Type() );
}

} //bv
