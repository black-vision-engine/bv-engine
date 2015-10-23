#pragma once

#include <list>

#include "Engine/Models/Interfaces/ITimeline.h"

#include "Engine/Models/Timeline/TimeEvaluatorBase.h"
#include "Engine/Models/Timeline/TimeSegmentEvalImpl.h"


namespace bv { namespace model {

class DefaultTimeline : public TimeEvaluatorBase< ITimeline >
{
private:

    typedef TimeEvaluatorBase< ITimeline > Parent;

private:

    TimeSegmentEvalImpl             m_timeEvalImpl;
    TimeType                        m_prevTime;

    ITimelineEvent *                m_activeEvent;

    std::vector< ITimelineEvent * > m_keyFrameEvents;

public:

                                                DefaultTimeline     ( const std::string & name, TimeType duration, TimelineWrapMethod preMethod, TimelineWrapMethod postMethod );
                                                ~DefaultTimeline    ();

    virtual void                                Serialize           ( SerializeObject & sob ) const;
    static ISerializablePtr                     Create              ( DeserializeObject & dob );

    //ITimeline
    virtual TimeType                            GetDuration         () const override;

    //ITimeEvaluator
    virtual void                                SetGlobalTimeImpl   ( TimeType t ) override;
    virtual TimeType                            GetLocalTime        () const override;

    virtual void                                SetLocalTime        ( TimeType t ) override;

    virtual void                                Restart             () override;

    virtual void                                SetPlayDirection    ( TimelinePlayDirection direction ) override;
    virtual TimelinePlayDirection               GetPlayDirection    () const override;
    virtual void                                Reverse             () override;

    virtual void                                Play                () override;
    virtual void                                Stop                () override;

    virtual void                                SetTimeAndStop      ( TimeType t ) override; 
    virtual void                                SetTimeAndPlay      ( TimeType t ) override; 

    virtual void                                SetWrapBehavior     ( TimelineWrapMethod preMethod, TimelineWrapMethod postMethod ) override;
    virtual TimelineWrapMethod                  GetWrapBehaviorPre  () const override;
    virtual TimelineWrapMethod                  GetWrapBehaviorPost () const override;

    virtual SizeType                            NumKeyFrames        () const override;

    virtual bool                                AddKeyFrame         ( ITimelineEvent * evt ) override;

    virtual const ITimelineEvent *              GetKeyFrameEvent    ( const std::string & name ) const override;
    virtual const ITimelineEvent *              GetKeyFrameEvent    ( unsigned int idx ) const override;

    virtual bool                                RemoveKeyFrameEvent ( unsigned int idx ) override;
    virtual bool                                RemoveKeyFrameEvent ( const std::string & name ) override;

private:

    void                                        DeactivateEvent     ( ITimelineEvent * evt );
    ITimelineEvent *                            CurrentEvent        ( TimeType curTime, TimeType prevTime ) const;

    bool                                        CanBeInserted       ( const ITimelineEvent * evt ) const;

    void                                        TriggerEventStep    ( TimeType curTime, TimeType prevTime );
    void                                        PostUpdateEventStep ();

};

DEFINE_PTR_TYPE(DefaultTimeline)

} //model
} //bv
