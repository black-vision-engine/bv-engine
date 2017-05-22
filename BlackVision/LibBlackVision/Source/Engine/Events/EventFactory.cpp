#include "stdafx.h"

#include "Engine/Events/Events.h"
#include "EventFactory.h"

#include "Serialization/Json/JsonDeserializeObject.h"

#include "UseLoggerLibBlackVision.h"



#include "Memory/MemoryLeaks.h"



namespace bv
{


// ***********************
//
EventFactory::EventFactory()
{
    RegisterEvent( LoadAssetEvent::Name(), LoadAssetEvent::Create );
    RegisterEvent( ParamKeyEvent::Name(), ParamKeyEvent::Create );
    RegisterEvent( NodeStructureEvent::Name(), NodeStructureEvent::Create );
    RegisterEvent( ProjectEvent::Name(), ProjectEvent::Create );
    RegisterEvent( SceneEvent::Name(), SceneEvent::Create );
    RegisterEvent( LightEvent::Name(), LightEvent::Create );
    RegisterEvent( CameraEvent::Name(), CameraEvent::Create );
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
    RegisterEvent( GridLineEvent::Name(), GridLineEvent::Create );
    RegisterEvent( ParamDescriptorEvent::Name(), ParamDescriptorEvent::Create );
    RegisterEvent( UndoRedoEvent::Name(), UndoRedoEvent::Create );
    RegisterEvent( GenericEvent::Name(), GenericEvent::Create );
}

// ***********************
//
EventFactory::~EventFactory()
{}

// ***********************
// Command name and event name are to separate things.
void EventFactory::RegisterEvent       ( const std::string& eventName, EventCreatorDelegate eventCreator )
{
    m_eventsConverter[ eventName ] = eventCreator;
}

// ***********************
// Returns nullptr if event is unregistered and sends message to log.
IEventPtr EventFactory::DeserializeEvent         ( IDeserializer& deser ) const
{
    std::string command = deser.GetAttribute( "Event" );
    auto iter = m_eventsConverter.find( command );
    if( iter != m_eventsConverter.end() )
    {
        auto eventCreator = iter->second;
        auto event = eventCreator( deser );
        
        if( !event )
            LOG_MESSAGE( SeverityLevel::error ) << "Cannot deserialize event: " + command;

        return event;
    }
    else
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Unregistered event cannot be deserialized: " + command;
        return nullptr;
    }
}

// ***********************
//
std::vector< IEventPtr >        EventFactory::ParseEventsList( IDeserializer & deser, int socketID ) const
{
    if( !deser.EnterChild( "Events" ) )
        return std::vector< IEventPtr >();

    std::vector< IEventPtr >    result;

    if( deser.EnterChild( "Event" ) )
    {
        do
        {
            auto newEvent = DeserializeEvent( deser );
            if( newEvent != nullptr )
            {
                RemoteEventPtr newEventBase = std::static_pointer_cast< RemoteEvent >( newEvent );
                newEventBase->SocketID = socketID;
                newEventBase->EventID = SerializationHelper::String2T( deser.GetAttribute( "EventID" ), std::numeric_limits< int >::max() );

                result.push_back( newEventBase );
            }

        } while( deser.NextChild() );

        deser.ExitChild();  // Event
    }
        
    deser.ExitChild();  // Events

    return result;
}

// ***********************
//
std::vector< IEventPtr >        EventFactory::ParseEventsListJSON ( const std::string & eventString, int socketID ) const
{
    JsonDeserializeObject deser;

    if( !deser.Load( eventString ) )
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Can't parse command: \n" + eventString;
        return std::vector< IEventPtr >();
    }

    return ParseEventsList( deser, socketID );
}

}	// bv