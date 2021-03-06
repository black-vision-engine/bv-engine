#pragma once

#include <string>

#include "CoreDEF.h" //FIXME: store in PCH
#include "Engine/Types/Enums.h"
#include "Serialization/ISerializable.h"


namespace bv { namespace model {

class ITimeline;
DEFINE_CONST_PTR_TYPE(ITimeline)

class ITimelineEvent;
DEFINE_PTR_TYPE(ITimelineEvent)
DEFINE_CONST_PTR_TYPE(ITimelineEvent)

// **********************************
class ITimelineEvent : public ISerializable
{
public:

    virtual std::string         GetName             () const = 0;
    virtual TimelineEventType   GetType             () const = 0;

    virtual bool                IsActive            () const = 0;
    virtual void                SetActive           ( bool active ) = 0;

    virtual TimeType            GetEventTime        () const = 0;

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
