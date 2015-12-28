#include "ModelNodeEffectLightScattering.h"

#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"


namespace bv { namespace model {


// ********************************
//
ModelNodeEffectLightScattering::ModelNodeEffectLightScattering  ( ITimeEvaluatorPtr timeEvaluator )
	: ModelNodeEffectBase( timeEvaluator )
	, m_exposureVal( 0.005f )
	, m_weightVal( 2.65f )
	, m_decayVal( 1.0f )
	, m_densityVal( 0.2f )
	, m_lightPositionOnScreenVal( glm::vec2( 0.1f, 0.4f ) )
	, m_numSamplesVal( 100.f )
{ 
    auto exposureEval = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "exposure", timeEvaluator );
    auto weightEval = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "weight", timeEvaluator );
    auto decayEval = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "decay", timeEvaluator );
    auto densityEval = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "density", timeEvaluator );
    auto lightPositionOnScreenEval = ParamValEvaluatorFactory::CreateSimpleVec2Evaluator( "lightPositionOnScreen", timeEvaluator );
    auto numSamplesEval = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "numSamples", timeEvaluator );

	m_paramExposure = exposureEval->Parameter();
	m_paramWeight = weightEval->Parameter();
	m_paramDecay = decayEval->Parameter();
	m_paramDensity = densityEval->Parameter();
	m_paramLightPositionOnScreen = lightPositionOnScreenEval->Parameter();
	m_paramNumSamples = numSamplesEval->Parameter();

    m_paramExposure->SetVal( m_exposureVal, 0.f );
    // m_paramExposure->SetVal( 1.f, 0.f );
    m_paramWeight->SetVal( m_weightVal, 0.f );
    //m_paramWeight->SetVal( 1.f, 5.f );
    m_paramDecay->SetVal( m_decayVal, 0.f );
    //m_paramDecay->SetVal( 0.8f, 5.f );
    m_paramDensity->SetVal( m_densityVal, 0.f );

    m_paramLightPositionOnScreen->SetVal( m_lightPositionOnScreenVal, 0.f );
    m_paramLightPositionOnScreen->SetVal( glm::vec2( 0.4f, 0.6f ), 5.f );
    m_paramLightPositionOnScreen->SetVal( glm::vec2( 0.5f, 0.4f ), 10.f );
    m_paramLightPositionOnScreen->SetVal( glm::vec2( 0.4f, 0.1f ), 15.f );

    m_paramLightPositionOnScreen->AccessInterpolator().SetWrapPostMethod( WrapMethod::pingPong );

    m_paramNumSamples->SetVal( 100.f, 0.f );
}

// ********************************
//
void            ModelNodeEffectLightScattering::Update    ( TimeType t )
{
	ModelNodeEffectBase::Update( t );
	
    m_exposureVal = m_paramExposure->Evaluate();
    m_weightVal = m_paramWeight->Evaluate();
    m_decayVal = m_paramDecay->Evaluate();
    m_densityVal = m_paramDensity->Evaluate();
    m_lightPositionOnScreenVal = m_paramLightPositionOnScreen->Evaluate();
    m_numSamplesVal = m_paramNumSamples->Evaluate();
}
// ********************************
//
NodeEffectType  ModelNodeEffectLightScattering::GetType   () const
{
    return NodeEffectType::NET_LIGHT_SCATTERING;
}

// ********************************
//
ParamFloatPtr           ModelNodeEffectLightScattering::GetParamExposure                () const
{
    return m_paramExposure;
}

// ********************************
//
ParamFloatPtr           ModelNodeEffectLightScattering::GetParamWeight                  () const
{
    return m_paramWeight;
}

// ********************************
//
ParamFloatPtr           ModelNodeEffectLightScattering::GetParamDecay                   () const
{
    return m_paramDecay;
}

// ********************************
//
ParamFloatPtr           ModelNodeEffectLightScattering::GetParamDensity                 () const
{
    return m_paramDensity;
}

// ********************************
//
ParamVec2Ptr            ModelNodeEffectLightScattering::GetParamLightPositionOnScreen   () const
{
    return m_paramLightPositionOnScreen;
}

// ********************************
//
ParamFloatPtr           ModelNodeEffectLightScattering::GetParamNumSamples              () const
{
    return m_paramNumSamples;
}

// ********************************
//
float                   ModelNodeEffectLightScattering::GetExposure                     () const
{
    return m_exposureVal;
}

// ********************************
//
float                   ModelNodeEffectLightScattering::GetWeight                       () const
{
    return m_weightVal;
}

// ********************************
//
float                   ModelNodeEffectLightScattering::GetDecay                        () const
{
    return m_decayVal;
}

// ********************************
//
float                   ModelNodeEffectLightScattering::GetDensity                      () const
{
    return m_densityVal;
}

// ********************************
//
glm::vec2               ModelNodeEffectLightScattering::GetLightPositionOnScreen        () const
{
    return m_lightPositionOnScreenVal;
}

// ********************************
//
float                   ModelNodeEffectLightScattering::GetNumSamples                   () const
{
    return m_numSamplesVal;
}

} // model
} // bv
