#pragma once

#include "Engine/Models/Lights/ModelBaseLight.h"


namespace bv { namespace model {


class ModelPointLight : public ModelBaseLight
{
private:

    struct PARAM {

        static const std::string COLOR;         // vec3
        static const std::string POSITION;      // vec3
        
        static const std::string ATT_CONSTANT;  // float
        static const std::string ATT_LINEAR;    // float
        static const std::string ATT_QUADRATIC; // float

    };

public:

                                ModelPointLight     ( ITimeEvaluatorPtr timeEvaluator );

    virtual std::string         GetTypeName         () const override;
    virtual LightType           GetType             () const override;

};

} //model
} //bv
