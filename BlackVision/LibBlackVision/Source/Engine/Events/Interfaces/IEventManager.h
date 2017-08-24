#pragma once

#include "Engine/Events/Interfaces/IEvent.h"
#include "Engine/Events/EventFactory.h"

namespace bv 
{

class IEventManager
{
public:

    static const unsigned int millisINFINITE = 0xffffffff;

public:

    virtual         ~IEventManager          () {};

    virtual bool    AddListener             ( const EventListenerDelegate & eventDelegate, const EventType & type ) = 0;
    virtual bool    RemoveListener          ( const EventListenerDelegate & eventDelegate, const EventType & type ) = 0;

    virtual bool    TriggerEvent            ( const IEventPtr & evt ) const                                         = 0;
    virtual bool    QueueEvent              ( const IEventPtr & evt )                                               = 0;
    virtual bool    ConcurrentQueueEvent    ( const IEventPtr & evt )                                               = 0;

    virtual bool    AbortEvent              ( const EventType & type, bool allOfType = false )                      = 0;

    virtual bool    Update                  ( unsigned long maxEvaluationMillis = millisINFINITE )                  = 0;

    virtual void    QueueResponse           ( const IEventPtr evt )                                                 = 0;

    virtual void    LockEvents              ( unsigned int numFrames )                                              = 0;

    virtual const EventFactory &            GetEventFactory()                                                       = 0; 


    virtual EventType                       RegisterEvent   ( const std::string& eventName, EventCreatorDelegate eventCreator ) = 0;
};

//FIXME: hack - should be created by means of Engine object or some global object responsible for application state and services
IEventManager &     GetDefaultEventManager  ();

} //bv
