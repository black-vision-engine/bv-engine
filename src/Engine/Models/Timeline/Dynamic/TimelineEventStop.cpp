#include "TimelineEventStop.h"


namespace bv { namespace model {

// *********************************
//
TimelineEventStop::TimelineEventStop   ( const std::string & name, TimeType eventTime, const ITimeline * owner )
    : Parent( name, TimelineEventType::TET_STOP, eventTime, owner )
{
}

// *********************************
//
TimelineEventStop::~TimelineEventStop  ()
{
}

} //model
} //bv
