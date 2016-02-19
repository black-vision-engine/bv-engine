#pragma once

#include "Engine/Models/NodeEffects/Fullscreen/ModelSimpleFullscreenEffect.h"

#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"


namespace bv { namespace model {

class ModelFullscreenEffectMixChannels : public ModelSimpleFullscreenEffect
{
private:

    ParamIntPtr     m_paramChannelMixMask;
    ParamVec4Ptr    m_paramChannelMaskMask;

    int             m_channelMixMask;
    glm::vec4       m_channelMaskMask;

public:

                                    ModelFullscreenEffectMixChannels    ( const std::string & name, ITimeEvaluatorPtr timeEvaluator );

    ParamIntPtr                     GetParamChannelMixMask  () const;
    ParamVec4Ptr                    GetParamChannelMaskMask () const;

    virtual void                    Update                  ( TimeType t ) override;

    glm::vec4                       GetChannelMaskMask      () const;
    int                             GetChannelMixMask       () const;

    static unsigned int             GetChannelMixMask       ( unsigned int rIdx, unsigned int gIdx, unsigned int bIdx, unsigned int aIdx );

private:

    // FIXME: implement
    // int                     GetChannelMask          () const;

};

DEFINE_PTR_TYPE(ModelFullscreenEffectMixChannels)
DEFINE_CONST_PTR_TYPE(ModelFullscreenEffectMixChannels)

} // model
} // bv
