#include "stdafx.h"

#include "ModelSpotLight.h"

#include <glm/gtx/euler_angles.hpp>

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Types/Values/ValuesFactory.h"


namespace bv { namespace model {


const std::string       ModelSpotLight::PARAM::COLOR          = "color";
const std::string       ModelSpotLight::PARAM::DIRECTION      = "direction";
const std::string       ModelSpotLight::PARAM::POSITION       = "position";

const std::string       ModelSpotLight::PARAM::ATT_CONSTANT   = "attConstant";
const std::string       ModelSpotLight::PARAM::ATT_LINEAR     = "attLinear";
const std::string       ModelSpotLight::PARAM::ATT_QUADRATIC  = "attQuadratic";

const std::string       ModelSpotLight::PARAM::CUT_OFF        = "cutOff";
const std::string       ModelSpotLight::PARAM::OUTER_CUT_OFF  = "outerCutOff";


// *************************************
//
                        ModelSpotLight::ModelSpotLight          ( ITimeEvaluatorPtr timeEvaluator )
    : ModelBaseLight()
    , m_defaultDirection( glm::vec3( 0.0f, 0.0f, -1.0f ) )
    , m_directionAngles( glm::vec3( 1.0f ) )
    , m_directionVector( glm::vec3( 0.0f ) )
    , m_cutOffAngle( 0.0f )
    , m_cutOffCos( 0.0f )
    , m_outerCutOffAngle( 0.0f )
    , m_outerCutOffCos( 0.0f )
{
    auto colorEvaluator         = ParamValEvaluatorFactory::CreateSimpleVec3Evaluator( PARAM::COLOR, timeEvaluator );
    m_directionEval             = ParamValEvaluatorFactory::CreateSimpleVec3Evaluator( PARAM::DIRECTION, timeEvaluator );
    auto positionEvaluator      = ParamValEvaluatorFactory::CreateSimpleVec3Evaluator( PARAM::POSITION, timeEvaluator );
    auto attConstantEvaluator   = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( PARAM::ATT_CONSTANT, timeEvaluator );
    auto attLinearEvaluator     = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( PARAM::ATT_LINEAR, timeEvaluator );
    auto attQuadraticEvaluator  = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( PARAM::ATT_QUADRATIC, timeEvaluator );
    m_cutOffEval                = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( PARAM::CUT_OFF, timeEvaluator );
    m_outerCutOffEval           = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( PARAM::OUTER_CUT_OFF, timeEvaluator );
    
    colorEvaluator->Parameter()->SetVal( glm::vec3( 1.0f, 1.0f, 1.0f ), 0.f );
    m_directionEval->Parameter()->SetVal( glm::vec3( 0.0f ), 0.f );
    positionEvaluator->Parameter()->SetVal( glm::vec3( 0.0f, 0.0f, 0.0f ), 0.f );
    attConstantEvaluator->Parameter()->SetVal( 1.0f, 0.f );
    attLinearEvaluator->Parameter()->SetVal( 0.0f, 0.f );
    attQuadraticEvaluator->Parameter()->SetVal( 0.0f, 0.f );
    m_cutOffEval->Parameter()->SetVal( 15.0f, 0.f );
    m_outerCutOffEval->Parameter()->SetVal( 25.0f, 0.f );
    
    m_paramModel->RegisterAll( colorEvaluator );
    m_paramModel->RegisterAll( m_directionEval );
    m_paramModel->RegisterAll( positionEvaluator );
    m_paramModel->RegisterAll( attConstantEvaluator );
    m_paramModel->RegisterAll( attLinearEvaluator );
    m_paramModel->RegisterAll( attQuadraticEvaluator );
    m_paramModel->RegisterAll( m_cutOffEval );
    m_paramModel->RegisterAll( m_outerCutOffEval );
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

    // update outer cut off angle to cosine
    auto outerCutOff = m_outerCutOffEval->Parameter()->Evaluate();
    if( outerCutOff != m_outerCutOffAngle )
    {
        m_outerCutOffCos = CalculateCosine( outerCutOff );
        m_outerCutOffAngle = outerCutOff;
    }
    m_outerCutOffEval->Value()->SetValue( m_outerCutOffCos );
}

// *************************************
//
std::string             ModelSpotLight::GetTypeName         () const
{
    return "spotLight";
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

} //model
} //bv
