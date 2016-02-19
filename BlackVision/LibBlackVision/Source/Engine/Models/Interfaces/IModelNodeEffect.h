#pragma once

#include "CoreDEF.h"
#include "Engine/Interfaces/IUpdatable.h"
#include "Engine/Models/Plugins/Interfaces/IParameter.h"
#include "Engine/Interfaces/IValue.h"
#include "Engine/Graphics/Effects/NodeEffect/NodeEffect.h"
#include "Engine/Models/Plugins/Interfaces/IParamValEvaluator.h"


namespace bv { namespace model {

class IModelNodeEffect : public bv::IUpdatable
{
public:

    virtual                                         ~IModelNodeEffect       () {};

    virtual void                                    Update                  ( TimeType t ) = 0;
    virtual NodeEffectType			                GetType			        () const = 0;

    virtual void                                    RegisterEvaluator       ( IParamValEvaluatorPtr paramEval ) = 0;
    virtual IParameterPtr                           GetParameter            ( const std::string & name ) const = 0;
    virtual const std::vector< IParameterPtr > &    GetParameters           () const = 0;
    virtual const std::vector< IValueConstPtr > &   GetValues               () const = 0;

};

DEFINE_PTR_TYPE(IModelNodeEffect)
DEFINE_CONST_PTR_TYPE(IModelNodeEffect)

} // model
} // bv
