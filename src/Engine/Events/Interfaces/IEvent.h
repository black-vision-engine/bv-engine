#pragma once

#include <memory>
#include <string>
#include <sstream>

#include "System/BasicTypes.h"

#include "FastDelegate.h"


namespace bv 
{

typedef unsigned long EventType;

class IEvent
{
public:

    virtual                         ~IEvent         () {}

    virtual IEvent *                Clone           () const                            = 0;

    virtual EventType               GetEventType    () const                            = 0;
    virtual TimeType                GetTimeStamp    () const                            = 0;

    virtual void                    Serialize       ( std::ostringstream & out ) const  = 0;
    virtual void                    Deserialize     ( std::istringstream & in )         = 0;

    virtual const std::string &     GetName         () const                            = 0;

};

typedef std::shared_ptr<IEvent>                 IEventPtr;
typedef fastdelegate::FastDelegate1<IEventPtr>  EventListenerDelegate;

}
