#include "ModelNodeEffectMixChannels.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"


namespace bv { namespace model {


// ********************************
//
ModelNodeEffectMixChannels::ModelNodeEffectMixChannels( ITimeEvaluatorPtr timeEvaluator )
    : m_channelMask( 228 )
{
    m_paramChannelMask = ParametersFactory::CreateParameterInt( "channelMask", timeEvaluator );

    m_paramChannelMask->SetVal( 228, 0.f );
}

// ********************************
//
NodeEffectType  ModelNodeEffectMixChannels::GetType() const
{
    return NodeEffectType::NET_MIX_CHANNELS;
}

// ********************************
//
ParamIntPtr   ModelNodeEffectMixChannels::GetParamChannelMask ()
{
    return m_paramChannelMask;
}

// ********************************
//
void            ModelNodeEffectMixChannels::Update    ( TimeType t )
{
    { t; }
    m_channelMask = m_paramChannelMask->Evaluate();
}

// ********************************
//
int           ModelNodeEffectMixChannels::GetChannelMask  () const
{
    return m_channelMask;
}

} // model
} // bv
