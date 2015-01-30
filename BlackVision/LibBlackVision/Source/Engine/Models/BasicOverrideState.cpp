#include "BasicOverrideState.h"

#include "Engine/Types/Values/ValuesFactory.h"
#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"


namespace bv { namespace model {

// ****************************
//
BasicOverrideState::BasicOverrideState  ( ITimeEvaluatorPtr timeEvaluator )
    : m_param( nullptr )
    , m_value( nullptr )
    , m_enabledAM( false )
    , m_enabledNM( false )
    , m_changedAM( false )
    , m_changedNM( false )
{
    m_param = ParametersFactory::CreateParameterFloat( "alpha", timeEvaluator );
    m_value = ValuesFactory::CreateValueFloat("alpha" );

    m_param->SetVal( 1.f, 0.f ); //Default model

    DisableAlphaAM();
    DisableAlphaNM();
}

// ****************************
//
BasicOverrideState::~BasicOverrideState ()
{}

// ****************************
//
void                BasicOverrideState::Update              ( TimeType t )
{
    { t; } // FIXME: suppress unused variable
    //Update alpha
    if( IsAlphaEnabledAM() || IsAlphaEnabledNM() )
    {
        m_value->SetValue( m_param->Evaluate() );
    }
}

// ****************************
//
bool                BasicOverrideState::ChangedAM           () const
{
    return m_changedAM;
}

// ****************************
//
bool                BasicOverrideState::ChangedNM           () const
{
    return m_changedNM;
}

// ****************************
//
void                BasicOverrideState::SetChangedAM        ( bool changed )
{
    m_changedAM = changed;
}

// ****************************
//
void                BasicOverrideState::SetChangedNM        ( bool changed )
{
    m_changedNM = changed;
}

// ****************************
//
bool                BasicOverrideState::IsAlphaEnabledAM    () const
{
    return m_enabledAM;
}

// ****************************
//
bool                BasicOverrideState::IsAlphaEnabledNM    () const
{
    return m_enabledNM;
}

// ****************************
//
void                BasicOverrideState::EnableAlphaAM       ()
{
    if( !m_enabledAM )
    {
        SetChangedAM( true );
    }

    m_enabledAM = true;
}

// ****************************
//
void                BasicOverrideState::EnableAlphaNM       ()
{
    if( !m_enabledNM )
    {
        SetChangedNM( true );
    }

    m_enabledNM = true;
}

// ****************************
//
void                BasicOverrideState::DisableAlphaAM      ()
{
    if( m_enabledAM )
    {
        SetChangedAM( true );
    }

    m_value->SetValue( 1.0f );

    m_enabledAM = false;
}

// ****************************
//
void                BasicOverrideState::DisableAlphaNM      ()
{
    if( m_enabledNM )
    {
        SetChangedNM( true );
    }

    m_value->SetValue( 1.0f );

    m_enabledNM = false;
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
