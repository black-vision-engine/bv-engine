#include "ModelNodeEffectNodeMask.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"


namespace bv { namespace model {


// ********************************
//
ModelNodeEffectNodeMask::ModelNodeEffectNodeMask( ITimeEvaluatorPtr timeEvaluator )
    : m_bgVal( 0 )
    , m_fgVal( 1 )
{ 
    m_paramBgIdx = ParametersFactory::CreateParameterInt( "maskIdx", timeEvaluator );
    m_paramFgIdx = ParametersFactory::CreateParameterInt( "fgIdx", timeEvaluator );
    m_paramAlpha = ParametersFactory::CreateParameterFloat( "alpha", timeEvaluator );

    m_paramBgIdx->SetVal( 0, 0.f );
    m_paramFgIdx->SetVal( 1, 0.f );
    m_paramAlpha->SetVal( 1.f, 0.f );
}

// ********************************
//
NodeEffectType  ModelNodeEffectNodeMask::GetType() const
{
    return NodeEffectType::NET_NODE_MASK;
}

// ********************************
//
ParamIntPtr     ModelNodeEffectNodeMask::GetParamBgIdx           ()
{
    return m_paramBgIdx;
}

// ********************************
//
ParamIntPtr     ModelNodeEffectNodeMask::GetParamFgIdx           ()
{
    return m_paramFgIdx;
}

// ********************************
//
ParamFloatPtr   ModelNodeEffectNodeMask::GetParamAlpha           ()
{
    return m_paramAlpha;
}

// ********************************
//
void            ModelNodeEffectNodeMask::Update  ( TimeType t )
{
    { t; }

    m_bgVal = m_paramBgIdx->Evaluate();
    m_fgVal = m_paramFgIdx->Evaluate();
    m_alphaVal = m_paramAlpha->Evaluate();

    assert( m_bgVal != m_fgVal );
}

// ********************************
//
unsigned int    ModelNodeEffectNodeMask::GetBackgroundChildIdx   () const
{
    return m_bgVal;
}

// ********************************
//
unsigned int    ModelNodeEffectNodeMask::GetForegroundChildIdx   () const
{
    return m_fgVal;
}

// ********************************
//
float           ModelNodeEffectNodeMask::GetAlpha                () const
{
    return m_alphaVal;
}

} // model
} // bv
