#pragma once

#include "Engine/Types/Enums.h"

#include "Engine/Models/Interfaces/ITimeEvaluator.h"


namespace bv { namespace model {

class ITimelineEvent;
class IParameter;

class ITimeline : public ITimeEvaluator
{
public:

    virtual TimeType                GetDuration         () const = 0;

    virtual void                    Restart             () = 0;

    virtual void                    SetPlayDirection    ( TimelinePlayDirection direction ) = 0;
    virtual TimelinePlayDirection   GetPlayDirection    () const = 0;
    virtual void                    Reverse             () = 0;

    virtual void                    Play                () = 0;
    virtual void                    Stop                () = 0;

    virtual void                    SetTimeAndStop      ( TimeType t ) = 0; 
    virtual void                    SetTimeAndPlay      ( TimeType t ) = 0; 

    virtual void                    SetWrapBehavior     ( TimelineWrapMethod preMethod, TimelineWrapMethod postMethod ) = 0;
    virtual TimelineWrapMethod      GetWrapBehaviorPre  () const = 0;
    virtual TimelineWrapMethod      GetWrapBehaviorPost () const = 0;

    virtual void                    SetLocalTime        ( TimeType t ) = 0;

    virtual unsigned int            NumKeyFrames        () const = 0;

    virtual bool                    AddKeyFrame         ( ITimelineEvent * evt ) = 0;

    virtual const ITimelineEvent *  GetKeyFrameEvent    ( const std::string & name ) const = 0;
    virtual const ITimelineEvent *  GetKeyFrameEvent    ( unsigned int idx ) const = 0;

    virtual bool                    RemoveKeyFrameEvent ( unsigned int idx ) = 0;
    virtual bool                    RemoveKeyFrameEvent ( const std::string & name ) = 0;

    virtual const ITimelineEvent *  CurrentEvent        () const = 0;
    virtual const ITimelineEvent *  LastTriggeredEvent  () const = 0;

    virtual                         ~ITimeline      () {}

};

DEFINE_PTR_TYPE(ITimeline)

} //model
} //bv
