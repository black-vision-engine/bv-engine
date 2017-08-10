#pragma once

#include "Engine/Events/Interfaces/IEvent.h"
#include <unordered_map>



namespace bv
{

typedef IEventPtr (*EventCreatorDelegate)( IDeserializer& deser );


class EventFactory
{
private:

    std::unordered_map< std::string, EventCreatorDelegate >      m_eventsConverter;

public:
    EventFactory    ();
    ~EventFactory   ();

    IEventPtr                       DeserializeEvent    ( IDeserializer& deser ) const;
    std::vector< IEventPtr >        ParseEventsList     ( IDeserializer & deser, int socketID ) const;
    std::vector< IEventPtr >        ParseEventsListJSON ( const std::string & eventString, int socketID ) const;

    void                            RegisterEvent       ( const std::string& eventName, EventCreatorDelegate eventCreator );
};


}	// bv
