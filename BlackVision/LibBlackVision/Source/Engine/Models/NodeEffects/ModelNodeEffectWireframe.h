#pragma once

#include "Engine/Models/NodeEffects/ModelNodeEffectBase.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"


namespace bv { namespace model {

class ModelNodeEffectWireframe : public ModelNodeEffectBase
{
public:

                            ModelNodeEffectWireframe( ITimeEvaluatorPtr timeEvaluator );

    static ISerializablePtr Create              ( const IDeserializer& deser );

    virtual NodeEffectType  GetType             () const override;

};

DEFINE_PTR_TYPE(ModelNodeEffectWireframe)
DEFINE_CONST_PTR_TYPE(ModelNodeEffectWireframe)

} // model
} // bv
