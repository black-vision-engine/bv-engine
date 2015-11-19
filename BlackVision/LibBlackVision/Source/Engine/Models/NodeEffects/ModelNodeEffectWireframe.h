#pragma once

#include "Engine/Models/Interfaces/IModelNodeEffect.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"


namespace bv { namespace model {

class ModelNodeEffectWireframe : public IModelNodeEffect
{
private:

public:

                            ModelNodeEffectWireframe( ITimeEvaluatorPtr timeEvaluator );

        virtual void            Serialize       ( ISerializer& ser ) const;
        static ISerializablePtr Create          ( const IDeserializer& deser );

        virtual void            Update              ( TimeType t ) override;

        virtual NodeEffectType  GetType             () const override;

};

DEFINE_PTR_TYPE(ModelNodeEffectWireframe)
DEFINE_CONST_PTR_TYPE(ModelNodeEffectWireframe)

} // model
} // bv
