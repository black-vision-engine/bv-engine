#pragma once

#include "Engine/Models/Lights/ModelBaseLight.h"


namespace bv { namespace model {


class ModelDirectionalLight : public ModelBaseLight
{
private:

    // direction
    SimpleVec3EvaluatorPtr  m_directionEval;        //in - angles in degrees, out - direction vector
    glm::vec3               m_directionAngles;
    glm::vec3               m_directionVector;
    glm::vec3               m_defaultDirection;

    struct PARAM {

        static const std::string COLOR;     // vec3
        static const std::string DIRECTION; // vec3

    };

public:

                                ModelDirectionalLight   ( ITimeEvaluatorPtr timeEvaluator );

    virtual void				Update			        ( TimeType t )	override;

    virtual std::string         GetTypeName             () const override;
    virtual LightType           GetType                 () const override;

private:

    glm::vec3                   CalculateDirection      ( const glm::vec3 & angles ) const;

};

} //model
} //bv
