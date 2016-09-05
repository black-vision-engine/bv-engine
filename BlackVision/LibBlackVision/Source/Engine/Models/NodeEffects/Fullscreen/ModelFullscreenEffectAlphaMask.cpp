#include "ModelFullscreenEffectAlphaMask.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {


// ********************************
//
ModelFullscreenEffectAlphaMask::ModelFullscreenEffectAlphaMask( const std::string & name, ITimeEvaluatorPtr timeEvaluator )
    : ModelSimpleFullscreenEffect( name )
    , m_alphaVal( 1.f )
{
    m_paramAlpha = ParametersFactory::CreateParameterFloat( "alpha", timeEvaluator );
    m_paramAlpha->SetVal( m_alphaVal, 0.f );
    m_paramValModel->AddParameter( m_paramAlpha );
}

// ********************************
//
void                    ModelFullscreenEffectAlphaMask::Update  ( TimeType t )
{
    ModelSimpleFullscreenEffect::Update( t );
    
    m_alphaVal = m_paramAlpha->Evaluate();
}

// ********************************
//
ParamFloatPtr           ModelFullscreenEffectAlphaMask::GetParamAlpha  () const
{
    return m_paramAlpha;
}

// ********************************
//
float                   ModelFullscreenEffectAlphaMask::GetAlpha      () const
{
    return m_alphaVal;
}

} // model
} // bv
