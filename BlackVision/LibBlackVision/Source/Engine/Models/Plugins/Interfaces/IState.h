#pragma once

//#include "System/BasicTypes.h"

namespace bv { namespace model {

class IStatedValue
{
public:
    virtual bool StateChanged() const = 0;
};

DEFINE_PTR_TYPE(IStatedValue);
DEFINE_CONST_PTR_TYPE(IStatedValue);

} //model
} //bv
