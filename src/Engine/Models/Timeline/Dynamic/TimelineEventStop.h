#pragma once

#include "Engine/Models/Timeline/Dynamic/TimelineEventBaseMixin.h"


namespace bv { namespace model {

class TimelineEventStop : public TimelineEventBaseMixin< ITimelineEventStop >
{
private:

    typedef TimelineEventBaseMixin< ITimelineEventStop > Parent;

public:

    TimelineEventStop   ( const std::string & name, TimeType eventTime, const ITimeline * owner =  nullptr );
    ~TimelineEventStop  ();

};

} //model
} //bv
