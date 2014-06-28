#include "BasicOverrideState.h"

#include "Engine/Types/Values/ValuesFactory.h"
#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"


namespace bv { namespace model {

// ****************************
//
BasicOverrideState::BasicOverrideState  ( ITimeEvaluatorPtr timeEvaluator )
    : m_param( nullptr )
    , m_value( nullptr )
    , m_enabled( false )
    , m_changed( false )
{
    m_param = ParametersFactory::CreateParameterFloat( "alpha", timeEvaluator );
    m_value = ValuesFactory::CreateValueFloat("alpha" );

    m_param->SetVal( 1.f, 0.f ); //Default model

    DisableAlpha();
}

// ****************************
//
BasicOverrideState::~BasicOverrideState ()
{}

// ****************************
//
void                BasicOverrideState::Update              ( TimeType t )
{
    //Update alpha
    if( IsAlphaEnabled() )
    {
        m_value->SetValue( m_param->Evaluate() );
    }
}

// ****************************
//
bool                BasicOverrideState::Changed             () const
{
    return m_changed;
}

// ****************************
//
void                BasicOverrideState::SetChanged          ( bool changed )
{
    m_changed = changed;
}

// ****************************
//
bool                BasicOverrideState::IsAlphaEnabled      () const
{
    return m_enabled;
}

// ****************************
//
void                BasicOverrideState::DisableAlpha        ()
{
    if( m_enabled )
    {
        SetChanged( true );
    }

    m_value->SetValue( 1.0f );

    m_enabled = false;
}

// ****************************
//
void                BasicOverrideState::EnableAlpha         ()
{
    if( !m_enabled )
    {
        SetChanged( true );
    }

    m_enabled = true;
}

// ****************************
//
IParameterPtr        BasicOverrideState::GetAlphaParam       ()
{
    return m_param;
}

// ****************************
//
IValueConstPtr      BasicOverrideState::GetAlphaValue       () const
{
    return m_value;
}

} //model
} //bv
