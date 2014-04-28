#include "Engine/Models/Interfaces/ITimeline.h"

#include "Engine/Models/Timeline/TimeSegmentEvalImpl.h"


namespace bv { namespace model {

class DefaultTimeline : public ITimeline
{
private:

    std::string                     m_name;
    TimeSegmentEvalImpl             m_timeEvalImpl;

    std::vector< IParameter * >     m_registeredParameters;

public:

                                                DefaultTimeline     ( const std::string & name, TimeType duration, ITimeEvaluator * parent );
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

    virtual void                                Play                () override;
    virtual void                                Stop                () override;

    virtual void                                SetTimeAndStop      ( TimeType t ) override; 
    virtual void                                SetTimeAndPlay      ( TimeType t ) override; 

    virtual void                                SetLocalTime        ( TimeType t ) override;
    virtual TimeType                            GetLocalTime        () const override;

    virtual unsigned int                        NumKeyFrames        () const override;

    virtual void                                AddKeyFrame         ( const ITimelineEvent * evt ) override;

    virtual const ITimelineEvent *              GetKeyFrameEvent    ( const std::string & name ) const override;
    virtual const ITimelineEvent *              GetKeyFrameEvent    ( unsigned int idx ) const override;

    virtual bool                                RemoveKeyFrameEvent ( unsigned int idx ) override;
    virtual bool                                RemoveKeyFrameEvent ( const std::string & name ) override;

    virtual const ITimelineEvent *              CurrentEvent        () const override;

    virtual void                                AddParameter        ( const IParameter * param ) override;

    virtual bool                                RemoveParameter     ( const IParameter * param ) override;
    virtual bool                                RemoveParameter     ( const std::string & name ) override;

};

} //model
} //bv
