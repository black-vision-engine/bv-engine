#include "ModelNodeEffectBase.h"

#include "Serialization/SerializationHelper.h"

namespace bv { namespace model {

// ********************************
//
ModelNodeEffectBase::ModelNodeEffectBase  ( ITimeEvaluatorPtr timeEvaluator )
    : m_paramValModel( std::make_shared< DefaultParamValModel >() )
{
    { timeEvaluator; }
}

// ********************************
//
void            ModelNodeEffectBase::Serialize       ( ISerializer& ser ) const
{
ser.EnterChild( "effect" );
    ser.SetAttribute( "type", SerializationHelper::T2String< NodeEffectType >( GetType() ) );
    m_paramValModel->Serialize( ser );
ser.ExitChild();
}

// ********************************
//
void            ModelNodeEffectBase::Update  ( TimeType t )
{
    { t; }
    m_paramValModel->Update();
}

// ********************************
//
model::IParameterPtr                        ModelNodeEffectBase::GetParameter   ( const std::string & name ) const
{
    return m_paramValModel->GetParameter( name );
}

// ********************************
//
const std::vector< IParameterPtr > &        ModelNodeEffectBase::GetParameters   () const
{
    return m_paramValModel->GetParameters();
}

// ********************************
//
const std::vector< bv::IValueConstPtr > &   ModelNodeEffectBase::GetValues      () const
{
    return m_paramValModel->GetValues();
}

} // model
} // bv
