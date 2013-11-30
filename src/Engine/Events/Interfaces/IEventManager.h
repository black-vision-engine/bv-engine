#pragma once

#include "Engine/Events/Interfaces/IEvent.h"


namespace bv 
{

class IEventManager
{
public:

    static const unsigned int millisINFINITE = 0xffffffff;

public:

    virtual ~IEventManager                  () {};

    virtual bool    AddListener             ( const EventListenerDelegate & eventDelegate, const EventType & type ) = 0;
    virtual bool    RemoveListener          ( const EventListenerDelegate & eventDelegate, const EventType & type ) = 0;

    virtual bool    TriggerEvent            ( const IEvent * evt ) const = 0;
    virtual bool    QueueEvent              ( const IEvent * evt ) = 0;
    virtual bool    ConcurrentQueueEvent    ( const IEvent * evt ) = 0;

    virtual bool    AbortEvent              ( const EventType & type, bool allOfType = false ) = 0;

    virtual bool    Update                  ( unsigned long maxMillis = millisINFINITE ) = 0;

};

}
