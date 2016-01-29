#include "stdafx.h"

#include "ModelNodeEffectWireframe.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"

#include "Serialization/SerializationHelper.h"


namespace bv { namespace model {


// ********************************
//
ModelNodeEffectWireframe::ModelNodeEffectWireframe  ( ITimeEvaluatorPtr timeEvaluator )
	: ModelNodeEffectBase( timeEvaluator )
{ 
}

// ********************************
//
NodeEffectType  ModelNodeEffectWireframe::GetType   () const
{
    return NodeEffectType::NET_WIREFRAME;
}

} // model
} // bv
