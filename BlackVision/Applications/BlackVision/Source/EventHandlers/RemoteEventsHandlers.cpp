#include "RemoteEventsHandlers.h"

#include "PluginEventsHandlers.h"
#include "SceneEventsHandlers.h"
#include "Engine/Events/EventManager.h"

namespace bv
{

// ***********************
//
RemoteEventsHandlers::RemoteEventsHandlers()
    :   m_pluginEvents( nullptr ),
        m_sceneEvents( nullptr )
{}

RemoteEventsHandlers::~RemoteEventsHandlers()
{
    if( m_pluginEvents )
        delete m_pluginEvents;
    if( m_sceneEvents )
        delete m_sceneEvents;
}

// ***********************
//
void RemoteEventsHandlers::InitializeHandlers      ( BVAppLogic* appLogic )
{
    m_pluginEvents = new PluginEventsHandlers( appLogic );
    m_sceneEvents = new SceneEventsHandlers( appLogic );

    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( m_pluginEvents, &PluginEventsHandlers::LoadAsset ), LoadAssetEvent::Type() );
}

} //bv
