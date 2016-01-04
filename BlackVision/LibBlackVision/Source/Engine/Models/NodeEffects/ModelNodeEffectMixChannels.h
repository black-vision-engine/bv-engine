#pragma once

#include "Engine/Models/Interfaces/IModelNodeEffect.h"

#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"


namespace bv { namespace model {

class ModelNodeEffectMixChannels : public IModelNodeEffect
{
private:

    ParamIntPtr     m_paramChannelMixMask;
    ParamVec4Ptr    m_paramChannelMaskMask;

    int             m_channelMixMask;
    glm::vec4       m_channelMaskMask;

public:

                            ModelNodeEffectMixChannels( ITimeEvaluatorPtr timeEvaluator );

    virtual NodeEffectType  GetType                 () const override;

    ParamIntPtr             GetParamChannelMixMask  ();
    ParamVec4Ptr            GetParamChannelMaskMask ();

    virtual void            Update                  ( TimeType t ) override;

    glm::vec4               GetChannelMaskMask      () const;
    int                     GetChannelMixMask       () const;

    static unsigned int     GetChannelMixMask       ( unsigned int rIdx, unsigned int gIdx, unsigned int bIdx, unsigned int aIdx );

private:

    // FIXME: implement
    // int                     GetChannelMask          () const;

};

DEFINE_PTR_TYPE(ModelNodeEffectMixChannels)
DEFINE_CONST_PTR_TYPE(ModelNodeEffectMixChannels)

} // model
} // bv
