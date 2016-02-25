#pragma once

#include "Engine/Models/Timeline/Dynamic/TimelineEventBaseMixin.h"


namespace bv { namespace model {

class TimelineEventNull;
DEFINE_PTR_TYPE(TimelineEventNull)

class TimelineEventNull : public TimelineEventBaseMixin< ITimelineEventNull >
{
private:

    typedef TimelineEventBaseMixin< ITimelineEventNull > Parent;

                                    TimelineEventNull   ( const std::string & name, TimeType eventTime, const ITimeline * owner );

public:

    static TimelineEventNullPtr     Create              ( const std::string & name, TimeType eventTime, const ITimeline * owner =  nullptr );
                                    ~TimelineEventNull  ();

    static TimelineEventNullPtr     Create              ( const IDeserializer & deser, const ITimeline * timeline );
    virtual void                    Serialize           ( ISerializer& ser ) const;
};

} //model
} //bv
