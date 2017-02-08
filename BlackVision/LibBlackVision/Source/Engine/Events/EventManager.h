#pragma once

#include <list>
#include <hash_map>

#include "Engine/Events/Interfaces/IEventManager.h"


#include "Engine/Events/Queues/EventQueue.h"
#include "Engine/Events/Queues/EventQueueConcurrent.h"


namespace bv
{

class EventManager : public IEventManager
{
private:

    typedef std::list<EventListenerDelegate>            EventListenerList;
    typedef std::hash_map<EventType, EventListenerList> EventListenerMap;

    static const unsigned int NUM_QUEUES = 2;
    static const unsigned int NUM_CONCURRENT_QUEUES = 2;

private:

    EventFactory            m_eventFactory;

    EventListenerMap        m_eventListeners;

    EventQueue              m_queues[ NUM_QUEUES ];
    EventQueueConcurrent    m_concurrentQueues[ NUM_CONCURRENT_QUEUES ];

    int m_activeQueue;
    int m_activeconcurrentQueue;
    int m_numLockedFrames;          // Locks processing events for m_numLockedFrames frames.

public:

    explicit        EventManager            ();
    virtual         ~EventManager           ();

    virtual bool    AddListener             ( const EventListenerDelegate & eventDelegate, const EventType & type );
    virtual bool    RemoveListener          ( const EventListenerDelegate & eventDelegate, const EventType & type );

    virtual bool    TriggerEvent            ( const IEventPtr & evt ) const;
    virtual bool    QueueEvent              ( const IEventPtr & evt );
    virtual bool    ConcurrentQueueEvent    ( const IEventPtr & evt );

    virtual bool    AbortEvent              ( const EventType & type, bool allOfType = false );

    virtual bool    Update                  ( unsigned long maxEvaluationMillis = millisINFINITE );

    virtual void    QueueResponse           ( const IEventPtr evt ) override;

    virtual void    LockEvents              ( unsigned int numFrames ) override;
    

    static EventType                        RegisterType    ();
    virtual const EventFactory &            GetEventFactory () override;
};

} //bv
