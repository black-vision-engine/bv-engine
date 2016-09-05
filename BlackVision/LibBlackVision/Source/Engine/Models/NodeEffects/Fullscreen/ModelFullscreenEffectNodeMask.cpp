#include "ModelFullscreenEffectNodeMask.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {


// ********************************
//
ModelFullscreenEffectNodeMask::ModelFullscreenEffectNodeMask( const std::string & name, ITimeEvaluatorPtr timeEvaluator )
	: ModelSimpleFullscreenEffect( name )
	, m_bgVal( 0 )
	, m_fgVal( 1 )
	, m_alphaVal( 1.f )
{ 
	m_paramBgIdx = ParametersFactory::CreateParameterInt( "maskIdx", timeEvaluator );
    m_paramFgIdx = ParametersFactory::CreateParameterInt( "fgIdx", timeEvaluator );
	m_paramAlpha = ParametersFactory::CreateParameterFloat( "alpha", timeEvaluator );

    m_paramBgIdx->SetVal( m_bgVal, 0.f );
    m_paramFgIdx->SetVal( m_fgVal, 0.f );
    m_paramAlpha->SetVal( m_alphaVal, 0.f );

    m_paramValModel->AddParameter( m_paramBgIdx );
    m_paramValModel->AddParameter( m_paramFgIdx );
    m_paramValModel->AddParameter( m_paramAlpha );
}

// ********************************
//
ParamIntPtr     ModelFullscreenEffectNodeMask::GetParamBgIdx           () const
{
    return m_paramBgIdx;
}

// ********************************
//
ParamIntPtr     ModelFullscreenEffectNodeMask::GetParamFgIdx           () const
{
    return m_paramFgIdx;
}

// ********************************
//
ParamFloatPtr   ModelFullscreenEffectNodeMask::GetParamAlpha           () const
{
    return m_paramAlpha;
}

// ********************************
//
void            ModelFullscreenEffectNodeMask::Update  ( TimeType t )
{
    ModelSimpleFullscreenEffect::Update( t );

    m_bgVal = m_paramBgIdx->Evaluate();
    m_fgVal = m_paramFgIdx->Evaluate();
    m_alphaVal = m_paramAlpha->Evaluate();

    assert( m_bgVal != m_fgVal );
}

// ********************************
//
unsigned int    ModelFullscreenEffectNodeMask::GetBackgroundChildIdx   () const
{
    return m_bgVal;
}

// ********************************
//
unsigned int    ModelFullscreenEffectNodeMask::GetForegroundChildIdx   () const
{
    return m_fgVal;
}

// ********************************
//
float           ModelFullscreenEffectNodeMask::GetAlpha                () const
{
    return m_alphaVal;
}

} // model
} // bv
