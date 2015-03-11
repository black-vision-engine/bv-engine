#include "DataTypes/Queue.h"

#include <list>

#include "Engine/Events/Interfaces/IEvent.h"


namespace bv
{

typedef Queue< IEventPtr >     EventQueue;
typedef std::list< IEventPtr > EventList;

EventQueue  GetEventQueue       ();
bool        RemoveEventsOfType  ( EventQueue & queue, EventType type, bool allOfType );

} //bv
