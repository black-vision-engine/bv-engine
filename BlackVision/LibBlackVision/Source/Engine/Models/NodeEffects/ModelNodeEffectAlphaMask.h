#pragma once

#include "Engine/Models/Interfaces/IModelNodeEffect.h"

#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"


namespace bv { namespace model {

class ModelNodeEffectAlphaMask : public IModelNodeEffect
{
private:

    ParamFloatPtr       m_paramAlpha;
    
    float               m_alpha;

public:

                            ModelNodeEffectAlphaMask( ITimeEvaluatorPtr timeEvaluator );

    virtual NodeEffectType  GetType                 () const override;

    ParamFloatPtr           GetParamAlpha           ();

    virtual void            Update                  ( TimeType t ) override;

    float                   GetAlpha                () const;

};

DEFINE_PTR_TYPE(ModelNodeEffectAlphaMask)
DEFINE_CONST_PTR_TYPE(ModelNodeEffectAlphaMask)

} // model
} // bv
