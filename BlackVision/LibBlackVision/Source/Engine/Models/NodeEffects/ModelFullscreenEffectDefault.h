#pragma once

#include "Engine/Models/NodeEffects/ModelSimpleFullscreenEffect.h"


namespace bv { namespace model {

class ModelFullscreenEffectDefault : public ModelSimpleFullscreenEffect
{
public:

                                ModelFullscreenEffectDefault  ( const std::string & name, ITimeEvaluatorPtr timeEvaluator );

    virtual NodeEffectType      GetType         () const override;

};

DEFINE_PTR_TYPE(ModelFullscreenEffectDefault)
DEFINE_CONST_PTR_TYPE(ModelFullscreenEffectDefault)

} // model
} // bv
