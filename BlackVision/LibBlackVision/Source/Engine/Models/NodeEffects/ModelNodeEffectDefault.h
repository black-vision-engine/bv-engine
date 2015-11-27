#pragma once

#include "Engine/Models/NodeEffects/ModelNodeEffectBase.h"

namespace bv { namespace model {

class ModelNodeEffectDefault : public ModelNodeEffectBase
{
public:

            ModelNodeEffectDefault  ( ITimeEvaluatorPtr timeEvaluator );

    virtual NodeEffectType  GetType () const override;

};
} // model
} // bv
