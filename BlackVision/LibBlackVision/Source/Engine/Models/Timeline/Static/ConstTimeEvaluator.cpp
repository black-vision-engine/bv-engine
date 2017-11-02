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
void                ConstTimeEvaluator::Serialize           ( ISerializer & sob ) const
{
    sob.EnterChild( "timeline" );
    sob.SetAttribute( "name", GetName() );
    sob.SetAttribute( "type", "const" );
    sob.SetAttribute( "value", SerializationHelper::T2String< TimeType >( m_timeVal ) );
    sob.ExitChild();
}

// *******************************
//
ConstTimeEvaluatorPtr    ConstTimeEvaluator::Create         ( const std::string & name, TimeType val )
{
    return ConstTimeEvaluatorPtr( new ConstTimeEvaluator( name, val ) );
}

// *******************************
//
ConstTimeEvaluatorPtr     ConstTimeEvaluator::Create         ( const IDeserializer & dob )
{
    auto name = dob.GetAttribute( "name" );
    auto value = SerializationHelper::String2T< TimeType >( dob.GetAttribute( "value" ) );

    return Create( name, value );
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
