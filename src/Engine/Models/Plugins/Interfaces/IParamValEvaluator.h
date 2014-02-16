#pragma once

#include "Engine/Models/Plugins/Interfaces/IParamSet.h"

namespace bv { namespace model {

class IValue;

class IParamValEvaluator : public IParamSet
{
public:

    virtual IValue *    QueryValue              () = 0;

    virtual             ~IParamValEvaluator     () {}

};

} //model
} //bv
