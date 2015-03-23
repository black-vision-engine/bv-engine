#pragma once

#include "System/BasicTypes.h"

namespace bv { namespace model {

class IState
{
public:
    virtual bool StateChanged() const = 0;
};

DEFINE_PTR_TYPE(IState)

} //model
} //bv
