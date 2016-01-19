#pragma once

#include "Engine/Models/NodeEffects/ModelNodeEffectBase.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"

namespace bv { namespace model {

class ModelNodeEffectBlur : public ModelNodeEffectBase
{
private:

    ParamFloatPtr   m_paramBlurSize;
    ParamIntPtr     m_paramNormalize;

    Float32         m_blurSizeVal;
    Int32           m_normalizeVal;

public:

                            ModelNodeEffectBlur     ( ITimeEvaluatorPtr timeEvaluator ); 

    static ISerializablePtr Create                  ( const IDeserializer& deser );

    virtual NodeEffectType  GetType                 () const override;

	virtual void			Update					( TimeType t ) override;

    //ParamFloatPtr           GetParamBlurSize        () const;

    //Float32                 GetBlurSize             () const;

    //ParamIntPtr             GetParamNormalize       () const;

    //Int32                   GetNormalize            () const;


};

DEFINE_PTR_TYPE(ModelNodeEffectBlur)
DEFINE_CONST_PTR_TYPE(ModelNodeEffectBlur)

} // model
} // bv
