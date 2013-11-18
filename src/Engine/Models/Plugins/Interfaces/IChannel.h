#pragma once

#include "System/BasicTypes.h"


namespace bv { namespace model
{

class IChannel
{
public:

    virtual void        Update      ( TimeType t )  = 0;
    virtual bool        IsReadOnly  () const        = 0;

    virtual             ~IChannel   (){};

};

} // model
} // bv
