#include "stdafx.h"

#include "DirectionalLight.h"


namespace bv { namespace model {


const std::string       DirectionalLight::PARAM::COLOR          = "color";
const std::string       DirectionalLight::PARAM::DIRECTION      = "direction";


// *************************************
//
                            DirectionalLight::DirectionalLight          ( ITimeEvaluatorPtr timeEvaluator )
{
    m_paramModel = std::make_shared< DefaultParamValModel >();
    
    auto colorEvaluator     = ParamValEvaluatorFactory::CreateSimpleVec3Evaluator( PARAM::COLOR, timeEvaluator );
    auto directionEvaluator = ParamValEvaluatorFactory::CreateSimpleVec3Evaluator( PARAM::DIRECTION, timeEvaluator );

    m_colorParam = colorEvaluator->Parameter();
    m_directionParam = directionEvaluator->Parameter();

    m_colorParam->SetVal( glm::vec3( 1.0f, 1.0f, 1.0f ), 0.f );
    m_directionParam->SetVal( glm::vec3( -1.0f, 0.0f, 0.0f ), 0.f );

    m_paramModel->RegisterAll( colorEvaluator );
    m_paramModel->RegisterAll( directionEvaluator );
}

// *************************************
//
void                            DirectionalLight::Serialize             ( ISerializer & ser ) const
{
    { ser; }
}

// *************************************
//
void                            DirectionalLight::Update                ( TimeType t )
{
    { t; }
    m_paramModel->Update();
}

// *************************************
//
std::vector< IParameterPtr > &  DirectionalLight::GetParameters         ()
{
    return m_paramModel->GetParameters();
}

// *************************************
//
IParameterPtr                   DirectionalLight::GetParameter          ( const std::string & name )
{
    return m_paramModel->GetParameter( name );
}

// *************************************
//
const std::vector< IValueConstPtr > &  DirectionalLight::GetValues      () const
{
    return m_paramModel->GetValues();
}

// *************************************
//
IValueConstPtr                  DirectionalLight::GetValue          ( const std::string & name ) const
{
    return m_paramModel->GetValue( name );
}

// *************************************
//
LightType                       DirectionalLight::GetType               () const
{
    return LightType::LT_DIRECTIONAL;
}

} //model
} //bv
