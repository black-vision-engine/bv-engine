#include "stdafx.h"

#include "ModelDirectionalLight.h"
#include "Mathematics/glm_inc.h"


#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {


const std::string       ModelDirectionalLight::PARAM::COLOR          = "color";
const std::string       ModelDirectionalLight::PARAM::DIRECTION      = "direction";


// *************************************
//
                        ModelDirectionalLight::ModelDirectionalLight          ( ITimeEvaluatorPtr timeEvaluator )
    : ModelBaseLight()
    , m_defaultDirection( glm::vec3( 0.0f, 0.0f, -1.0f ) )
    , m_directionAngles( glm::vec3( 1.0f ) )
    , m_directionVector( glm::vec3( 0.0f ) )
{
    auto colorEvaluator     = ParamValEvaluatorFactory::CreateSimpleVec3Evaluator( PARAM::COLOR, timeEvaluator );
    m_directionEval         = ParamValEvaluatorFactory::CreateSimpleVec3Evaluator( PARAM::DIRECTION, timeEvaluator );

    colorEvaluator->Parameter()->SetVal( glm::vec3( 1.0f, 1.0f, 1.0f ), 0.f );
    m_directionEval->Parameter()->SetVal( glm::vec3( 0.0f ), 0.f );

    m_paramModel->RegisterAll( colorEvaluator );
    m_paramModel->RegisterAll( m_directionEval );
}

                        
// *************************************
//
void                    ModelDirectionalLight::Update                ( TimeType t )
{
    ModelBaseLight::Update( t );

    // compute direction vector from angles
    auto direction = m_directionEval->Parameter()->Evaluate();
    if( direction != m_directionAngles )
    {
        m_directionVector = CalculateDirection( direction );
        m_directionAngles = direction;
    }
    m_directionEval->Value()->SetValue( m_directionVector );
}

// *************************************
//
LightType               ModelDirectionalLight::GetType          () const
{
    return LightType::LT_DIRECTIONAL;
}

// *************************************
//
glm::vec3               ModelDirectionalLight::CalculateDirection   ( const glm::vec3 & angles ) const 
{
    return glm::orientate3( glm::radians( angles ) ) * m_defaultDirection;
}

// ***********************
//
void                    ModelDirectionalLight::UpdateToCameraSpace ( const glm::mat4 & viewMat )
{
    auto directionVal = QueryTypedValue< ValueVec3Ptr >( m_paramModel->GetValue( PARAM::DIRECTION ) );
    directionVal->SetValue( glm::vec3( viewMat * glm::vec4( directionVal->GetValue(), 0.0f ) ) );
}

} //model
} //bv
