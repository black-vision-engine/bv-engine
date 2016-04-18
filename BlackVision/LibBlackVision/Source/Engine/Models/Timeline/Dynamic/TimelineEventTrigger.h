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

    std::vector< IEventPtr >        m_eventsToTrigger;

public:
    TimelineEventTrigger    ( const std::string & name, TimeType eventTime, const ITimeline * owner, std::vector< IEventPtr >&& eventsToTrigger );
    ~TimelineEventTrigger   ();


    virtual void                    Serialize       ( ISerializer & ser ) const;
    static TimelineEventTriggerPtr  Create          ( const IDeserializer & deser, const ITimeline * timeline );

    void                            SendEvents      ();
};


}   // model
}	// bv