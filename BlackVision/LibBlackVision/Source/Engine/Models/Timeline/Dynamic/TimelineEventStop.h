#pragma once

#include "Engine/Models/Timeline/Dynamic/TimelineEventBaseMixin.h"


namespace bv { namespace model {

class TimelineEventStop;
DEFINE_PTR_TYPE(TimelineEventStop)

class TimelineEventStop : public TimelineEventBaseMixin< ITimelineEventStop >
{
private:

    typedef TimelineEventBaseMixin< ITimelineEventStop > Parent;

    TimelineEventStop   ( const std::string & name, TimeType eventTime, const ITimeline * owner );

public:
    virtual void                Serialize       ( ISerializer& ser ) const;
    
    static TimelineEventStopPtr Create          ( const std::string & name, TimeType eventTime, const ITimeline * owner =  nullptr );

    static TimelineEventStopPtr Create          ( const IDeserializer & deser, const ITimeline * timeline );

    ~TimelineEventStop  ();

};

} //model
} //bv
