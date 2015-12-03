#pragma once

#include "Engine/Models/Interfaces/IModelNodeEffect.h"

#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"


namespace bv { namespace model {

class ModelNodeEffectAlphaChannel : public IModelNodeEffect
{
public:

                            ModelNodeEffectAlphaChannel	( ITimeEvaluatorPtr timeEvaluator );

    virtual void            Serialize					( ISerializer& ser ) const;
    static ISerializablePtr Create						( const IDeserializer& deser );

    virtual NodeEffectType  GetType                 () const override;

    virtual void            Update                  ( TimeType t ) override;

};

DEFINE_PTR_TYPE(ModelNodeEffectAlphaChannel)
DEFINE_CONST_PTR_TYPE(ModelNodeEffectAlphaChannel)

} // model
} // bv
