#include "ModelNodeEffectMixChannels.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"


namespace bv { namespace model {


// ********************************
//
ModelNodeEffectMixChannels::ModelNodeEffectMixChannels( ITimeEvaluatorPtr timeEvaluator )
    : m_channelMixMask( GetChannelMixMask( 0, 1, 2, 3 ) )
    , m_channelMaskMask( 1.f, 1.f, 1.f, 1.f )
{
    m_paramChannelMixMask = ParametersFactory::CreateParameterInt( "channelMixMask", timeEvaluator );
    m_paramChannelMaskMask = ParametersFactory::CreateParameterVec4( "channelMaskMask", timeEvaluator );

    m_paramChannelMixMask->SetVal( m_channelMixMask, 0.f );
    m_paramChannelMaskMask->SetVal( m_channelMaskMask, 0.f );
}

// ********************************
//
NodeEffectType  ModelNodeEffectMixChannels::GetType() const
{
    return NodeEffectType::NET_MIX_CHANNELS;
}

// ********************************
//
ParamIntPtr     ModelNodeEffectMixChannels::GetParamChannelMixMask  ()
{
    return m_paramChannelMixMask;
}

// ********************************
//
ParamVec4Ptr    ModelNodeEffectMixChannels::GetParamChannelMaskMask ()
{
    return m_paramChannelMaskMask;
}

// ********************************
//
void            ModelNodeEffectMixChannels::Update    ( TimeType t )
{
    { t; }
    m_channelMixMask = m_paramChannelMixMask->Evaluate();
    m_channelMaskMask = m_paramChannelMaskMask->Evaluate();
}

// ********************************
//
glm::vec4       ModelNodeEffectMixChannels::GetChannelMaskMask  () const
{
    return m_channelMaskMask;
}

// ********************************
//
int             ModelNodeEffectMixChannels::GetChannelMixMask   () const
{
    return m_channelMixMask;
}

// **************************
//
unsigned int    ModelNodeEffectMixChannels:: GetChannelMixMask ( unsigned int rIdx, unsigned int gIdx, unsigned int bIdx, unsigned int aIdx )
{
    return ( aIdx & 0x3 ) << 6 | ( bIdx & 0x3 ) << 4 | ( gIdx & 0x3 ) << 2 | ( rIdx & 0x3 ) << 0;
}

} // model
} // bv
