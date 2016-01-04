#pragma once

#include "Engine/Models/NodeEffects/ModelNodeEffectBase.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"

namespace bv { namespace model {

class ModelNodeEffectBlur : public ModelNodeEffectBase
{
private:

    ParamFloatPtr   m_paramBlurSize;

    float           m_blurSizeVal;

public:

                            ModelNodeEffectBlur     ( ITimeEvaluatorPtr timeEvaluator ); 

    static ISerializablePtr Create                  ( const IDeserializer& deser );

    virtual NodeEffectType  GetType                 () const override;

	virtual void			Update					( TimeType t ) override;

    ParamFloatPtr           GetParamBlurSize        () const;

    float                   GetBlurSize             () const;

};

DEFINE_PTR_TYPE(ModelNodeEffectBlur)
DEFINE_CONST_PTR_TYPE(ModelNodeEffectBlur)

} // model
} // bv
