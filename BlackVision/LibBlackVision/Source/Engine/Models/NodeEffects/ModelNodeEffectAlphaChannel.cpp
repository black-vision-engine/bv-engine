#include "ModelNodeEffectAlphaChannel.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"

#include "Serialization/SerializationHelper.h"

namespace bv { namespace model {


// ********************************
//
void            ModelNodeEffectAlphaChannel::Serialize       ( ISerializer& ser ) const
{
ser.EnterChild( "effect" );
    ser.SetAttribute( "type", SerializationHelper::T2String< NodeEffectType >( GetType() ) );
ser.ExitChild();
}

// ********************************
//
ModelNodeEffectAlphaChannel::ModelNodeEffectAlphaChannel( ITimeEvaluatorPtr timeEvaluator )
{
}

// ********************************
//
NodeEffectType  ModelNodeEffectAlphaChannel::GetType() const
{
    return NodeEffectType::NET_ALPHA_CHANNEL;
}

// ********************************
//
void            ModelNodeEffectAlphaChannel::Update    ( TimeType t )
{
    { t; }
}

} // model
} // bv
