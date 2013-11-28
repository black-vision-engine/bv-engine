#pragma once

#include <string>
#include <sstream>

#include "System/BasicTypes.h"


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

}
