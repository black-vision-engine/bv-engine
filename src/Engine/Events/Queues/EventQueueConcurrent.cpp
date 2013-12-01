#include "EventQueueConcurrent.h"

namespace bv
{

namespace
{
void concurrent_queue_event_deinitializer( const IEvent * evt )
{
    delete evt;
}

}

// *************************************
//
EventQueueConcurrent    GetEventQueueConcurrent ()
{
    return EventQueueConcurrent();
}

// *************************************
//
void                    ClearEventQueue         ( EventQueueConcurrent & queue )
{
    queue.Clear< concurrent_queue_event_deinitializer >();
}

}
