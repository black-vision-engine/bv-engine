#include "EventManager.h"

#include <cassert>


namespace bv
{

// *******************************
//
EventManager::EventManager                  ()
    : m_activeQueue( 0 )
    , m_activeconcurrentQueue( 0 )
{
}

// *******************************
//
EventManager::~EventManager                 ()
{
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
            if ( eventDelegate == (*it) )
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
bool    EventManager::TriggerEvent          ( const IEvent * evt ) const
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
bool    EventManager::QueueEvent            ( const IEvent * evt )
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
bool    EventManager::ConcurrentQueueEvent  ( const IEvent * evt )
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
//FIXME: concurrent queues as well
bool    EventManager::RemoveEvent           ( const EventType & type, bool allOfType )
{
	assert( m_activeQueue >= 0 );
    assert( m_activeQueue < NUM_QUEUES );
	assert( m_activeconcurrentQueue >= 0 );
    assert( m_activeconcurrentQueue < NUM_CONCURRENT_QUEUES );

    bool bOK = false;

    EventListenerMap::iterator listenersIt = m_eventListeners.find( type );

    if ( listenersIt != m_eventListeners.end() )
    {
        EventQueue & eventQueue = m_queues[ m_activeQueue ];
        
        //eventQueue->Remove( evt );
        //FIXME: implement
    }

	return false;
}

// *******************************
//
bool    EventManager::Update                ( unsigned long maxMillis )
{
    //FIXME: implement

    return false;
}

} //bv
