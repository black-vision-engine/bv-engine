#include "stdafx.h"

#include "SpotLight.h"


namespace bv { namespace model {


const std::string       SpotLight::PARAM::COLOR          = "color";
const std::string       SpotLight::PARAM::DIRECTION      = "direction";
const std::string       SpotLight::PARAM::POSITION       = "position";

const std::string       SpotLight::PARAM::ATT_CONSTANT   = "attConstant";
const std::string       SpotLight::PARAM::ATT_LINEAR     = "attLinear";
const std::string       SpotLight::PARAM::ATT_QUADRATIC  = "attQuadratic";

const std::string       SpotLight::PARAM::CUT_OFF        = "cutOff";
const std::string       SpotLight::PARAM::OUTER_CUT_OFF  = "outerCutOff";


// *************************************
//
                                SpotLight::SpotLight          ( ITimeEvaluatorPtr timeEvaluator )
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

    m_colorParam = colorEvaluator->Parameter();
    m_positionParam = positionEvaluator->Parameter();
    m_attConstantParam = attConstantEvaluator->Parameter();
    m_attLinearParam = attLinearEvaluator->Parameter();
    m_attQuadraticParam = attQuadraticEvaluator->Parameter();
    m_cutOffParam = cutOffEvaluator->Parameter();
    m_outerCutOffParam = outerCutOffEvaluator->Parameter();


    m_colorParam->SetVal( glm::vec3( 1.0f, 1.0f, 1.0f ), 0.f );
    m_directionParam->SetVal( glm::vec3( -1.0f, 0.0f, 0.0f ), 0.f );
    m_positionParam->SetVal( glm::vec3( 0.0f, 0.0f, 0.0f ), 0.f );
    m_attConstantParam->SetVal( 1.0f, 0.f );
    m_attLinearParam->SetVal( 0.0f, 0.f );
    m_attQuadraticParam->SetVal( 0.02f, 0.f );
    m_cutOffParam->SetVal( glm::radians( 12.5f ), 0.f );
    m_outerCutOffParam->SetVal( glm::radians( 15.0f ), 0.f );


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
void                            SpotLight::Serialize             ( ISerializer & ser ) const
{
    { ser; }
}

// *************************************
//
void                            SpotLight::Update                ( TimeType t )
{
    { t; }
    m_paramModel->Update();
}

// *************************************
//
std::vector< IParameterPtr > &  SpotLight::GetParameters         ()
{
    return m_paramModel->GetParameters();
}

// *************************************
//
IParameterPtr                   SpotLight::GetParameter          ( const std::string & name )
{
    return m_paramModel->GetParameter( name );
}

// *************************************
//
const std::vector< IValueConstPtr > &  SpotLight::GetValues     () const
{
    return m_paramModel->GetValues();
}

// *************************************
//
IValueConstPtr                  SpotLight::GetValue             ( const std::string & name ) const
{
    return m_paramModel->GetValue( name );
}

// *************************************
//
LightType                       SpotLight::GetType              () const
{
    return LightType::LT_SPOT;
}

} //model
} //bv
