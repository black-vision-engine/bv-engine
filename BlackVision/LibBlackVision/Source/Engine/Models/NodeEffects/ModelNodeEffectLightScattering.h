#pragma once

#include "Engine/Models/Interfaces/IModelNodeEffect.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"


namespace bv { namespace model {

class ModelNodeEffectLightScattering : public IModelNodeEffect
{
private:
     ParamFloatPtr  m_paramExposure;
     ParamFloatPtr  m_paramWeight;
     ParamFloatPtr  m_paramDecay;
     ParamFloatPtr  m_paramDensity;
     ParamVec2Ptr   m_paramLightPositionOnScreen;
     ParamIntPtr    m_paramNumSamples;

     float          m_exposureVal;
     float          m_weightVal;
     float          m_decayVal;
     float          m_densityVal;
     glm::vec2      m_lightPositionOnScreenVal;
     int            m_numSamplesVal;

public:

                            ModelNodeEffectLightScattering( ITimeEvaluatorPtr timeEvaluator );

        
    ParamFloatPtr           GetParamExposure                ();
    ParamFloatPtr           GetParamWeight                  ();
    ParamFloatPtr           GetParamDecay                   ();
    ParamFloatPtr           GetParamDensity                 ();
    ParamVec2Ptr            GetParamLightPositionOnScreen   ();
    ParamIntPtr             GetParamNumSamples              ();         

    float                   GetExposure                     ();
    float                   GetWeight                       ();
    float                   GetDecay                        ();
    float                   GetDensity                      ();
    glm::vec2               GetLightPositionOnScreen        ();
    int                     GetNumSamples                   ();   

    virtual void            Serialize       ( ISerializer& ser ) const;
    static ISerializablePtr Create          ( const IDeserializer& deser );

    virtual void            Update              ( TimeType t ) override;

    virtual NodeEffectType  GetType             () const override;

};

DEFINE_PTR_TYPE(ModelNodeEffectLightScattering)
DEFINE_CONST_PTR_TYPE(ModelNodeEffectLightScattering)

} // model
} // bv
