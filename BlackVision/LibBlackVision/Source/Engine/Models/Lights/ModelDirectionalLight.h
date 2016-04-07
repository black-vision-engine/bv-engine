#pragma once

#include "Engine/Models/Lights/ModelBaseLight.h"


namespace bv { namespace model {


class ModelDirectionalLight : public ModelBaseLight
{
private:

    struct PARAM {

        static const std::string COLOR;     // vec3
        static const std::string DIRECTION; // vec3

    };

public:

                                ModelDirectionalLight   ( ITimeEvaluatorPtr timeEvaluator );

    virtual std::string         GetTypeName             () const override;
    virtual LightType           GetType                 () const override;

};

} //model
} //bv
