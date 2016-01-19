#pragma once

#include "Engine/Models/NodeEffects/ModelNodeEffectBase.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"

namespace bv { namespace model {

class ModelNodeEffectShadow : public ModelNodeEffectBase
{
private:

    ParamVec4Ptr    m_paramColor;
    ParamVec2Ptr    m_paramShift;
    ParamFloatPtr   m_paramBlurSize;
    ParamIntPtr     m_paramNormalize;

    glm::vec4       m_colorVal;
    glm::vec2       m_shiftVal;
    Float32         m_blurSizeVal;
    Int32           m_normalizeVal;

public:

                            ModelNodeEffectShadow   ( ITimeEvaluatorPtr timeEvaluator ); 

    static ISerializablePtr Create                  ( const IDeserializer& deser );

    virtual NodeEffectType  GetType                 () const override;

	virtual void			Update					( TimeType t ) override;

    //ParamVec4Ptr            GetParamColor           () const;
    //ParamVec2Ptr            GetParamShift           () const;
    //ParamFloatPtr           GetParamBlurSize        () const;
    //ParamIntPtr             GetParamNormalize       () const;

    //glm::vec4               GetColor                () const;
    //glm::vec2               GetShift                () const;
    //Float32                 GetBlurSize             () const;
    //Int32                   GetNormalize            () const;

};

DEFINE_PTR_TYPE(ModelNodeEffectShadow)
DEFINE_CONST_PTR_TYPE(ModelNodeEffectShadow)

} // model
} // bv
