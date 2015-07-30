#include "ConstTimeEvaluator.h"

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
void                ConstTimeEvaluator::Serialize           ( SerializeObject & sob ) const
{
    sob.SetName( "timeline" );
    sob.SetValue( "name", GetName() );
    sob.SetValue( "type", "const" );
    sob.Pop();
}

// *******************************
//
ISerializablePtr     ConstTimeEvaluator::Create              ( DeserializeObject & dob )
{
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

} //model
} //bv
