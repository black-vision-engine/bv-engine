#pragma once

#include "System/BasicTypes.h"

#include "Engine/Models/Plugins/Interfaces/IParamSet.h"
#include "Engine/Models/Plugins/Interfaces/IValueSet.h"

namespace bv { namespace model {

class IValue;

class IParamValEvaluator : public IParamSet, public IValueSet
{
public:

    virtual     void    Evaluate                ( TimeType t ) = 0;

    virtual             ~IParamValEvaluator     () {}

};

} //model
} //bv
