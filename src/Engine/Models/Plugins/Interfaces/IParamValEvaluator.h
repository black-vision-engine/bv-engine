#pragma once

#include "Engine/Models/Plugins/Interfaces/IParamSet.h"

namespace bv { namespace model {

class IValue;

class IParamValEvaluator : public IParamSet
{
public:

    virtual IValue *    GetValue                () = 0;

    virtual             ~IParamValEvaluator     () {}

};

} //model
} //bv
