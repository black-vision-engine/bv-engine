#pragma once

#include "Engine/Models/NodeEffects/ModelNodeEffectBase.h"

#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"


namespace bv { namespace model {

class ModelNodeEffectLightScattering : public ModelNodeEffectBase
{
private:

     ParamFloatPtr  m_paramExposure;
     ParamFloatPtr  m_paramWeight;
     ParamFloatPtr  m_paramDecay;
     ParamFloatPtr  m_paramDensity;
     ParamVec2Ptr   m_paramLightPositionOnScreen;
     ParamFloatPtr  m_paramNumSamples;

     float          m_exposureVal;
     float          m_weightVal;
     float          m_decayVal;
     float          m_densityVal;
     glm::vec2      m_lightPositionOnScreenVal;
     float          m_numSamplesVal;

public:

                            ModelNodeEffectLightScattering  ( ITimeEvaluatorPtr timeEvaluator );

        
    ParamFloatPtr           GetParamExposure                () const;
    ParamFloatPtr           GetParamWeight                  () const;
    ParamFloatPtr           GetParamDecay                   () const;
    ParamFloatPtr           GetParamDensity                 () const;
    ParamVec2Ptr            GetParamLightPositionOnScreen   () const;
    ParamFloatPtr           GetParamNumSamples              () const;         

    float                   GetExposure                     () const;
    float                   GetWeight                       () const;
    float                   GetDecay                        () const;
    float                   GetDensity                      () const;
    glm::vec2               GetLightPositionOnScreen        () const;
    float                   GetNumSamples                   () const;   

    static ISerializablePtr Create              ( const IDeserializer& deser );

    virtual void            Update              ( TimeType t ) override;

    virtual NodeEffectType  GetType             () const override;

};

DEFINE_PTR_TYPE(ModelNodeEffectLightScattering)
DEFINE_CONST_PTR_TYPE(ModelNodeEffectLightScattering)

} // model
} // bv
