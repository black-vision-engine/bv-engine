#include "RemoteController.h"

#include "Serialization/IDeserializer.h"
#include "Engine/Events/Events.h"

#include "../UseLoggerBVAppModule.h"
#include "Engine/Events/EventHelpers.h"

namespace bv
{

// ***********************
//
RemoteController::RemoteController()
{
    RegisterEvent( LoadAssetEvent::Name(), LoadAssetEvent::Create );
    RegisterEvent( ParamKeyEvent::Name(), ParamKeyEvent::Create );
    RegisterEvent( NodeStructureEvent::Name(), NodeStructureEvent::Create );
    RegisterEvent( ProjectEvent::Name(), ProjectEvent::Create );
    RegisterEvent( SceneEvent::Name(), SceneEvent::Create );
    RegisterEvent( InfoEvent::Name(), InfoEvent::Create );
    RegisterEvent( TimeLineEvent::Name(), TimeLineEvent::Create );
    RegisterEvent( TimerEvent::Name(), TimerEvent::Create );
    RegisterEvent( PluginStructureEvent::Name(), PluginStructureEvent::Create );
    RegisterEvent( EngineStateEvent::Name(), EngineStateEvent::Create );
    RegisterEvent( VideoCardEvent::Name(), VideoCardEvent::Create );
    RegisterEvent( NodeLogicEvent::Name(), NodeLogicEvent::Create );
    RegisterEvent( HightmapEvent::Name(), HightmapEvent::Create );
    RegisterEvent( AssetEvent::Name(), AssetEvent::Create );
    RegisterEvent( GlobalEffectEvent::Name(), GlobalEffectEvent::Create );
    RegisterEvent( TimelineKeyframeEvent::Name(), TimelineKeyframeEvent::Create );
}


// ***********************
// Returns nullptr if event is unregistered and sends message to log.
IEventPtr RemoteController::DeserializeEvent         ( IDeserializer& deser )
{
    std::wstring command = deser.GetAttribute( L"Event" );
    auto iter = m_eventsConverter.find( command );
    if( iter != m_eventsConverter.end() )
    {
        auto eventCreator = iter->second;
        return eventCreator( deser );
    }
    else
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Unregistered event cannot be deserialized: " + toString( command );
        return nullptr;
    }
}

// ***********************
// Command name and event name are to separate things. Command names
void RemoteController::RegisterEvent       ( const std::string& eventName, EventCreatorDelegate eventCreator )
{
    std::wstring commandName = std::wstring( eventName.begin(), eventName.end() );
    m_eventsConverter[ commandName ] = eventCreator;
}


}//bv
