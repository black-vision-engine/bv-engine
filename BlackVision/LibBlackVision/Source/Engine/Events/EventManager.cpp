#include "stdafx.h"

#include "EventManager.h"

#include "System/Time.h"

#include <cassert>
#include <list>
#include <chrono>



#include "Memory/MemoryLeaks.h"



namespace bv
{

// *******************************
//
EventManager::EventManager                  ()
    : m_activeQueue( 0 )
    , m_activeconcurrentQueue( 0 )
    , m_numLockedFrames( 0 )
{
}

// *******************************
//
EventManager::~EventManager                 ()
{
    printf( "Default Event Manager shutdown\n" );
}

// *******************************
//
bool    EventManager::AddListener           ( const EventListenerDelegate & eventDelegate, const EventType & type )
{
    EventListenerList & ell = m_eventListeners[ type ];

    for( auto listener : ell )
    {
        if( eventDelegate == listener )
        {
            return false;
        }
    }

    ell.push_back( eventDelegate );

	return true;
}

// *******************************
//
bool    EventManager::RemoveListener        ( const EventListenerDelegate & eventDelegate, const EventType & type )
{
    bool bOK = false;

    auto listenersIt = m_eventListeners.find( type );

    if( listenersIt != m_eventListeners.end() )
    {
        EventListenerList & listeners = listenersIt->second;

        for( auto it = listeners.begin(); it != listeners.end(); ++it )
        {
            if ( eventDelegate == ( *it ) )
            {
                listeners.erase( it );
                bOK = true;

                break;
            }
        }
    }

    return bOK;
}

// *******************************
//
bool    EventManager::TriggerEvent          ( const IEventPtr & evt ) const
{
    bool bOK = false;

    auto listenersIt = m_eventListeners.find( evt->GetEventType() );

	if( listenersIt != m_eventListeners.end() )
    {
        for( auto listener : listenersIt->second )
        {
            listener( evt );
            bOK = true;
        }
    }
	
	return bOK;
}

// *******************************
//
bool    EventManager::QueueEvent            ( const IEventPtr & evt )
{
	assert( m_activeQueue >= 0 );
    assert( m_activeQueue < NUM_QUEUES );

    assert( evt );

    auto listenersIt = m_eventListeners.find( evt->GetEventType() );

    if( listenersIt != m_eventListeners.end() )
    {
        m_queues[ m_activeQueue ].Push( evt );

        return true;
    }

    return false;
}

// *******************************
//
bool    EventManager::ConcurrentQueueEvent  ( const IEventPtr & evt )
{
	assert( m_activeconcurrentQueue >= 0 );
    assert( m_activeconcurrentQueue < NUM_CONCURRENT_QUEUES );

    assert( evt );

    auto listenersIt = m_eventListeners.find( evt->GetEventType() );

    if( listenersIt != m_eventListeners.end() )
    {
        m_concurrentQueues[ m_activeQueue ].Push( evt );

        return true;
    }

    return false;
}

// *******************************
//FIXME: also from threaded queues
bool    EventManager::AbortEvent            ( const EventType & type, bool allOfType )
{
	assert( m_activeQueue >= 0 );
    assert( m_activeQueue < NUM_QUEUES );
	assert( m_activeconcurrentQueue >= 0 );
    assert( m_activeconcurrentQueue < NUM_CONCURRENT_QUEUES );

    bool bRemoved = false;

    EventListenerMap::iterator listenersIt = m_eventListeners.find( type );

    if ( listenersIt != m_eventListeners.end() )
    {
        EventQueue & eventQueue = m_queues[ m_activeQueue ];

        bRemoved = RemoveEventsOfType( eventQueue, type, allOfType );         
    }

	return bRemoved;
}

// *******************************
//
bool    EventManager::Update                ( unsigned long maxEvaluationMillis )
{
    auto curMillis = Time::Now();
    auto maxMillis = ( ( maxEvaluationMillis == IEventManager::millisINFINITE ) ? ( IEventManager::millisINFINITE ) : ( curMillis + maxEvaluationMillis ) );

    if( m_numLockedFrames > 0 )
        m_numLockedFrames--;

    //Multithreaded part
    unsigned int activeConcurrentQueue = m_activeconcurrentQueue;
    m_activeconcurrentQueue = ( m_activeconcurrentQueue + 1 ) % NUM_CONCURRENT_QUEUES;

	m_concurrentQueues[ m_activeconcurrentQueue ].Clear();

	IEventPtr  concurrentEvent;

    while ( m_concurrentQueues[ activeConcurrentQueue ].TryPop( concurrentEvent ) )
    {
        QueueEvent( concurrentEvent );
        
        curMillis = Time::Now();

        if ( maxEvaluationMillis != IEventManager::millisINFINITE && curMillis >= maxMillis )
        {
            assert( !"Concurrent events are DOSing event manager" );
        }
    }

    //Regular events
    unsigned int activeQueue = m_activeQueue;
    m_activeQueue = ( m_activeQueue + 1 ) % NUM_QUEUES;

    m_queues[ m_activeQueue ].Clear();

    while( !m_queues[ activeQueue ].IsEmpty() && m_numLockedFrames == 0 )
    {
        IEventPtr evt = m_queues[ activeQueue ].Front();
        m_queues[ activeQueue ].Pop();

        EventType eventType = evt->GetEventType();

        auto listenersIt = m_eventListeners.find( eventType );

        for( auto listener : listenersIt->second )
        {
            listener( evt );
        }

        curMillis = Time::Now();

        if ( maxEvaluationMillis != IEventManager::millisINFINITE && curMillis >= maxMillis )
        {
            break;
        }
    }
	
    EventList tmp;

    bool pendingEvents = !m_queues[ activeQueue ].IsEmpty();

    if( pendingEvents )
    {
        while ( !m_queues[ activeQueue ].IsEmpty() )
        {
            auto evt = m_queues[ activeQueue ].Front();
            m_queues[ activeQueue ].Pop();

            tmp.push_back( evt );
	    }
	
        while ( !m_queues[ m_activeQueue ].IsEmpty() )
        {
            auto evt = m_queues[ m_activeQueue ].Front();
            m_queues[ activeQueue ].Pop();

            tmp.push_back( evt );
        }

        for( auto evt : tmp )
        {
            m_queues[ m_activeQueue ].Push( evt );
        }
    }

    return pendingEvents;
}

// *******************************
//
void EventManager::QueueResponse       ( const IEventPtr evt )
{
    ConcurrentQueueEvent( evt );
}


void EventManager::LockEvents           ( unsigned int numFrames )
{
    if( numFrames > 0 )
        m_numLockedFrames = numFrames;
}


//FIXME: hack - should be created by means of Engine object or some global object responsible for application state and services
IEventManager &     GetDefaultEventManager  ()
{
    static EventManager instance;
    static bool initialized = false;

    if( !initialized )
    {
        initialized = true;

        printf( "Default Event Manager initialized\n" );
    }

    return instance;
}

// ***********************
//
EventType           EventManager::RegisterType()
{
    static EventType nextType = 0x0001;
    return nextType++;
}

// ***********************
//
const EventFactory &            EventManager::GetEventFactory()
{
    return m_eventFactory;
}

} //bv
