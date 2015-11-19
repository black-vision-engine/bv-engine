#include "ModelNodeEffectAlphaMask.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"

#include "Serialization/SerializationHelper.h"

namespace bv { namespace model {


// ********************************
//
void            ModelNodeEffectAlphaMask::Serialize       ( ISerializer& ser ) const
{
ser.EnterChild( "effect" );
    ser.SetAttribute( "type", SerializationHelper::T2String< NodeEffectType >( GetType() ) );
    m_paramAlpha->Serialize( ser );
ser.ExitChild();
}

// ********************************
//
ModelNodeEffectAlphaMask::ModelNodeEffectAlphaMask( ITimeEvaluatorPtr timeEvaluator )
    : m_alpha( 1.f )
{
    m_paramAlpha = ParametersFactory::CreateParameterFloat( "alpha", timeEvaluator );

    m_paramAlpha->SetVal( 1.f, 0.f );
}

// ********************************
//
NodeEffectType  ModelNodeEffectAlphaMask::GetType() const
{
    return NodeEffectType::NET_ALPHA_MASK;
}

// ********************************
//
ParamFloatPtr   ModelNodeEffectAlphaMask::GetParamAlpha ()
{
    return m_paramAlpha;
}

// ********************************
//
void            ModelNodeEffectAlphaMask::Update    ( TimeType t )
{
    { t; }
    m_alpha = m_paramAlpha->Evaluate();
}

// ********************************
//
float           ModelNodeEffectAlphaMask::GetAlpha  () const
{
    return m_alpha;
}

} // model
} // bv
