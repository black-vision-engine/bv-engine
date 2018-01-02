#include "Framework/FrameworkTest.h"

#include "Engine/Events/Interfaces/IEvent.h"
#include "Engine/Events/Interfaces/IEventManager.h"

#include "Engine/Events/Events.h"
#include "Utils/Events/EndOperationEvent.h"


using namespace bv;



#include "Framework/FrameworkTest.h"


// ***********************
// Bug https://www.pivotaltracker.com/story/show/153778657 replication.
//
// When many asynchronous events come to EventsManager, some of them are lost.
// This is caused by lack of synchronization in some parts of EventsManager. It uses multiple queues
// which are changed each frame. After changing, queue is cleared. This switching is not synchronized
// and some of events can be cleared when they came in bad momement.
//
// This test tries to spam EventsManager with so many events as posible to check if all of them passed.
// This is rather random test as often in multithreading, so it can not always be reliable.
//
// This test works in relase mode. In Debug it passes for some reason :(
class Events_EventsManager_LostEvents : public bv::FrameworkTest
{
    DECALRE_GTEST_INFO_WITH_CONSTRUCTOR( Events_EventsManager_LostEvents )
public:

    virtual void        PreEvents           () override;
    virtual void        PreModelUpdate      () override;

public:

    static bv::IEventPtr        CreateTestEvent     ();
    void                        EventSpammingThread ();
    void                        SpawnThread         ();

    void                        CountEventsHandler  ( bv::IEventPtr evt );
    void                        SpammingThreadEnd   ( bv::IEventPtr evt );

private:

    UInt64              m_eventsCounter;
    std::atomic_bool    m_end;
    UInt64              m_lastSendingFrame;
};
REGISTER_FRAMEWORK_GTEST_INFO( Events_EventsManager_LostEvents, Events_EventsManager, LostEvents )



// ========================================================================= //
// Test implementation
// ========================================================================= //

#ifdef _DEBUG
const Int64 totalEvents = 100000;
const Int64 loopEvents = 10000;
#else
const Int64 totalEvents = 100000;
const Int64 loopEvents = 1000;
#endif


// ***********************
//
void        Events_EventsManager_LostEvents::PreEvents           ()
{
    if( GetFrameNumber() == 0 )
    {
        m_eventsCounter = 0;
        m_end = false;
        
        bv::GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &Events_EventsManager_LostEvents::CountEventsHandler ), bv::ParamKeyEvent::Type() );
        bv::GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &Events_EventsManager_LostEvents::SpammingThreadEnd ), bv::EndOperationEvent::Type() );

        SpawnThread();

        EndTestAfterThisFrame( false );
    }
}

// ***********************
//
void        Events_EventsManager_LostEvents::PreModelUpdate        ()
{
    if( m_end )
    {
        m_lastSendingFrame = GetFrameNumber();
        m_end = false;
    }

    // Wait some frames for the rest of events.
    if( GetFrameNumber() != 0 && GetFrameNumber() == m_lastSendingFrame )
    {
        EXPECT_EQ( m_eventsCounter, totalEvents );

        EndTestAfterThisFrame( true );

        // Remove aded delegate since this class will be destroyed soon.
        bv::GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &Events_EventsManager_LostEvents::CountEventsHandler ), bv::ParamKeyEvent::Type() );
        bv::GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &Events_EventsManager_LostEvents::SpammingThreadEnd ), bv::EndOperationEvent::Type() );
    }
}

// ***********************
//
bv::IEventPtr       Events_EventsManager_LostEvents::CreateTestEvent       ()
{
    auto event = std::make_shared< ParamKeyEvent >();

    event->ParamCommand = ParamKeyEvent::Command::AddKey;
    event->Time = 0.0f;
    event->Value = "0.0f";
    
    event->ParamAddress.SceneName = "TestScene";
    event->ParamAddress.NodeName = "TestNode";
    event->ParamAddress.PluginName = "TestPlugin";
    event->ParamAddress.ParamTargetType = ParameterAddress::TargetType::PluginParam;

    return event;
}

// ***********************
//
void                Events_EventsManager_LostEvents::EventSpammingThread   ()
{
    using namespace std::chrono_literals;

    auto eventsLeft = totalEvents;

    auto event = CreateTestEvent();

    while( eventsLeft > 0 )
    {
        for( Int64 i = 0; i < loopEvents; ++i )
        {
            bv::GetDefaultEventManager().ConcurrentQueueEvent( event );
        }

        eventsLeft -= loopEvents;

        std::this_thread::sleep_for( 8ms );
    }

    // Give engine some time to deal with events.
    std::this_thread::sleep_for( 10000ms );

    // Send notification that all events are send.
    // Since this test checks if events are not lost during processing, we need to send multiple end events
    // to make sure at least one will do the work.
    for( int i = 0; i < 5; ++i )
    {
        bv::GetDefaultEventManager().ConcurrentQueueEvent( std::make_shared< EndOperationEvent >() );

        std::this_thread::sleep_for( 100ms );
    }
}

// ***********************
//
void                Events_EventsManager_LostEvents::SpawnThread            ()
{
    std::thread spammingThread( &Events_EventsManager_LostEvents::EventSpammingThread, this );

    spammingThread.detach();
}

// ***********************
// Events come synchronously in events processing phase.
void                Events_EventsManager_LostEvents::CountEventsHandler    ( bv::IEventPtr evt )
{
    if( evt->GetEventType() == ParamKeyEvent::Type() )
    {
        m_eventsCounter++;
    }
}

// ***********************
//
void                Events_EventsManager_LostEvents::SpammingThreadEnd      ( bv::IEventPtr evt )
{
    m_end = true;
}




