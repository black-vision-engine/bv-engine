#include "BasicOverrideState.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv { namespace model {

// ****************************
//
BasicOverrideState::BasicOverrideState  ( ITimeEvaluatorPtr timeEvaluator )
    : m_param( nullptr )
    , m_value( nullptr )
    , m_curVal( nullptr )
{
    m_param = new ParamFloat( "overrideAlpha", FloatInterpolator(), timeEvaluator );
    m_value = ValuesFactory::CreateValueFloat("overrideAlpha" );
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
    return m_curVal != nullptr;
}

// ****************************
//
void                BasicOverrideState::DisableAlpha        ()
{
    SetCurAlphaVal( nullptr );
}

// ****************************
//
void                BasicOverrideState::EnableAlpha         ()
{
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
IValue *            BasicOverrideState::GetAlphaValue       ()
{
    return m_value;
}

// ****************************
//
void                BasicOverrideState::SetCurAlphaVal      ( const IValue * val )
{
    auto tv = QueryTypedValue< ValueFloat >( val );
    assert( tv != nullptr );

    m_curVal = tv;
}

// ****************************
//
const IValue *      BasicOverrideState::GetCurAlphaVal      () const
{
    return m_curVal;
}

} //model
} //bv
