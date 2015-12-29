#pragma once

#include "Engine/Models/Interfaces/IModelNodeEffect.h"

#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"


namespace bv { namespace model {

class ModelNodeEffectMixChannels : public IModelNodeEffect
{
private:

    ParamIntPtr     m_paramChannelMask;
    
    int             m_channelMask;

public:

                            ModelNodeEffectMixChannels( ITimeEvaluatorPtr timeEvaluator );

    virtual NodeEffectType  GetType                 () const override;

    ParamIntPtr             GetParamChannelMask     ();

    virtual void            Update                  ( TimeType t ) override;

    int                     GetChannelMask          () const;

private:

    // FIXME: implement
    // int                     GetChannelMask          () const;

};

DEFINE_PTR_TYPE(ModelNodeEffectMixChannels)
DEFINE_CONST_PTR_TYPE(ModelNodeEffectMixChannels)

} // model
} // bv
