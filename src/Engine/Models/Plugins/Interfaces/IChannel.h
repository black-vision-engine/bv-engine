#pragma once

#include "System/BasicTypes.h"
#include "Engine/Interfaces/IUpdatable.h"


namespace bv { namespace model
{

class IChannel : public IUpdatable
{
public:

    virtual bool        IsReadOnly  () const        = 0;

    virtual             ~IChannel   (){};

};

} // model
} // bv
