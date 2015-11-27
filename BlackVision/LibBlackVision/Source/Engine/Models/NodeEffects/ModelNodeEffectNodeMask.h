#pragma once

#include "Engine/Models/NodeEffects/ModelNodeEffectBase.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"

namespace bv { namespace model {

class ModelNodeEffectNodeMask : public ModelNodeEffectBase
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

	virtual void			Update					( TimeType t ) override;

    ParamIntPtr             GetParamBgIdx           () const;
    ParamIntPtr             GetParamFgIdx           () const;
    ParamFloatPtr           GetParamAlpha           () const;

    unsigned int            GetBackgroundChildIdx   () const;
    unsigned int            GetForegroundChildIdx   () const;
    float                   GetAlpha                () const;

};

DEFINE_PTR_TYPE(ModelNodeEffectNodeMask)
DEFINE_CONST_PTR_TYPE(ModelNodeEffectNodeMask)

} // model
} // bv
