#pragma once

#include <memory>

#include "CoreDEF.h"

#include "Engine/Models/Plugins/Interfaces/IParamSet.h"
#include "Engine/Interfaces/IValueSet.h"

namespace bv { 
    
class IValue;

namespace model {

class IParamValEvaluator;
DEFINE_PTR_TYPE(IParamValEvaluator)

class IParamValEvaluator : public IParamSet, public IValueSet
{
public:

    virtual     void    Evaluate                () = 0;

    virtual             ~IParamValEvaluator     () {}

};

} //model
} //bv
