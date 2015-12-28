#pragma once

#include "Engine/Models/NodeEffects/ModelNodeEffectBase.h"

namespace bv { namespace model {

class ModelNodeEffectDefault : public ModelNodeEffectBase
{
public:

            ModelNodeEffectDefault  ( ITimeEvaluatorPtr timeEvaluator );

    static ISerializablePtr Create          ( const IDeserializer& deser );

    virtual NodeEffectType  GetType () const override;

};
} // model
} // bv
