#pragma once

#include "Engine/Interfaces/IUpdatable.h"

#include "Engine/Models/Plugins/Interfaces/IParamSet.h"
#include "Engine/Interfaces/IValueSet.h"


namespace bv { namespace model {

class IParamValEvaluator;

class IParamValModel : public IParamSet, public IValueSet, public IUpdatable
{
public:

    virtual std::vector< IParamValEvaluator * > &   GetEvaluators       () = 0;

    virtual ~IParamValModel()   {}

};

} //model
} //bv
