#include "stdafx.h"

#include "EventQueue.h"


namespace bv
{


// *************************************
//
EventQueue  GetEventQueue   ()
{
    return EventQueue();
}

// *************************************
//
bool        RemoveEventsOfType  ( EventQueue & queue, EventType type, bool allOfType )
{
    EventList tmp;

    while( !queue.IsEmpty() )
    {
        auto evt = queue.Front();
        queue.Pop();

        tmp.push_back( evt );
    }

    bool oneSkipped = false;
    bool removed = false;

    for( auto evt : tmp )
    {
        if( evt->GetEventType() != type )
        {
            queue.Push( evt );
        }
        else
        {
            if( !allOfType )
            {
                if( !oneSkipped )
                {
                    oneSkipped = true;
                }
                else
                {
                    queue.Push( evt );
                }
            }
        }
    }

    return removed || oneSkipped;
}

} //bv
