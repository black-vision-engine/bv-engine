#include "TimelineEventNull.h"


namespace bv { namespace model {

// *********************************
//
TimelineEventNull::TimelineEventNull   ( const std::string & name, TimeType eventTime, const ITimeline * owner )
    : Parent( name, TimelineEventType::TET_NULL, eventTime, owner )
{
}

// *********************************
//
TimelineEventNull::~TimelineEventNull  ()
{
}

} //model
} //bv
