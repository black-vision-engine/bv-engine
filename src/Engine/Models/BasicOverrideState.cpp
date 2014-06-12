#include "BasicOverrideState.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv { namespace model {

// ****************************
//
BasicOverrideState::BasicOverrideState  ( ITimeEvaluatorPtr timeEvaluator )
    : m_param( nullptr )
    , m_value( nullptr )
    , m_curVal( nullptr )
    , m_enabled( false )
    , m_overriden( false )
    , m_changed( false )
{
    m_param = new ParamFloat( "overrideAlpha", FloatInterpolator(), timeEvaluator );
    m_value = ValuesFactory::CreateValueFloat("overrideAlpha" );

    Disable();
}

// ****************************
//
BasicOverrideState::~BasicOverrideState ()
{
    delete m_param;
    delete m_value;
}

// ****************************
//
void                BasicOverrideState::Update              ( TimeType t )
{
    //Update alpha
    if( IsEnabled() )
    {
        if( IsOverriden() )
        {
            m_value->SetValue( m_curVal->GetValue() );
        }
        else
        {
            m_value->SetValue( m_param->Evaluate() );
        }
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
bool                BasicOverrideState::IsOverriden         () const
{
    return IsAlphaOverriden();
}

// ****************************
//
bool                BasicOverrideState::IsEnabled           () const
{
    return IsAlphaEnabled();
}

// ****************************
//
void                BasicOverrideState::Disable             ()
{
    DisableAlpha();
}

// ****************************
//
void                BasicOverrideState::Enable              ()
{
    EnableAlpha();
}

// ****************************
//
bool                BasicOverrideState::IsAlphaEnabled      () const
{
    return m_enabled;
}

// ****************************
//
bool                BasicOverrideState::IsAlphaOverriden    () const
{
    return m_overriden;
}

// ****************************
//
void                BasicOverrideState::DisableAlpha        ()
{
    if( m_curVal != nullptr || m_enabled || m_overriden )
    {
        SetChanged( true );
    }

    SetCurAlphaVal( nullptr );

    m_value->SetValue( 1.0f );

    m_enabled = false;
    m_overriden = false;
}

// ****************************
//
void                BasicOverrideState::EnableAlpha         ()
{
    if( m_curVal != m_value || !m_enabled || !m_overriden )
    {
        SetChanged( true );
    }

    SetCurAlphaVal( m_value );
}

// ****************************
//
IParameter *        BasicOverrideState::GetAlphaParam       ()
{
    return m_param;
}

// ****************************
//
const IValue *      BasicOverrideState::GetAlphaValue       () const
{
    return m_value;
}

// ****************************
//
void                BasicOverrideState::SetCurAlphaVal      ( const IValue * val )
{    
    if( val == nullptr )
    {
        m_curVal = nullptr;
    }
    else
    {
        auto tv = QueryTypedValue< ValueFloat >( val );
        assert( tv != nullptr );

        if( tv != m_value )
        {
            m_overriden = true;
        }
        else
        {
            m_overriden = false;
        }

        m_enabled = true;

        m_curVal = tv;
    }
}

// ****************************
//
const IValue *      BasicOverrideState::GetCurAlphaVal      () const
{
    return m_curVal;
}

} //model
} //bv
