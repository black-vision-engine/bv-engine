#include "RemoteController.h"

#include "Serialization/IDeserializer.h"
#include "Engine/Events/Events.h"

#include "UseLoggerBVAppModule.h"
#include "Serialization/SerializationHelper.h"

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
    RegisterEvent( LightEvent::Name(), LightEvent::Create );
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
    RegisterEvent( MouseEvent::Name(), MouseEvent::Create );
    RegisterEvent( SceneVariableEvent::Name(), SceneVariableEvent::Create );
    RegisterEvent( ConfigEvent::Name(), ConfigEvent::Create );
    RegisterEvent( TabStopEvent::Name(), TabStopEvent::Create );
}


// ***********************
// Returns nullptr if event is unregistered and sends message to log.
IEventPtr RemoteController::DeserializeEvent         ( IDeserializer& deser )
{
    std::string command = deser.GetAttribute( "Event" );
    auto iter = m_eventsConverter.find( command );
    if( iter != m_eventsConverter.end() )
    {
        auto eventCreator = iter->second;
        return eventCreator( deser );
    }
    else
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Unregistered event cannot be deserialized: " + command;
        return nullptr;
    }
}

// ***********************
// Command name and event name are to separate things. Command names
void RemoteController::RegisterEvent       ( const std::string& eventName, EventCreatorDelegate eventCreator )
{
    m_eventsConverter[ eventName ] = eventCreator;
}


}//bv
