#include "ModelNodeEffectDefault.h"

#include "Serialization/SerializationHelper.h"


namespace bv { namespace model {

// ********************************
//
ModelNodeEffectDefault::ModelNodeEffectDefault  ( ITimeEvaluatorPtr timeEvaluator )
{
    { timeEvaluator; }
}

// ********************************
//
void            ModelNodeEffectDefault::Serialize       ( ISerializer& ser ) const
{
ser.EnterChild( "effect" );
    ser.SetAttribute( "type", SerializationHelper::T2String< NodeEffectType >( GetType() ) );
ser.ExitChild();
}

// ********************************
//
NodeEffectType  ModelNodeEffectDefault::GetType () const
{
    return NodeEffectType::NET_DEFAULT;
}

// ********************************
//
void            ModelNodeEffectDefault::Update  ( TimeType t )
{
    { t; }
}

} // model
} // bv
