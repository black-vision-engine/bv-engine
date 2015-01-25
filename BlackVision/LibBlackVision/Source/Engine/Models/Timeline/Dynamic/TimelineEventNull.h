#pragma once

#include "Engine/Models/Timeline/Dynamic/TimelineEventBaseMixin.h"


namespace bv { namespace model {

class TimelineEventNull : public TimelineEventBaseMixin< ITimelineEventNull >
{
private:

    typedef TimelineEventBaseMixin< ITimelineEventNull > Parent;

public:

    TimelineEventNull   ( const std::string & name, TimeType eventTime, const ITimeline * owner =  nullptr );
    ~TimelineEventNull  ();

};

} //model
} //bv
