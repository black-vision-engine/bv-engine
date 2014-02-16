#pragma once

#include "Engine/Models/Plugins/Interfaces/IParamSet.h"
#include "Engine/Models/Plugins/Interfaces/IValueSet.h"

namespace bv { namespace model {

class IParamValEvaluator;

class IParamValModel : public IValueSet, public IParamSet
{
public:

    virtual std::vector< IParamValEvaluator * > &   QueryEvaluators     () = 0;

    virtual ~IParamValModel()   {}

};

} //model
} //bv
