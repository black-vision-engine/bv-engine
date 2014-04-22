#pragma once

#include "Engine/Models/Interfaces/ITimeEvaluator.h"


namespace bv { namespace model {

class ITimelineEvent;
class IParameter;

class ITimeline : public ITimeEvaluator
{
public:

    virtual void                    Play                () = 0;
    virtual void                    Stop                () = 0;

    virtual void                    SetTimeAndStop      ( TimeType t ) = 0; 
    virtual void                    SetTimeAndPlay      ( TimeType t ) = 0; 

    virtual void                    SetLocalTime        ( TimeType t ) = 0;
    virtual TimeType                GetLocalTime        () const = 0;

    virtual void                    Restart             () = 0;

    virtual void                    SetDirectionFwd     () = 0;
    virtual void                    SetDirectionBack    () = 0;

    virtual void                    AddKeyFrame         ( const ITimelineEvent * evt ) = 0;
    virtual unsigned int            NumKeyFrames        () const = 0;
    virtual const ITimelineEvent *  GetKeyFrameEvent    ( const std::string & name ) const = 0;
    virtual const ITimelineEvent *  GetKeyFrameEvent    ( unsigned int idx ) const = 0;
    virtual bool                    RemoveKeyFrameEvent ( unsigned int idx ) = 0;
    virtual bool                    RemoveKeyFrameEvent ( const std::string & name ) = 0;

    virtual void                    AddParameter        ( const IParameter * param ) = 0;
    virtual bool                    RemoveParameter     ( const IParameter * param ) = 0;
    virtual bool                    RemoveParameter     ( const std::string & name ) = 0;

    virtual                         ~ITimeline      () {}

};

} //model
} //bv
