#pragma once

#include "Engine/Models/Lights/ModelBaseLight.h"


namespace bv { namespace model {


class ModelPointLight : public ModelBaseLight
{
private:

    struct PARAM {

        static const std::string COLOR;         // vec3
        static const std::string POSITION;      // vec3
        static const std::string ATTENUATION;   // vec3

    };

public:

                                ModelPointLight     ( ITimeEvaluatorPtr timeEvaluator );

    virtual LightType           GetType             () const override;

};

} //model
} //bv
