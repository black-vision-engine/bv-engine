#include "stdafx.h"

#include "ModelDirectionalLight.h"


namespace bv { namespace model {


const std::string       ModelDirectionalLight::PARAM::COLOR          = "color";
const std::string       ModelDirectionalLight::PARAM::DIRECTION      = "direction";


// *************************************
//
                        ModelDirectionalLight::ModelDirectionalLight          ( ITimeEvaluatorPtr timeEvaluator )
    : ModelBaseLight()
{
    auto colorEvaluator     = ParamValEvaluatorFactory::CreateSimpleVec3Evaluator( PARAM::COLOR, timeEvaluator );
    auto directionEvaluator = ParamValEvaluatorFactory::CreateSimpleVec3Evaluator( PARAM::DIRECTION, timeEvaluator );

    colorEvaluator->Parameter()->SetVal( glm::vec3( 1.0f, 1.0f, 1.0f ), 0.f );
    directionEvaluator->Parameter()->SetVal( glm::vec3( 0.0f, 0.0f, -1.0f ), 0.f );

    m_paramModel->RegisterAll( colorEvaluator );
    m_paramModel->RegisterAll( directionEvaluator );
}

// *************************************
//
std::string             ModelDirectionalLight::GetTypeName      () const
{
    return "directionalLight";
}

// *************************************
//
LightType               ModelDirectionalLight::GetType          () const
{
    return LightType::LT_DIRECTIONAL;
}

} //model
} //bv
