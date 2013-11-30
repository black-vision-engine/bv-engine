#include "Core/Queue.h"
#include "Engine/Events/Interfaces/IEvent.h"


namespace bv
{

typedef Queue< const IEvent * >   EventQueue;

EventQueue  GetEventQueue();

}
