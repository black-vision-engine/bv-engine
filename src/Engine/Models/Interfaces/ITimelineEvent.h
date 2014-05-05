#pragma once

#include <string>

#include "System/BasicTypes.h" //FIXME: store in PCH
#include "Engine/Types/Enums.h"


namespace bv { namespace model {

class ITimeline;

// **********************************
class ITimelineEvent
{
public:

    virtual std::string         GetName             () const = 0;
    virtual TimelineEventType   GetType             () const = 0;

    virtual TimeType            GetEventTime        () const = 0;
    virtual TimeType            GetLastTriggerTime  () const = 0;

    virtual const ITimeline *   GetOwnerTimeline    () const = 0;

    virtual ~ITimelineEvent() {}

};

// **********************************
class ITimelineEventStop : public ITimelineEvent
{
public:

    virtual ~ITimelineEventStop() {}
};

// **********************************
class ITimelineEventLoop : public ITimelineEvent
{
public:

    virtual unsigned int    GetLoopCount        () const = 0;
    virtual unsigned int    GetTotalLoopCount   () const = 0;

    virtual LoopEventAction GetActionType       () const = 0;
    virtual TimeType        GetTargetTime       () const = 0;

    virtual ~ITimelineEventLoop() {}
};

class ITimelineEventNull : public ITimelineEvent
{
public:

    virtual ~ITimelineEventNull() {}
};

} //model
} //bv
