#include "stdafx.h"

#include "ConstTimeEvaluator.h"
#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"

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
void                ConstTimeEvaluator::Serialize           ( ISerializer& sob ) const
{
    sob.EnterChild( "timeline" );
    sob.SetAttribute( "name", GetName() );
    sob.SetAttribute( "type", "const" );
    sob.ExitChild();
}

// *******************************
//
ConstTimeEvaluator *     ConstTimeEvaluator::Create              ( const IDeserializer& dob )
{
    assert( false );
    dob; return nullptr;
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
const std::string&        ConstTimeEvaluator::GetType             ()
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
