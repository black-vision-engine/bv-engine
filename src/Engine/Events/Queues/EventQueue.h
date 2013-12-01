#include "Core/Queue.h"

#include <list>

#include "Engine/Events/Interfaces/IEvent.h"


namespace bv
{

typedef Queue< const IEvent * >     EventQueue;
typedef std::list< const IEvent * > EventList;

EventQueue  GetEventQueue       ();
void        ClearEventQueue     ( EventQueue & queue );
bool        RemoveEventsOfType  ( EventQueue & queue, EventType type, bool allOfType );

} //bv
