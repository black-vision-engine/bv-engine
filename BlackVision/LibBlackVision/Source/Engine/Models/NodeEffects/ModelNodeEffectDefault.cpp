#include "stdafx.h"

#include "ModelNodeEffectDefault.h"

#include "Serialization/SerializationHelper.h"


namespace bv { namespace model {

// ********************************
//
ModelNodeEffectDefault::ModelNodeEffectDefault  ( ITimeEvaluatorPtr timeEvaluator )
	: ModelNodeEffectBase( timeEvaluator )
{
}

// ********************************
//
NodeEffectType  ModelNodeEffectDefault::GetType () const
{
    return NodeEffectType::NET_DEFAULT;
}

} // model
} // bv
