#include "Framework/FrameworkTest.h"

#include "Engine/Events/EventManager.h"
#include "Engine/Events/Interfaces/IEvent.h"

#include "Engine/Events/InnerEvents/InputSlots/SlotRemovedEvent.h"
#include "Engine/Events/InnerEvents/InputSlots/FirstSlotRefEvent.h"
#include "Engine/Events/InnerEvents/InputSlots/SlotAddedEvent.h"
#include "Engine/Events/InnerEvents/InputSlots/AllSlotRefsRemovedEvent.h"

#include "Engine/Graphics/InputSlots/InputSlots.h"

#include "Helpers/TextureHelpers.h"
#include "Helpers/InputAssetHelpers.h"

#include <vector>


// ========================================================================= //
// Handlers
// ========================================================================= //


namespace bv
{

// ***********************
//
enum class SlotEventType
{
    FirstRef,
    AllRefsRemoved,
    SlotAdded,
    SlotRemoved
};

// ***********************
//
struct EventCapture
{
    SlotEventType       Type;
    std::string         Name;
    SlotIndex           Idx;
};


std::vector< EventCapture >     EventCaptureQueue;


// ***********************
//
void            PushEvent                   ( SlotEventType type, IEventPtr evt )
{
    EventCapture captured;
    captured.Type = type;
    captured.Idx = std::static_pointer_cast< SlotEventBase >( evt )->Index;
    captured.Name = std::static_pointer_cast< SlotEventBase >( evt )->Name;

    EventCaptureQueue.push_back( captured );
}

// ***********************
//
void            FirstReferenceHandler       ( IEventPtr evt )
{
    if( evt->GetEventType() == FirstSlotRefEvent::Type() )
    {
        PushEvent( SlotEventType::FirstRef, evt );
    }
}

// ***********************
//
void            AllRefsRemovedHandler       ( IEventPtr evt )
{
    if( evt->GetEventType() == AllSlotRefsRemovedEvent::Type() )
    {
        PushEvent( SlotEventType::AllRefsRemoved, evt );
    }
}

// ***********************
//
void            SlotAddedHandler            ( IEventPtr evt )
{
    if( evt->GetEventType() == SlotAddedEvent::Type() )
    {
        PushEvent( SlotEventType::SlotAdded, evt );
    }
}

// ***********************
//
void            SlotRemovedHandler          ( IEventPtr evt )
{
    if( evt->GetEventType() == SlotRemovedEvent::Type() )
    {
        PushEvent( SlotEventType::SlotRemoved, evt );
    }
}

}	// bv


// ========================================================================= //
// Test cases
// ========================================================================= //


using namespace bv;

// ***********************
// Adding first reference should send FirstSlotRefEvent. Second reference does nothing.
POST_RENDER_FRAMEWORK_TEST( Engine_InputSlots, Events_FirstReference )
{
    static auto context = CreateInputContext();
    static Expected< SlotIndex > slotIdx = 0;

    if( GetFrameNumber() == 0 )
    {
        EventCaptureQueue.clear();
        GetDefaultEventManager().AddListener( &FirstReferenceHandler, FirstSlotRefEvent::Type() );

        slotIdx = CreateSlot( context, "Source1" );

        ASSERT_TRUE( slotIdx.IsValid() );

        // This should send event.
        context.slots->ReferenceSource( slotIdx );

        EndTestAfterThisFrame( false );
    }
    else if( GetFrameNumber() == 1 )
    {
        EXPECT_EQ( EventCaptureQueue.size(), 1 ) << "First reference doesn't send event";
        
        // Clear events before next loop.
        EventCaptureQueue.clear();

        // This shouldn't send event.
        context.slots->ReferenceSource( slotIdx );
    }
    else
    {
        EXPECT_TRUE( EventCaptureQueue.empty() ) << "Second reference to slot sends event.";

        GetDefaultEventManager().RemoveListener( &FirstReferenceHandler, FirstSlotRefEvent::Type() );
        ClearContext( context );
        EndTestAfterThisFrame( true );
    }
}
