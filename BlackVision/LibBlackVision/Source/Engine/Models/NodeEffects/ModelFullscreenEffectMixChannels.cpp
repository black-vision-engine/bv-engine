#include "ModelFullscreenEffectMixChannels.h"

#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"


namespace bv { namespace model {


// ********************************
//
ModelFullscreenEffectMixChannels::ModelFullscreenEffectMixChannels( const std::string & name, ITimeEvaluatorPtr timeEvaluator )
    : ModelSimpleFullscreenEffect( name )
    , m_channelMixMask( GetChannelMixMask( 0, 1, 2, 3 ) )
    , m_channelMaskMask( 1.f, 1.f, 1.f, 1.f )
{
    auto channelMixMaskEval = ParamValEvaluatorFactory::CreateSimpleIntEvaluator( "channelMixMask", timeEvaluator );
    auto channelMaskMaskEval = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "channelMaskMask", timeEvaluator );
    
    m_paramValModel->RegisterAll( channelMixMaskEval );
    m_paramValModel->RegisterAll( channelMaskMaskEval );

    m_paramChannelMixMask = channelMixMaskEval->Parameter();
    m_paramChannelMaskMask = channelMaskMaskEval->Parameter();

    m_paramChannelMixMask->SetVal( m_channelMixMask, 0.f );
    m_paramChannelMaskMask->SetVal( m_channelMaskMask, 0.f );
}

// ********************************
//
NodeEffectType    ModelFullscreenEffectMixChannels::GetType() const
{
    return NodeEffectType::NET_MIX_CHANNELS;
}

// ********************************
//
ParamIntPtr             ModelFullscreenEffectMixChannels::GetParamChannelMixMask    () const
{
    return m_paramChannelMixMask;
}

// ********************************
//
ParamVec4Ptr            ModelFullscreenEffectMixChannels::GetParamChannelMaskMask   () const
{
    return m_paramChannelMaskMask;
}

// ********************************
//
void                    ModelFullscreenEffectMixChannels::Update                ( TimeType t )
{
    ModelSimpleFullscreenEffect::Update( t );
    
    m_channelMixMask = m_paramChannelMixMask->Evaluate();
    m_channelMaskMask = m_paramChannelMaskMask->Evaluate();
}

// ********************************
//
glm::vec4               ModelFullscreenEffectMixChannels::GetChannelMaskMask    () const
{
    return m_channelMaskMask;
}

// ********************************
//
int                     ModelFullscreenEffectMixChannels::GetChannelMixMask     () const
{
    return m_channelMixMask;
}

// **************************
//
unsigned int            ModelFullscreenEffectMixChannels:: GetChannelMixMask    ( unsigned int rIdx, unsigned int gIdx, unsigned int bIdx, unsigned int aIdx )
{
    return ( aIdx & 0x3 ) << 6 | ( bIdx & 0x3 ) << 4 | ( gIdx & 0x3 ) << 2 | ( rIdx & 0x3 ) << 0;
}

} // model
} // bv
