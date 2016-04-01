#pragma once

#include "Mathematics/glm_inc.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Interfaces/IModelLight.h"
#include "Serialization/SerializationHelper.h"


namespace bv { namespace model {


class PointLight : public IModelLight
{
private:

    DefaultParamValModelPtr     m_paramModel;

    ParamVec3Ptr                m_colorParam;
    ParamVec3Ptr                m_positionParam;

    ParamFloatPtr               m_attConstantParam;
    ParamFloatPtr               m_attLinearParam;
    ParamFloatPtr               m_attQuadraticParam;

    struct PARAM {

        static const std::string COLOR;         // vec3
        static const std::string POSITION;      // vec3
        
        static const std::string ATT_CONSTANT;  // float
        static const std::string ATT_LINEAR;    // float
        static const std::string ATT_QUADRATIC; // float

    };

public:

                                PointLight          ( ITimeEvaluatorPtr timeEvaluator );

    virtual void                Serialize           ( ISerializer & ser ) const override;

    virtual void				Update			    ( TimeType t )	override;
    
    virtual std::vector< IParameterPtr > &          GetParameters   () override;
    virtual IParameterPtr                           GetParameter    ( const std::string & name ) override;

    virtual const std::vector< IValueConstPtr > &   GetValues       () const override;
    virtual IValueConstPtr                          GetValue        ( const std::string & name ) const override;

    virtual LightType           GetType             () const override;

};

} //model
} //bv
