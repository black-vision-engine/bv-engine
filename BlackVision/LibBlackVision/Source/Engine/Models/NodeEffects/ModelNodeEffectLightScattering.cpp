#include "ModelNodeEffectLightScattering.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"

#include "Serialization/SerializationHelper.h"


namespace bv { namespace model {


// ********************************
//
ModelNodeEffectLightScattering::ModelNodeEffectLightScattering  ( ITimeEvaluatorPtr timeEvaluator )
{ 
    m_paramExposure = ParametersFactory::CreateParameterFloat( "exposure", timeEvaluator );
    m_paramWeight = ParametersFactory::CreateParameterFloat( "weight", timeEvaluator );
    m_paramDecay = ParametersFactory::CreateParameterFloat( "decay", timeEvaluator );
    m_paramDensity = ParametersFactory::CreateParameterFloat( "density", timeEvaluator );
    m_paramLightPositionOnScreen = ParametersFactory::CreateParameterVec2( "lightPositionOnScreen", timeEvaluator );
    m_paramNumSamples = ParametersFactory::CreateParameterFloat( "numSamples", timeEvaluator );


    m_paramExposure->SetVal( 1.f, 0.f );
    m_paramWeight->SetVal( 1.f, 0.f );
    m_paramDecay->SetVal( 1.f, 0.f );
    m_paramDensity->SetVal( 1.f, 0.f );
    m_paramLightPositionOnScreen->SetVal( glm::vec2( 0.f, 0.f ), 0.f );
    m_paramNumSamples->SetVal( 100.f, 0.f );
}

// ********************************
//
void            ModelNodeEffectLightScattering::Serialize       ( ISerializer& ser ) const
{
    ser.EnterChild( "effect" );
    ser.SetAttribute( "type", SerializationHelper::T2String< NodeEffectType >( GetType() ) );

    m_paramExposure->Serialize( ser );
    m_paramWeight->Serialize( ser );
    m_paramDecay->Serialize( ser );
    m_paramDensity->Serialize( ser );
    m_paramLightPositionOnScreen->Serialize( ser );
    m_paramNumSamples->Serialize( ser );

    ser.ExitChild();
}

// ********************************
//
void            ModelNodeEffectLightScattering::Update    ( TimeType )
{
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
ParamFloatPtr           ModelNodeEffectLightScattering::GetParamExposure                ()
{
    return m_paramExposure;
}

// ********************************
//
ParamFloatPtr           ModelNodeEffectLightScattering::GetParamWeight                  ()
{
    return m_paramWeight;
}

// ********************************
//
ParamFloatPtr           ModelNodeEffectLightScattering::GetParamDecay                   ()
{
    return m_paramDecay;
}

// ********************************
//
ParamFloatPtr           ModelNodeEffectLightScattering::GetParamDensity                 ()
{
    return m_paramDensity;
}

// ********************************
//
ParamVec2Ptr            ModelNodeEffectLightScattering::GetParamLightPositionOnScreen   ()
{
    return m_paramLightPositionOnScreen;
}

// ********************************
//
ParamFloatPtr           ModelNodeEffectLightScattering::GetParamNumSamples              ()
{
    return m_paramNumSamples;
}

// ********************************
//
float                   ModelNodeEffectLightScattering::GetExposure                     ()
{
    return m_exposureVal;
}

// ********************************
//
float                   ModelNodeEffectLightScattering::GetWeight                       ()
{
    return m_weightVal;
}

// ********************************
//
float                   ModelNodeEffectLightScattering::GetDecay                        ()
{
    return m_decayVal;
}

// ********************************
//
float                   ModelNodeEffectLightScattering::GetDensity                      ()
{
    return m_densityVal;
}

// ********************************
//
glm::vec2               ModelNodeEffectLightScattering::GetLightPositionOnScreen        ()
{
    return m_lightPositionOnScreenVal;
}

// ********************************
//
float                   ModelNodeEffectLightScattering::GetNumSamples                   ()
{
    return m_numSamplesVal;
}

} // model
} // bv
