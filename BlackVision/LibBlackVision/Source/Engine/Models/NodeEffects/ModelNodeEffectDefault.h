#pragma once

#include "Engine/Models/Interfaces/IModelNodeEffect.h"
#include "Engine/Models/Interfaces/ITimeEvaluator.h"


namespace bv { namespace model {

class ModelNodeEffectDefault : public IModelNodeEffect
{
public:

            ModelNodeEffectDefault  ( ITimeEvaluatorPtr timeEvaluator );

    virtual NodeEffectType  GetType () const override;

};
} // model
} // bv
