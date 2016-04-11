#pragma once

#include "Engine/Models/Lights/ModelBaseLight.h"


namespace bv { namespace model {


class ModelSpotLight : public ModelBaseLight
{
private:

    // direction
    SimpleVec3EvaluatorPtr      m_directionEval;    //in - angles in degrees, out - direction vector  
    glm::vec3                   m_directionAngles;
    glm::vec3                   m_directionVector;
    glm::vec3                   m_defaultDirection;

    // cut off
    SimpleFloatEvaluatorPtr     m_cutOffEval;       //in - angle in degrees, out - cosine of angle
    Float32                     m_cutOffAngle;
    Float32                     m_cutOffCos;

    struct PARAM {

        static const std::string COLOR;         // vec3
        static const std::string DIRECTION;     // vec3
        static const std::string POSITION;      // vec3
        static const std::string ATTENUATION;   // vec3
        static const std::string CUT_OFF;       // float
        static const std::string EXPONENT;      // float

    };

public:

                                ModelSpotLight      ( ITimeEvaluatorPtr timeEvaluator );

    virtual void				Update			    ( TimeType t )	override;

    virtual LightType           GetType             () const override;

private:

    glm::vec3                   CalculateDirection  ( const glm::vec3 & angles ) const;
    Float32                     CalculateCosine     ( Float32 angle ) const;

};

} //model
} //bv
