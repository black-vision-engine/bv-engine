#include "stdafx.h"

#include "ModelPointLight.h"


namespace bv { namespace model {


const std::string       ModelPointLight::PARAM::COLOR          = "color";
const std::string       ModelPointLight::PARAM::POSITION       = "position";

const std::string       ModelPointLight::PARAM::ATT_CONSTANT   = "attConstant";
const std::string       ModelPointLight::PARAM::ATT_LINEAR     = "attLinear";
const std::string       ModelPointLight::PARAM::ATT_QUADRATIC  = "attQuadratic";


// *************************************
//
                        ModelPointLight::ModelPointLight          ( ITimeEvaluatorPtr timeEvaluator )
{
    m_paramModel = std::make_shared< DefaultParamValModel >();
    
    auto colorEvaluator     = ParamValEvaluatorFactory::CreateSimpleVec3Evaluator( PARAM::COLOR, timeEvaluator );
    auto positionEvaluator = ParamValEvaluatorFactory::CreateSimpleVec3Evaluator( PARAM::POSITION, timeEvaluator );
    auto attConstantEvaluator   = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( PARAM::ATT_CONSTANT, timeEvaluator );
    auto attLinearEvaluator     = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( PARAM::ATT_LINEAR, timeEvaluator );
    auto attQuadraticEvaluator  = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( PARAM::ATT_QUADRATIC, timeEvaluator );

    colorEvaluator->Parameter()->SetVal( glm::vec3( 1.0f, 1.0f, 1.0f ), 0.f );
    positionEvaluator->Parameter()->SetVal( glm::vec3( 0.0f, 0.0f, 0.0f ), 0.f );
    attConstantEvaluator->Parameter()->SetVal( 1.0f, 0.f );
    attLinearEvaluator->Parameter()->SetVal( 0.0f, 0.f );
    attQuadraticEvaluator->Parameter()->SetVal( 0.02f, 0.f );

    m_paramModel->RegisterAll( colorEvaluator );
    m_paramModel->RegisterAll( positionEvaluator );
    m_paramModel->RegisterAll( attConstantEvaluator );
    m_paramModel->RegisterAll( attLinearEvaluator );
    m_paramModel->RegisterAll( attQuadraticEvaluator );
}

// *************************************
//
void                    ModelPointLight::Serialize           ( ISerializer & ser ) const
{
    { ser; }
}

// *************************************
//
void                    ModelPointLight::Update              ( TimeType t )
{
    { t; }
    m_paramModel->Update();
}

// *************************************
//
std::vector< IParameterPtr > &  ModelPointLight::GetParameters       ()
{
    return m_paramModel->GetParameters();
}

// *************************************
//
IParameterPtr                   ModelPointLight::GetParameter        ( const std::string & name )
{
    return m_paramModel->GetParameter( name );
}

// *************************************
//
const std::vector< IValueConstPtr > &  ModelPointLight::GetValues    () const
{
    return m_paramModel->GetValues();
}

// *************************************
//
IValueConstPtr          ModelPointLight::GetValue            ( const std::string & name ) const
{
    return m_paramModel->GetValue( name );
}

// *************************************
//
std::string             ModelPointLight::GetTypeName         () const
{
    return "pointLight";
}

// *************************************
//
LightType               ModelPointLight::GetType             () const
{
    return LightType::LT_POINT;
}

} //model
} //bv
