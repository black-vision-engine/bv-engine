#pragma once

#include "Engine/Models/Timeline/Dynamic/TimelineEventBaseMixin.h"
#include "Engine/Models/Interfaces/ITimeline.h"

namespace bv { namespace model {

class TimelineEventLoop;
DEFINE_PTR_TYPE(TimelineEventLoop)

class TimelineEventLoop : public TimelineEventBaseMixin< ITimelineEventLoop >
{
private:

    typedef TimelineEventBaseMixin< ITimelineEventLoop > Parent;

private:

    unsigned int    m_curLoopCount;
    unsigned int    m_totalLoopCount;
    
    LoopEventAction m_action;
    TimeType        m_targetTime;

                                TimelineEventLoop   ( const std::string & name, TimeType eventTime, LoopEventAction action, unsigned int totalLoopCount, TimeType targetTime, const ITimeline * owner );

public:

    static TimelineEventLoopPtr Create              ( const std::string & name, TimeType eventTime, LoopEventAction action, unsigned int totalLoopCount, TimeType targetTime = 0.0f, const ITimeline * owner =  nullptr );
                                ~TimelineEventLoop  ( );

    virtual void                Serialize       ( ISerializer & ser ) const;
    static TimelineEventLoopPtr Create          ( const IDeserializer & deser, const ITimeline * timeline );

    virtual unsigned int        GetLoopCount        () const override;
    virtual unsigned int        GetTotalLoopCount   () const override;

    virtual LoopEventAction     GetActionType       () const override;
    virtual TimeType            GetTargetTime       () const override;

    virtual bool                IsActive            () const override;

    void                        SetTargetTime       ( TimeType t );
    void                        SetAction           ( LoopEventAction action );
    void                        SetLoopCount        ( unsigned int loopCount );
    void                        IncLoopCount        ();
    void                        ResetLoopCount      ();
    void                        SetTotalLoopCount   ( unsigned int totalLoopCount );

};

} //model
} //bv
