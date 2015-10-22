#pragma once

#include "Engine/Models/Interfaces/IModelNodeEffect.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"


namespace bv { namespace model {

class ModelNodeEffectNodeMask : public IModelNodeEffect
{
private:

    ParamIntPtr     m_paramBgIdx;
    ParamIntPtr     m_paramFgIdx;

    unsigned int    m_bgVal;
    unsigned int    m_fgVal;

public:

                            ModelNodeEffectNodeMask ( ITimeEvaluatorPtr timeEvaluator ); 

    virtual NodeEffectType  GetType                 () const override;

    ParamIntPtr             GetParamBgIdx           ();
    ParamIntPtr             GetParamFgIdx           ();

    virtual void            Update                  ( TimeType t ) override;

    unsigned int            GetBackgroundChildIdx   () const;
    unsigned int            GetForegroundChildIdx   () const;

};

DEFINE_PTR_TYPE(ModelNodeEffectNodeMask)
DEFINE_CONST_PTR_TYPE(ModelNodeEffectNodeMask)

} // model
} // bv
