#pragma once

#include "Engine/Models/Timeline/Dynamic/TimelineEventBaseMixin.h"
#include "Engine/Models/Interfaces/ITimeline.h"
#include "Engine/Events/Interfaces/IEvent.h"

#include <vector>


namespace bv { namespace model
{


class TimelineEventTrigger;
DEFINE_PTR_TYPE(TimelineEventTrigger)


class TimelineEventTrigger : public TimelineEventBaseMixin< ITimelineEvent >
{
    typedef TimelineEventBaseMixin< ITimelineEvent > Parent;
private:

    std::string             m_eventsToTrigger;

public:
    TimelineEventTrigger    ( const std::string & name, TimeType eventTime, const ITimeline * owner, const std::string & eventsToTrigger );
    ~TimelineEventTrigger   ();


    virtual void                    Serialize       ( ISerializer & ser ) const;
    static TimelineEventTriggerPtr  Create          ( const IDeserializer & deser, const ITimeline * timeline );
    static TimelineEventTriggerPtr  Create          ( const std::string & name, TimeType eventTime, const std::string & eventsToTrigger, const ITimeline * owner =  nullptr );

    void                            SendEvents      ();
};


}   // model
}	// bv