#pragma once

#include "Engine/Models/NodeEffects/Fullscreen/ModelSimpleFullscreenEffect.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"


namespace bv { namespace model {

class ModelFullscreenEffectNodeMask : public ModelSimpleFullscreenEffect
{
private:

    ParamIntPtr     m_paramBgIdx;
    ParamIntPtr     m_paramFgIdx;
    ParamFloatPtr   m_paramAlpha;

    unsigned int    m_bgVal;
    unsigned int    m_fgVal;
    float           m_alphaVal;

public:

                                    ModelFullscreenEffectNodeMask   ( const std::string & name, ITimeEvaluatorPtr timeEvaluator ); 
    
    ParamIntPtr                     GetParamBgIdx           () const;
    ParamIntPtr                     GetParamFgIdx           () const;
    ParamFloatPtr                   GetParamAlpha           () const;

    virtual void                    Update                  ( TimeType t ) override;

    unsigned int                    GetBackgroundChildIdx   () const;
    unsigned int                    GetForegroundChildIdx   () const;
    float                           GetAlpha                () const;

};

DEFINE_PTR_TYPE(ModelFullscreenEffectNodeMask)
DEFINE_CONST_PTR_TYPE(ModelFullscreenEffectNodeMask)

} // model
} // bv
