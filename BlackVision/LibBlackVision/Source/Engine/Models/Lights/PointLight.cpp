#include "stdafx.h"

#include "PointLight.h"


namespace bv { namespace model {


const std::string       PointLight::PARAM::COLOR          = "color";
const std::string       PointLight::PARAM::POSITION       = "position";

const std::string       PointLight::PARAM::ATT_CONSTANT   = "attConstant";
const std::string       PointLight::PARAM::ATT_LINEAR     = "attLinear";
const std::string       PointLight::PARAM::ATT_QUADRATIC  = "attQuadratic";


// *************************************
//
                            PointLight::PointLight          ( ITimeEvaluatorPtr timeEvaluator )
{
    m_paramModel = std::make_shared< DefaultParamValModel >();
    
    auto colorEvaluator     = ParamValEvaluatorFactory::CreateSimpleVec3Evaluator( PARAM::COLOR, timeEvaluator );
    auto positionEvaluator = ParamValEvaluatorFactory::CreateSimpleVec3Evaluator( PARAM::POSITION, timeEvaluator );
    auto attConstantEvaluator   = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( PARAM::ATT_CONSTANT, timeEvaluator );
    auto attLinearEvaluator     = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( PARAM::ATT_LINEAR, timeEvaluator );
    auto attQuadraticEvaluator  = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( PARAM::ATT_QUADRATIC, timeEvaluator );


    m_colorParam = colorEvaluator->Parameter();
    m_positionParam = positionEvaluator->Parameter();
    m_attConstantParam = attConstantEvaluator->Parameter();
    m_attLinearParam = attLinearEvaluator->Parameter();
    m_attQuadraticParam = attQuadraticEvaluator->Parameter();


    m_colorParam->SetVal( glm::vec3( 1.0f, 1.0f, 1.0f ), 0.f );
    m_positionParam->SetVal( glm::vec3( 0.0f, 0.0f, 0.0f ), 0.f );
    m_attConstantParam->SetVal( 1.0f, 0.f );
    m_attLinearParam->SetVal( 0.0f, 0.f );
    m_attQuadraticParam->SetVal( 0.02f, 0.f );

    m_paramModel->RegisterAll( colorEvaluator );
    m_paramModel->RegisterAll( positionEvaluator );
    m_paramModel->RegisterAll( attConstantEvaluator );
    m_paramModel->RegisterAll( attLinearEvaluator );
    m_paramModel->RegisterAll( attQuadraticEvaluator );
}

// *************************************
//
void                            PointLight::Serialize           ( ISerializer & ser ) const
{
    { ser; }
}

// *************************************
//
void                            PointLight::Update              ( TimeType t )
{
    { t; }
    m_paramModel->Update();
}

// *************************************
//
std::vector< IParameterPtr > &  PointLight::GetParameters       ()
{
    return m_paramModel->GetParameters();
}

// *************************************
//
IParameterPtr                   PointLight::GetParameter        ( const std::string & name )
{
    return m_paramModel->GetParameter( name );
}

// *************************************
//
const std::vector< IValueConstPtr > &  PointLight::GetValues    () const
{
    return m_paramModel->GetValues();
}

// *************************************
//
IValueConstPtr                  PointLight::GetValue            ( const std::string & name ) const
{
    return m_paramModel->GetValue( name );
}

// *************************************
//
LightType                       PointLight::GetType             () const
{
    return LightType::LT_POINT;
}

} //model
} //bv
