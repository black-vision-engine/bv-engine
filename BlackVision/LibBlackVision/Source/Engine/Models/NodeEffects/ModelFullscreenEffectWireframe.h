#pragma once

#include "Engine/Models/NodeEffects/ModelSimpleFullscreenEffect.h"


namespace bv { namespace model {

class ModelFullscreenEffectWireframe : public ModelSimpleFullscreenEffect
{
public:

                                    ModelFullscreenEffectWireframe  ( const std::string & name, ITimeEvaluatorPtr timeEvaluator );

        virtual NodeEffectType      GetType             () const override;

};

DEFINE_PTR_TYPE(ModelFullscreenEffectWireframe)
DEFINE_CONST_PTR_TYPE(ModelFullscreenEffectWireframe)

} // model
} // bv
