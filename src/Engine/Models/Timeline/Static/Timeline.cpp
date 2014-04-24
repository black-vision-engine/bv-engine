#include "Timeline.h"

#include <cassert>


namespace bv { namespace model {

// *********************************
//
Timeline::Timeline    ( ITimeEvaluator * parent, TimeType startTime, TimeType endTime, const std::string & name )
    : TimelineBase( startTime, endTime, name )
    , m_parent( parent )
{
    //FIXME: skip it for now - production version should have at least GlobalTimeline added here
//    assert( parent );
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
    if ( m_parent )
        t = m_parent->Evaluate( t );

    return TimelineBase::Evaluate( t );
}

} //model
} //bv
