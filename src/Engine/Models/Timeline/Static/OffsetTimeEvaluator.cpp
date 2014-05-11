#include "OffsetTimeEvaluator.h"

namespace bv { namespace model {

// *******************************
//
OffsetTimeEvaluator::OffsetTimeEvaluator                ( const std::string & name, TimeType offsetTime )
    : m_name( name )
    , m_globalTime( 0.f )
    , m_timeOffset( offsetTime )
{
}

// *******************************
//
OffsetTimeEvaluator::~OffsetTimeEvaluator               ()
{
}

// *******************************
//
void                OffsetTimeEvaluator::SetTimeOffset  ( TimeType t )
{
    m_timeOffset = t;
}

// *******************************
//
const std::string & OffsetTimeEvaluator::GetName        () const
{
    return m_name;
}

// *******************************
//
void                OffsetTimeEvaluator::SetGlobalTime  ( TimeType t )
{
    m_globalTime = t;
}

// *******************************
//
TimeType            OffsetTimeEvaluator::GetLocalTime   () const
{
    return m_globalTime + m_timeOffset;
}

} //model
} //bv
