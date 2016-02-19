#pragma once

#include "Engine/Models/NodeEffects/Fullscreen/ModelSimpleFullscreenEffect.h"


namespace bv { namespace model {

class ModelFullscreenEffectDefault : public ModelSimpleFullscreenEffect
{
public:

                                ModelFullscreenEffectDefault  ( const std::string & name, ITimeEvaluatorPtr timeEvaluator );

};

DEFINE_PTR_TYPE(ModelFullscreenEffectDefault)
DEFINE_CONST_PTR_TYPE(ModelFullscreenEffectDefault)

} // model
} // bv
