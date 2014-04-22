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

protected:

    TimelineEventBaseMixin( const std::string & name, TimelineEventType type, TimeType eventTime, const ITimeline * owner );

public:

    virtual std::string         GetName         () const override;
    virtual TimelineEventType   GetType         () const override;
    
    virtual TimeType            GetEventTime    () const override;
    
    virtual const ITimeline *   GetOwnerTimeline() const override;

    void                        SetOwnerTimeline( const ITimeline * owner );
    void                        SetEventTime    ( TimeType eventTime );

    virtual ~TimelineEventBaseMixin() {}

};

} //model
} //bv

#include "TimelineEventBaseMixin.inl"
