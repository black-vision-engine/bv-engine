#pragma once

#include "Engine/Models/Interfaces/ITimelineEvent.h"


namespace bv { namespace model {

template< typename ITimelineEventIface >
class TimelineEventBaseMixin : public ITimelineEventIface
{
private:

    std::string         m_name;

    TimelineEventType   m_type;

    const ITimeline *   m_owner;

    TimeType            m_eventTime;
    TimeType            m_lastTriggerTime;

protected:

    TimelineEventBaseMixin( const std::string & name, TimelineEventType type, TimeType eventTime, const ITimeline * owner );

public:

    virtual std::string         GetName             () const override;
    virtual TimelineEventType   GetType             () const override;
    
    virtual TimeType            GetEventTime        () const override;
    virtual TimeType            GetLastTriggerTime  () const override;
    
    virtual const ITimeline *   GetOwnerTimeline    () const override;

    void                        SetOwnerTimeline    ( const ITimeline * owner );
    void                        SetEventTime        ( TimeType eventTime );
    void                        SetLastTriggerTime  ( TimeType triggerTime );

    virtual ~TimelineEventBaseMixin() {}

};

} //model
} //bv

#include "TimelineEventBaseMixin.inl"
