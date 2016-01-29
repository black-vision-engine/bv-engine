#include "stdafx.h"

#include "ModelNodeEffectAlphaMask.h"

#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"


namespace bv { namespace model {

// ********************************
//
ModelNodeEffectAlphaMask::ModelNodeEffectAlphaMask( ITimeEvaluatorPtr timeEvaluator )
    : ModelNodeEffectBase( timeEvaluator )
    , m_alphaVal( 1.f )
{
    auto alphaEval = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "alpha", timeEvaluator );
    alphaEval->Parameter()->SetVal( m_alphaVal, 0.f );

    m_paramValModel->RegisterAll( alphaEval );

    m_paramAlpha = alphaEval->Parameter();
}

// ********************************
//
NodeEffectType  ModelNodeEffectAlphaMask::GetType() const
{
    return NodeEffectType::NET_ALPHA_MASK;
}

// ********************************
//
void            ModelNodeEffectAlphaMask::Update  ( TimeType t )
{
    ModelNodeEffectBase::Update( t );
    
    m_alphaVal = m_paramAlpha->Evaluate();
}

// ********************************
//
ParamFloatPtr   ModelNodeEffectAlphaMask::GetParamAlpha  () const
{
    return m_paramAlpha;
}

// ********************************
//
float           ModelNodeEffectAlphaMask::GetAlpha      () const
{
    return m_alphaVal;
}

} // model
} // bv
