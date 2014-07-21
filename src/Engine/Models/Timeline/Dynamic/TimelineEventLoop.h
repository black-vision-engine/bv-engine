#pragma once

#include "Engine/Models/Timeline/Dynamic/TimelineEventBaseMixin.h"


namespace bv { namespace model {

class TimelineEventLoop : public TimelineEventBaseMixin< ITimelineEventLoop >
{
private:

    typedef TimelineEventBaseMixin< ITimelineEventLoop > Parent;

private:

    unsigned int    m_curLoopCount;
    unsigned int    m_totalLoopCount;
    
    LoopEventAction m_action;
    TimeType        m_targetTime;

public:

                            TimelineEventLoop   ( const std::string & name, TimeType eventTime, LoopEventAction action, unsigned int totalLoopCount, TimeType targetTime = 0.0f, const ITimeline * owner =  nullptr );
                            ~TimelineEventLoop  ( );

    virtual unsigned int    GetLoopCount        () const override;
    virtual unsigned int    GetTotalLoopCount   () const override;

    virtual LoopEventAction GetActionType       () const override;
    virtual TimeType        GetTargetTime       () const override;

    virtual bool            IsActive            () const override;

    void                    SetTargetTime       ( TimeType t );
    void                    SetAction           ( LoopEventAction action );
    void                    SetLoopCount        ( unsigned int loopCount );
    void                    IncLoopCount        ();
    void                    ResetLoopCount      ();
    void                    SetTotalLoopCount   ( unsigned int totalLoopCount );

};

} //model
} //bv
