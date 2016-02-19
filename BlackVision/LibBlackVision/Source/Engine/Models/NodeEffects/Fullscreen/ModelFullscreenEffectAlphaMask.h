#pragma once

#include "Engine/Models/NodeEffects/Fullscreen/ModelSimpleFullscreenEffect.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"


namespace bv { namespace model {

class ModelFullscreenEffectAlphaMask : public ModelSimpleFullscreenEffect
{
private:

    ParamFloatPtr       m_paramAlpha;

    float               m_alphaVal;

public:

                                    ModelFullscreenEffectAlphaMask  ( const std::string & name, ITimeEvaluatorPtr timeEvaluator );

    virtual void                    Update                  ( TimeType t ) override;

    ParamFloatPtr                   GetParamAlpha           () const;
    float                           GetAlpha                () const;

};

DEFINE_PTR_TYPE(ModelFullscreenEffectAlphaMask)
DEFINE_CONST_PTR_TYPE(ModelFullscreenEffectAlphaMask)

} // model
} // bv
