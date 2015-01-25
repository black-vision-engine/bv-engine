#include "Core/QueueConcurrent.h"
#include "Engine/Events/Interfaces/IEvent.h"


namespace bv
{

typedef QueueConcurrent< IEventPtr >   EventQueueConcurrent;

EventQueueConcurrent    GetEventQueueConcurrent ();

}
