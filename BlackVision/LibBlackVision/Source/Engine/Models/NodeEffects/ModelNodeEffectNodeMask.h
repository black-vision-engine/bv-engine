#pragma once

#include "Engine/Models/Interfaces/IModelNodeEffect.h"

#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"


namespace bv { namespace model {

class ModelNodeEffectNodeMask : public IModelNodeEffect
{
private:

    ParamIntPtr     m_paramBgIdx;
    ParamIntPtr     m_paramFgIdx;
    ParamFloatPtr   m_paramAlpha;

    unsigned int    m_bgVal;
    unsigned int    m_fgVal;
    float           m_alphaVal;

public:

                            ModelNodeEffectNodeMask ( ITimeEvaluatorPtr timeEvaluator ); 

    virtual NodeEffectType  GetType                 () const override;

    ParamIntPtr             GetParamBgIdx           ();
    ParamIntPtr             GetParamFgIdx           ();
    ParamFloatPtr           GetParamAlpha           ();

    virtual void            Update                  ( TimeType t ) override;

    unsigned int            GetBackgroundChildIdx   () const;
    unsigned int            GetForegroundChildIdx   () const;
    float                   GetAlpha                () const;

};

DEFINE_PTR_TYPE(ModelNodeEffectNodeMask)
DEFINE_CONST_PTR_TYPE(ModelNodeEffectNodeMask)

} // model
} // bv
