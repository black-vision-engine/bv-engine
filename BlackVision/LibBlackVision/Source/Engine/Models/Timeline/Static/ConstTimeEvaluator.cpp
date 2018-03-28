#include "stdafx.h"

#include "ConstTimeEvaluator.h"
#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"



#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {

// *******************************
//
ConstTimeEvaluator::ConstTimeEvaluator                      ( const std::string & name, TimeType val )
    : Parent( name )
    , m_timeVal( val )
{
}

// *******************************
//
ConstTimeEvaluator::~ConstTimeEvaluator                     ()
{
}

// *******************************
//
void                ConstTimeEvaluator::Serialize           ( ISerializer & ser ) const
{
    ser.EnterChild( "timeline" );
    ser.SetAttribute( "name", GetName() );
    ser.SetAttribute( "type", "const" );
    ser.SetAttribute( "value", Convert::T2String< TimeType >( m_timeVal ) );
//    SerializeChildren( ser );
    ser.ExitChild();
}

// *******************************
//
ConstTimeEvaluatorPtr    ConstTimeEvaluator::Create         ( const std::string & name, TimeType val )
{
    return ConstTimeEvaluatorPtr( new ConstTimeEvaluator( name, val ) );
}

// *******************************
//
ConstTimeEvaluatorPtr     ConstTimeEvaluator::Create         ( const IDeserializer & deser )
{
    auto name = deser.GetAttribute( "name" );
    auto value = Convert::String2T< TimeType >( deser.GetAttribute( "value" ) );

    auto te = Create( name, value );
    te->DeserializeChildren( deser );

    return te;
}

// *******************************
//
void                ConstTimeEvaluator::SetConstTimeValue   ( TimeType t )
{
    m_timeVal = t;
}

// *******************************
//
void                ConstTimeEvaluator::SetGlobalTimeImpl   ( TimeType t )
{
    { t; } // FIXME: suppress unused warning
}

// *******************************
//
TimeType            ConstTimeEvaluator::GetLocalTime        () const
{
    return m_timeVal;
}

// ***********************
//
const std::string&        ConstTimeEvaluator::GetType             () const
{
    return Type();
}

// ***********************
//
namespace {
const std::string ConstTimeEvaluatorType = "ConstTimeEvaluator";
} //annonymous

// ***********************
//
const std::string&        ConstTimeEvaluator::Type                ()
{
    return ConstTimeEvaluatorType;
}

} //model
} //bv
