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

    glm::vec4       m_colorVal;
    glm::vec2       m_shiftVal;
    float           m_blurSizeVal;

public:

                            ModelNodeEffectShadow   ( ITimeEvaluatorPtr timeEvaluator ); 

    static ISerializablePtr Create                  ( const IDeserializer& deser );

    virtual NodeEffectType  GetType                 () const override;

	virtual void			Update					( TimeType t ) override;

    ParamVec4Ptr            GetParamColor           () const;
    ParamVec2Ptr            GetParamShift           () const;
    ParamFloatPtr           GetParamBlurSize        () const;

    glm::vec4               GetColor                () const;
    glm::vec2               GetShift                () const;
    float                   GetBlurSize             () const;

};

DEFINE_PTR_TYPE(ModelNodeEffectShadow)
DEFINE_CONST_PTR_TYPE(ModelNodeEffectShadow)

} // model
} // bv
