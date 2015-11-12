#pragma once

#include "Engine/Models/Interfaces/IModelNodeEffect.h"
#include "Engine/Models/Interfaces/ITimeEvaluator.h"


namespace bv { namespace model {

class ModelNodeEffectDefault : public IModelNodeEffect
{
public:

            ModelNodeEffectDefault  ( ITimeEvaluatorPtr timeEvaluator );

    virtual void            Serialize       ( ISerializer& ser ) const;
    static ISerializablePtr Create          ( const IDeserializer& deser );

    virtual NodeEffectType  GetType () const override;
    virtual void            Update  ( TimeType t ) override;

};
} // model
} // bv
