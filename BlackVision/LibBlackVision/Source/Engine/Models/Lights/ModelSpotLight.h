#pragma once

#include "Mathematics/glm_inc.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Interfaces/IModelLight.h"
#include "Serialization/SerializationHelper.h"


namespace bv { namespace model {


class ModelSpotLight : public IModelLight
{
private:

    DefaultParamValModelPtr     m_paramModel;

    struct PARAM {

        static const std::string COLOR;         // vec3
        static const std::string DIRECTION;     // vec3
        static const std::string POSITION;      // vec3

        static const std::string ATT_CONSTANT;  // float
        static const std::string ATT_LINEAR;    // float
        static const std::string ATT_QUADRATIC; // float

        static const std::string CUT_OFF;       // float
        static const std::string OUTER_CUT_OFF; // float

    };

public:

                                ModelSpotLight      ( ITimeEvaluatorPtr timeEvaluator );

    virtual void                Serialize           ( ISerializer & ser ) const override;

    virtual void				Update			    ( TimeType t )	override;
    
    virtual std::vector< IParameterPtr > &          GetParameters   () override;
    virtual IParameterPtr                           GetParameter    ( const std::string & name ) override;

    virtual const std::vector< IValueConstPtr > &   GetValues       () const override;
    virtual IValueConstPtr                          GetValue        ( const std::string & name ) const override;

    virtual std::string             GetTypeName         () const override;
    virtual LightType               GetType             () const override;

};

} //model
} //bv
