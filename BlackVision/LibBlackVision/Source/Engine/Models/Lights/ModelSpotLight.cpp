#include "stdafx.h"

#include "ModelSpotLight.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Types/Values/ValuesFactory.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {


const std::string       ModelSpotLight::PARAM::COLOR          = "color";
const std::string       ModelSpotLight::PARAM::DIRECTION      = "direction";
const std::string       ModelSpotLight::PARAM::POSITION       = "position";
const std::string       ModelSpotLight::PARAM::ATTENUATION    = "attenuation";

const std::string       ModelSpotLight::PARAM::CUT_OFF        = "cutOff";
const std::string       ModelSpotLight::PARAM::EXPONENT       = "exponent";


// *************************************
//
                        ModelSpotLight::ModelSpotLight          ( ITimeEvaluatorPtr timeEvaluator )
    : ModelBaseLight()
    , m_defaultDirection( glm::vec3( 0.0f, 0.0f, -1.0f ) )
    , m_directionAngles( glm::vec3( 1.0f ) )
    , m_directionVector( glm::vec3( 0.0f ) )
    , m_cutOffAngle( 0.0f )
    , m_cutOffCos( 0.0f )
{
    auto colorEvaluator         = ParamValEvaluatorFactory::CreateSimpleVec3Evaluator( PARAM::COLOR, timeEvaluator );
    m_directionEval             = ParamValEvaluatorFactory::CreateSimpleVec3Evaluator( PARAM::DIRECTION, timeEvaluator );
    auto positionEvaluator      = ParamValEvaluatorFactory::CreateSimpleVec3Evaluator( PARAM::POSITION, timeEvaluator );
    auto attenuationEvaluator   = ParamValEvaluatorFactory::CreateSimpleVec3Evaluator( PARAM::ATTENUATION, timeEvaluator );
    m_cutOffEval                = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( PARAM::CUT_OFF, timeEvaluator );
    auto exponentEval           = ParamValEvaluatorFactory::CreateSimpleIntEvaluator( PARAM::EXPONENT, timeEvaluator );
    
    colorEvaluator->Parameter()->SetVal( glm::vec3( 1.0f, 1.0f, 1.0f ), 0.f );
    m_directionEval->Parameter()->SetVal( glm::vec3( 0.0f ), 0.f );
    positionEvaluator->Parameter()->SetVal( glm::vec3( 0.0f, 0.0f, 0.0f ), 0.f );
    attenuationEvaluator->Parameter()->SetVal( glm::vec3( 1.0f, 0.0f, 0.02f ), 0.f );
    m_cutOffEval->Parameter()->SetVal( 100.0f, 0.f );
    exponentEval->Parameter()->SetVal( 10, 0.f );
    
    m_paramModel->RegisterAll( colorEvaluator );
    m_paramModel->RegisterAll( m_directionEval );
    m_paramModel->RegisterAll( positionEvaluator );
    m_paramModel->RegisterAll( attenuationEvaluator );
    m_paramModel->RegisterAll( m_cutOffEval );
    m_paramModel->RegisterAll( exponentEval );
}

// *************************************
//
void                    ModelSpotLight::Update                ( TimeType t )
{
    ModelBaseLight::Update( t );

    // update direction vector from angles
    auto direction = m_directionEval->Parameter()->Evaluate();
    if( direction != m_directionAngles )
    {
        m_directionVector = CalculateDirection( direction );
        m_directionAngles = direction;
    }
    m_directionEval->Value()->SetValue( m_directionVector );

    // update cut off angle to cosine
    auto cutOff = m_cutOffEval->Parameter()->Evaluate();
    if( cutOff != m_cutOffAngle )
    {
        m_cutOffCos = CalculateCosine( cutOff );
        m_cutOffAngle = cutOff;
    }
    m_cutOffEval->Value()->SetValue( m_cutOffCos );
}

// *************************************
//
LightType               ModelSpotLight::GetType             () const
{
    return LightType::LT_SPOT;
}

// *************************************
//
glm::vec3               ModelSpotLight::CalculateDirection  ( const glm::vec3 & angles ) const 
{
    return glm::orientate3( glm::radians( angles ) ) * m_defaultDirection;
}

// *************************************
//
Float32                 ModelSpotLight::CalculateCosine     ( Float32 angle ) const
{
    return glm::cos( glm::radians( angle ) );
}

// ***********************
//
void                    ModelSpotLight::UpdateToCameraSpace ( const glm::mat4 & viewMat )
{
    auto positionVal = QueryTypedValue< ValueVec3Ptr >( m_paramModel->GetValue( PARAM::POSITION ) );
    positionVal->SetValue( glm::vec3( viewMat * glm::vec4( positionVal->GetValue(), 1.0f ) ) );

    auto directionVal = QueryTypedValue< ValueVec3Ptr >( m_paramModel->GetValue( PARAM::DIRECTION ) );
    directionVal->SetValue( glm::vec3( viewMat * glm::vec4( directionVal->GetValue(), 0.0f ) ) );
}

} //model
} //bv
