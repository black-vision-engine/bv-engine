#include "ModelNodeEffectWireframe.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"

#include "Serialization/SerializationHelper.h"


namespace bv { namespace model {


// ********************************
//
ModelNodeEffectWireframe::ModelNodeEffectWireframe  ( ITimeEvaluatorPtr timeEvaluator )
{ 
    { timeEvaluator; }
}

// ********************************
//
void            ModelNodeEffectWireframe::Serialize       ( ISerializer& ser ) const
{
ser.EnterChild( "effect" );
    ser.SetAttribute( "type", SerializationHelper::T2String< NodeEffectType >( GetType() ) );
ser.ExitChild();
}

// ********************************
//
void            ModelNodeEffectWireframe::Update    ( TimeType t )
{
    { t; }
}

// ********************************
//
NodeEffectType  ModelNodeEffectWireframe::GetType   () const
{
    return NodeEffectType::NET_WIREFRAME;
}

} // model
} // bv
