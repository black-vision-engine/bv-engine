#include "stdafx.h"

#include "ProcessThreadedEvents.h"

#include <cassert>
#include <thread>
#include <chrono>

#include "Engine/Events/Interfaces/IEventManager.h"

namespace bv
{

// *******************************
//
ProcessThreadedEventSender::ProcessThreadedEventSender  ( IEventManager * eventManager )
    : m_quit( false )
    , m_eventManager( eventManager )
{
    assert( eventManager );
}

// *******************************
//
void ProcessThreadedEventSender::Run                    ()
{
    while( !m_quit )
    { 
        IEventPtr e; //FIXME: some concrete event
        m_eventManager->ConcurrentQueueEvent( e );
        std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) ); //FIXME: wait for another event to be sent
    } 

    Succeed();
}


// *******************************
//
ProcessThreadedEventReceiver::ProcessThreadedEventReceiver    ( IEventManager * eventManager )
    : m_quit( false )
{
    eventManager->AddListener( fastdelegate::MakeDelegate( this, &ProcessThreadedEventReceiver::DelegateFun), 0 ); //FIXME: add valid event type here
}

// *******************************
//
void ProcessThreadedEventReceiver::DelegateFun                ( IEventPtr evt )
{
	m_eventCache.Push( evt );
}


// *******************************
//
void ProcessThreadedEventReceiver::Run                ()
{
    while( !m_quit )
    {
        IEventPtr e;	
		
        if ( m_eventCache.TryPop( e ) )
        {
            //FIXME: do something with the event
        }
        else
        {
            std::this_thread::sleep_for( std::chrono::milliseconds( 2 ) ); //FIXME: wait for other events
        }
    }

    Succeed();

    //FIXME: deregister delegate - or some other finalization method
}

} //bv

