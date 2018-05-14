#pragma once


#include "Engine/Events/BaseEvent.h"
#include "Serialization/ConversionHelper.h"




namespace bv
{



/**@brief Timeline keyframes events.

@section TimeLineEvent_Commands Commands

This event supports following commands:
- @ref TimelineKeyframeEvent_Commands_AddKeyframe "AddKeyframe"
- @ref TimelineKeyframeEvent_Commands_RemoveKeyframe "RemoveKeyframe"

@subsection TimelineKeyframeEvent_Commands_AddKeyframe Adding keyframes to Timelines

@subsection TimelineKeyframeEvent_Commands_RemoveKeyframe Removing keyframes from Timelines

*/
class TimelineKeyframeEvent : public RemoteEvent
{
public:

    typedef enum
    {
        AddKeyframe,
        RemoveKeyframe,
        Fail            ///< Wrong command
    } Command;

    typedef enum
    {
        StopKeyframe,
        LoopReverseKeyframe,
        LoopJumpKeyframe,
        LoopRestartKeyframe,
        TriggerEventKeyframe,
        NullKeyframe,
        KeyframeTypeFail
    } KeyframeType;

private:

    static const EventType      m_sEventType;
    static std::string          m_sEventName;

public:

    TimelineKeyframeEvent::Command      KeyframeCommand;
    TimelineKeyframeEvent::KeyframeType NewKeyframeType;
    std::string                         TimelinePath;
    std::string                         KeyframeName;
    int                                 KeyframeIndex;

    float                               Time;
    float                               JumpToTime;
    unsigned int                        TotalLoopCount;
    std::string                         TriggerEvents;

public:
    explicit                        TimelineKeyframeEvent   () {}

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};


DECLARE_ENUM_SERIALIZATION( TimelineKeyframeEvent::Command )
DECLARE_ENUM_SERIALIZATION( TimelineKeyframeEvent::KeyframeType )
DEFINE_PTR_TYPE( TimelineKeyframeEvent )



}	// bv

