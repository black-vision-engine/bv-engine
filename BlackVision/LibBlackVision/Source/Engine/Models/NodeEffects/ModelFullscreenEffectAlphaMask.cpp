#include "ModelFullscreenEffectAlphaMask.h"

#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"


namespace bv { namespace model {


// ********************************
//
ModelFullscreenEffectAlphaMask::ModelFullscreenEffectAlphaMask( const std::string & name, ITimeEvaluatorPtr timeEvaluator )
    : ModelSimpleFullscreenEffect( name )
    , m_alphaVal( 1.f )
{
    auto alphaEval = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "alpha", timeEvaluator );
    alphaEval->Parameter()->SetVal( m_alphaVal, 0.f );

    m_paramValModel->RegisterAll( alphaEval );

    m_paramAlpha = alphaEval->Parameter();
}

// ********************************
//
NodeEffectType    ModelFullscreenEffectAlphaMask::GetType() const
{
    return NodeEffectType::NET_ALPHA_MASK;
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
