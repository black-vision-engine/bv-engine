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

    IEventPtr   DeserializeEvent    ( IDeserializer& deser ) const;

private:
    void        RegisterEvent       ( const std::string& eventName, EventCreatorDelegate eventCreator );
};


}	// bv
