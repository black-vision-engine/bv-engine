#include "OffsetTimeEvaluator.h"

namespace bv { namespace model {

// *******************************
//
OffsetTimeEvaluator::OffsetTimeEvaluator                    ( const std::string & name, TimeType offsetTime )
    : Parent( name )
    , m_globalTime( 0.f )
    , m_timeOffset( offsetTime )
{
}

// *******************************
//
OffsetTimeEvaluator::~OffsetTimeEvaluator                   ()
{
}

// *******************************
//
void                OffsetTimeEvaluator::Serialize           ( SerializeObject & sob ) const
{
    sob.SetName( "timeline" );
    sob.SetValue( "name", GetName() );
    sob.Pop();
}

// *******************************
//
ISerializablePtr     OffsetTimeEvaluator::Create              ( DeserializeObject & dob )
{
    dob; return nullptr;
}

// *******************************
//
void                OffsetTimeEvaluator::SetTimeOffset      ( TimeType t )
{
    m_timeOffset = t;
}

// *******************************
//
void                OffsetTimeEvaluator::SetGlobalTimeImpl  ( TimeType t )
{
    m_globalTime = t;
}

// *******************************
//
TimeType            OffsetTimeEvaluator::GetLocalTime       () const
{
    return m_globalTime + m_timeOffset;
}

} //model
} //bv
