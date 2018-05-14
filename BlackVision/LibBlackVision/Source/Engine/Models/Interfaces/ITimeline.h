#pragma once

#include "Engine/Types/Enums.h"

#include "Engine/Models/Interfaces/ITimeEvaluator.h"


/**@page Timelines Timelines

A timeline's purpose is to handle time change. The most important Timeline is @ref DefaultTimeline, which also has keyframe events, that get triggered in the right moment.

@section KeyframeEvents Keyframe Events

We have a following list of keyframe events:
- stop events
- loop events
- null events (which just send notification to the editor)
- trigger events (which may trigger any action)

@subsection LoopEvents Loop events

Loop events may be of the following type:
- jump events (jump to the specified time)
- restart events (jump to time 0)
- reverse events (change the direction of playing)

@see bv::model::ITimeline, bv::model::DefaultTimeline
*/

namespace bv { namespace model {

class ITimelineEvent;
DEFINE_PTR_TYPE(ITimelineEvent)
class IParameter;


/**@brief Timeline interface
@ingroup Model
*/
class ITimeline : public ITimeEvaluator
{
public:

    virtual void	                SetDuration         ( TimeType duration ) = 0;
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

    virtual SizeType                NumKeyFrames        () const = 0;

    virtual bool                    AddKeyFrame         ( const ITimelineEventPtr & evt ) = 0;

    virtual const ITimelineEvent *  GetKeyFrameEvent    ( const std::string & name ) const = 0;
    virtual const ITimelineEvent *  GetKeyFrameEvent    ( unsigned int idx ) const = 0;

    virtual bool                    RemoveKeyFrameEvent ( unsigned int idx ) = 0;
    virtual bool                    RemoveKeyFrameEvent ( const std::string & name ) = 0;

    //virtual const ITimelineEvent *  CurrentEvent        () const = 0;
    //virtual const ITimelineEvent *  LastTriggeredEvent  () const = 0;

    virtual                         ~ITimeline      () {}

    virtual const std::string&      GetType         () const = 0;
};

DEFINE_PTR_TYPE(ITimeline)
DEFINE_CONST_PTR_TYPE(ITimeline)

} //model
} //bv
