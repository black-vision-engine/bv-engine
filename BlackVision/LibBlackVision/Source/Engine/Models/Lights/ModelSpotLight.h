#pragma once

#include "Engine/Models/Lights/ModelBaseLight.h"


namespace bv { namespace model {


class ModelSpotLight : public ModelBaseLight
{
private:

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

    virtual std::string         GetTypeName         () const override;
    virtual LightType           GetType             () const override;

};

} //model
} //bv
