#pragma once

#include "Engine/Models/NodeEffects/ModelNodeEffectBase.h"

#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"

namespace bv { namespace model {

class ModelNodeEffectAlphaMask : public ModelNodeEffectBase
{
private:

    ParamFloatPtr   m_paramAlpha;

    float           m_alphaVal;

public:

                            ModelNodeEffectAlphaMask( ITimeEvaluatorPtr timeEvaluator );

    virtual NodeEffectType  GetType                 () const override;

	virtual void			Update					( TimeType t ) override;

	ParamFloatPtr           GetParamAlpha           () const;

    float                   GetAlpha                () const;

};

DEFINE_PTR_TYPE(ModelNodeEffectAlphaMask)
DEFINE_CONST_PTR_TYPE(ModelNodeEffectAlphaMask)

} // model
} // bv
