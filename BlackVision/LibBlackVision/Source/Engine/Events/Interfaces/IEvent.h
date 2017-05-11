#pragma once

#include <memory>
#include <string>
#include <sstream>

#include "CoreDEF.h"

#pragma warning( push )
#pragma warning( disable : 4100 )

#include "FastDelegate.h"

#pragma warning( pop )

#include "Serialization/ISerializable.h"

namespace bv 
{

typedef unsigned long EventType;

class ISerializer;
class IDeserializer;

class IEvent;
DEFINE_PTR_TYPE(IEvent)

class IEvent : public ISerializable
{
public:

    virtual                         ~IEvent         () {}

    virtual IEventPtr               Clone           () const                            = 0;

    virtual EventType               GetEventType    () const                            = 0;
    virtual TimeType                GetTimeStamp    () const                            = 0;

    virtual const std::string &     GetName         () const                            = 0;

};

typedef fastdelegate::FastDelegate1<IEventPtr>  EventListenerDelegate;

template< class EventTyped >
std::shared_ptr< EventTyped > QueryTypedEvent( IEventPtr evt )
{
    assert( EventTyped::Type() == evt->GetEventType() );
    return std::static_pointer_cast< EventTyped >( evt );
}

}
