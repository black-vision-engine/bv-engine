#include <list>

#include "Engine/Models/Interfaces/ITimeline.h"

#include "Engine/Models/Timeline/TimeSegmentEvalImpl.h"


namespace bv { namespace model {

class DefaultTimeline : public ITimeline
{
private:

    std::string                             m_name;
    TimeSegmentEvalImpl                     m_timeEvalImpl;

    std::vector< const IParameter * >       m_registeredParameters;
    std::vector< const ITimelineEvent * >   m_keyFrameEvents;

    static const TimeType                   ms_evtTimeSeparation;

public:

                                                DefaultTimeline     ( const std::string & name, TimeType duration, TimelineWrapMethod preMethod, TimelineWrapMethod postMethod, ITimeEvaluator * parent );
                                                ~DefaultTimeline    ();

    //ITimeEvaluator
    virtual const std::string &                 GetName             () const override;
    virtual TimeType                            Evaluate            ( TimeType t ) const override;

    //IParamSet
    virtual std::vector< IParameter * > &       GetParameters       () override;
    virtual IParameter *                        GetParameter        ( const std::string & name ) override;

    //IUpdatable
    virtual void                                Update              ( TimeType t ) override;

    //ITimeline
    virtual TimeType                            GetDuration         () const = 0;

    virtual void                                Restart             () override;

    virtual void                                SetPlayDirection    ( TimelinePlayDirection direction ) override;
    virtual TimelinePlayDirection               GetPlayDirection    () const override;

    virtual void                                Play                () override;
    virtual void                                Stop                () override;

    virtual void                                SetTimeAndStop      ( TimeType t ) override; 
    virtual void                                SetTimeAndPlay      ( TimeType t ) override; 

    virtual void                                SetWrapBehavior     ( TimelineWrapMethod preMethod, TimelineWrapMethod postMethod ) override;
    virtual TimelineWrapMethod                  GetWrapBehaviorPre  () const override;
    virtual TimelineWrapMethod                  GetWrapBehaviorPost () const override;

    virtual void                                SetLocalTime        ( TimeType t ) override;
    virtual TimeType                            GetLocalTime        () const override;

    virtual unsigned int                        NumKeyFrames        () const override;

    virtual bool                                AddKeyFrame         ( const ITimelineEvent * evt ) override;

    virtual const ITimelineEvent *              GetKeyFrameEvent    ( const std::string & name ) const override;
    virtual const ITimelineEvent *              GetKeyFrameEvent    ( unsigned int idx ) const override;

    virtual bool                                RemoveKeyFrameEvent ( unsigned int idx ) override;
    virtual bool                                RemoveKeyFrameEvent ( const std::string & name ) override;

    virtual const ITimelineEvent *              CurrentEvent        () const override;

    virtual bool                                AddParameter        ( const IParameter * param ) override;

    virtual bool                                RemoveParameter     ( const IParameter * param ) override;
    virtual unsigned int                        RemoveParameters    ( const std::string & name ) override;

private:

    bool                                        CanBeInserted       ( const ITimelineEvent * evt ) const;

};

} //model
} //bv
