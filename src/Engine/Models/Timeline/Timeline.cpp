#include "Timeline.h"

#include <cassert>


namespace bv { namespace model {

// *********************************
//
Timeline::Timeline    ( ITimeEvaluator * parent, TimeType startTime, TimeType endTime )
    : TimelineBase( startTime, endTime )
    , m_parent( parent )
{
    assert( parent );
}

// *********************************
//
Timeline::~Timeline   ()
{
}

// *********************************
//
TimeType    Timeline::Evaluate    ( TimeType t )  const
{
    t = m_parent->Evaluate( t );

    return TimelineBase::Evaluate( t );
}

} //model
} //bv
