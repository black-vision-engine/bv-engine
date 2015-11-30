#pragma once

#include "Engine/Models/Timeline/Dynamic/TimelineEventBaseMixin.h"


namespace bv { namespace model {

class TimelineEventStop : public TimelineEventBaseMixin< ITimelineEventStop >
{
private:

    typedef TimelineEventBaseMixin< ITimelineEventStop > Parent;

public:
    virtual void                Serialize       ( ISerializer& ser ) const;
    static TimelineEventStop*   Create          ( const IDeserializer& deser, ITimeline* timeline );

    TimelineEventStop   ( const std::string & name, TimeType eventTime, const ITimeline * owner =  nullptr );
    ~TimelineEventStop  ();

};

} //model
} //bv
