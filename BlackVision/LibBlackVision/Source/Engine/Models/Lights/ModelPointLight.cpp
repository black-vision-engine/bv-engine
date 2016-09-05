#include "stdafx.h"

#include "ModelPointLight.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {


const std::string       ModelPointLight::PARAM::COLOR          = "color";
const std::string       ModelPointLight::PARAM::POSITION       = "position";
const std::string       ModelPointLight::PARAM::ATTENUATION    = "attenuation";


// *************************************
//
                        ModelPointLight::ModelPointLight          ( ITimeEvaluatorPtr timeEvaluator )
    : ModelBaseLight()
{
    auto colorEvaluator     = ParamValEvaluatorFactory::CreateSimpleVec3Evaluator( PARAM::COLOR, timeEvaluator );
    auto positionEvaluator = ParamValEvaluatorFactory::CreateSimpleVec3Evaluator( PARAM::POSITION, timeEvaluator );
    auto attenuationEvaluator   = ParamValEvaluatorFactory::CreateSimpleVec3Evaluator( PARAM::ATTENUATION, timeEvaluator );

    colorEvaluator->Parameter()->SetVal( glm::vec3( 1.0f, 1.0f, 1.0f ), 0.f );
    positionEvaluator->Parameter()->SetVal( glm::vec3( 0.0f, 0.0f, 0.0f ), 0.f );
    attenuationEvaluator->Parameter()->SetVal( glm::vec3( 1.0f, 0.0f, 0.02f ), 0.f );

    m_paramModel->RegisterAll( colorEvaluator );
    m_paramModel->RegisterAll( positionEvaluator );
    m_paramModel->RegisterAll( attenuationEvaluator );
}

// *************************************
//
LightType               ModelPointLight::GetType             () const
{
    return LightType::LT_POINT;
}

// ***********************
//
void                    ModelPointLight::UpdateToCameraSpace ( const glm::mat4 & viewMat )
{
    auto positionVal = QueryTypedValue< ValueVec3Ptr >( m_paramModel->GetValue( PARAM::POSITION ) );
    positionVal->SetValue( glm::vec3( viewMat * glm::vec4( positionVal->GetValue(), 1.0f ) ) );
}

} //model
} //bv
