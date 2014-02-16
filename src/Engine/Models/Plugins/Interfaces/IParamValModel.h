#pragma once

#include "Engine/Interfaces/IUpdatable.h"

#include "Engine/Models/Plugins/Interfaces/IParamSet.h"
#include "Engine/Models/Plugins/Interfaces/IValueSet.h"


namespace bv { namespace model {

class IParamValEvaluator;

class IParamValModel : public IValueSet, public IParamSet, public IUpdatable
{
public:

    virtual std::vector< IParamValEvaluator * > &   GetEvaluators       () = 0;

    virtual ~IParamValModel()   {}

};

} //model
} //bv
