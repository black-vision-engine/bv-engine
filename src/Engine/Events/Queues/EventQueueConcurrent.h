#include "Core/QueueConcurrent.h"
#include "Engine/Events/Interfaces/IEvent.h"


namespace bv
{

typedef QueueConcurrent< const IEvent * >   EventQueueConcurrent;

EventQueueConcurrent  GetEventQueueConcurrent();

}
