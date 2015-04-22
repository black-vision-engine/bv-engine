#pragma once

#include "Engine/Models/Plugins/Interfaces/IParamSet.h"
#include "Engine/Models/Plugins/Interfaces/IParamValEvaluator.h"
#include "Engine/Models/Plugins/Interfaces/IStateSet.h"
#include "Engine/Interfaces/IValueSet.h"


namespace bv { namespace model {

class IParamValModel;
DEFINE_PTR_TYPE(IParamValModel)

class IParamValModel : public IParamSet, public IValueSet, public IStateSet
{
public:

    virtual std::vector< IParamValEvaluatorPtr > &  GetEvaluators       () = 0;
    virtual void                                    Update              () = 0;

    virtual ~IParamValModel()   {}

};

} //model
} //bv
