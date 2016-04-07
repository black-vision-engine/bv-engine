#include "stdafx.h"

#include "ModelSpotLight.h"


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
{
    m_paramModel = std::make_shared< DefaultParamValModel >();
    
    auto colorEvaluator     = ParamValEvaluatorFactory::CreateSimpleVec3Evaluator( PARAM::COLOR, timeEvaluator );
    auto directionEvaluator = ParamValEvaluatorFactory::CreateSimpleVec3Evaluator( PARAM::DIRECTION, timeEvaluator );
    auto positionEvaluator = ParamValEvaluatorFactory::CreateSimpleVec3Evaluator( PARAM::POSITION, timeEvaluator );
    auto attConstantEvaluator   = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( PARAM::ATT_CONSTANT, timeEvaluator );
    auto attLinearEvaluator     = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( PARAM::ATT_LINEAR, timeEvaluator );
    auto attQuadraticEvaluator  = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( PARAM::ATT_QUADRATIC, timeEvaluator );
    auto cutOffEvaluator        = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( PARAM::CUT_OFF, timeEvaluator );
    auto outerCutOffEvaluator   = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( PARAM::OUTER_CUT_OFF, timeEvaluator );

    colorEvaluator->Parameter()->SetVal( glm::vec3( 1.0f, 1.0f, 1.0f ), 0.f );
    directionEvaluator->Parameter()->SetVal( glm::vec3( 0.0f, 0.0f, -1.0f ), 0.f );
    positionEvaluator->Parameter()->SetVal( glm::vec3( 0.0f, 0.0f, 0.0f ), 0.f );
    attConstantEvaluator->Parameter()->SetVal( 1.0f, 0.f );
    attLinearEvaluator->Parameter()->SetVal( 0.0f, 0.f );
    attQuadraticEvaluator->Parameter()->SetVal( 0.0f, 0.f );
    cutOffEvaluator->Parameter()->SetVal( glm::cos( glm::radians( 15.0f ) ), 0.f );
    outerCutOffEvaluator->Parameter()->SetVal( glm::cos( glm::radians( 25.0f ) ), 0.f );
    
    m_paramModel->RegisterAll( colorEvaluator );
    m_paramModel->RegisterAll( directionEvaluator );
    m_paramModel->RegisterAll( positionEvaluator );
    m_paramModel->RegisterAll( attConstantEvaluator );
    m_paramModel->RegisterAll( attLinearEvaluator );
    m_paramModel->RegisterAll( attQuadraticEvaluator );
    m_paramModel->RegisterAll( cutOffEvaluator );
    m_paramModel->RegisterAll( outerCutOffEvaluator );
}

// *************************************
//
void                    ModelSpotLight::Serialize             ( ISerializer & ser ) const
{
    { ser; }
}

// *************************************
//
void                    ModelSpotLight::Update                ( TimeType t )
{
    { t; }
    m_paramModel->Update();
}

// *************************************
//
std::vector< IParameterPtr > &  ModelSpotLight::GetParameters         ()
{
    return m_paramModel->GetParameters();
}

// *************************************
//
IParameterPtr           ModelSpotLight::GetParameter          ( const std::string & name )
{
    return m_paramModel->GetParameter( name );
}

// *************************************
//
const std::vector< IValueConstPtr > &  ModelSpotLight::GetValues     () const
{
    return m_paramModel->GetValues();
}

// *************************************
//
IValueConstPtr          ModelSpotLight::GetValue            ( const std::string & name ) const
{
    return m_paramModel->GetValue( name );
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

} //model
} //bv
