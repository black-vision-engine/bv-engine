#include "stdafx.h"

#include "ModelDirectionalLight.h"


namespace bv { namespace model {


const std::string       ModelDirectionalLight::PARAM::COLOR          = "color";
const std::string       ModelDirectionalLight::PARAM::DIRECTION      = "direction";


// *************************************
//
                        ModelDirectionalLight::ModelDirectionalLight          ( ITimeEvaluatorPtr timeEvaluator )
{
    m_paramModel = std::make_shared< DefaultParamValModel >();
    
    auto colorEvaluator     = ParamValEvaluatorFactory::CreateSimpleVec3Evaluator( PARAM::COLOR, timeEvaluator );
    auto directionEvaluator = ParamValEvaluatorFactory::CreateSimpleVec3Evaluator( PARAM::DIRECTION, timeEvaluator );

    colorEvaluator->Parameter()->SetVal( glm::vec3( 1.0f, 1.0f, 1.0f ), 0.f );
    directionEvaluator->Parameter()->SetVal( glm::vec3( -1.0f, 0.0f, 0.0f ), 0.f );

    m_paramModel->RegisterAll( colorEvaluator );
    m_paramModel->RegisterAll( directionEvaluator );
}

// *************************************
//
void                    ModelDirectionalLight::Serialize             ( ISerializer & ser ) const
{
    { ser; }
}

// *************************************
//
void                    ModelDirectionalLight::Update                ( TimeType t )
{
    { t; }
    m_paramModel->Update();
}

// *************************************
//
std::vector< IParameterPtr > &  ModelDirectionalLight::GetParameters         ()
{
    return m_paramModel->GetParameters();
}

// *************************************
//
IParameterPtr           ModelDirectionalLight::GetParameter          ( const std::string & name )
{
    return m_paramModel->GetParameter( name );
}

// *************************************
//
const std::vector< IValueConstPtr > &  ModelDirectionalLight::GetValues      () const
{
    return m_paramModel->GetValues();
}

// *************************************
//
IValueConstPtr          ModelDirectionalLight::GetValue          ( const std::string & name ) const
{
    return m_paramModel->GetValue( name );
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
