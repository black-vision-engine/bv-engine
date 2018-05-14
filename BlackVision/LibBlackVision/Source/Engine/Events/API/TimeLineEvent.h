#pragma once

#include "Engine/Events/BaseEvent.h"
#include "Serialization/ConversionHelper.h"

#include "Engine/Types/Enums.h"



namespace bv
{




/**@brief Timelines events.

@section TimeLineEvent_Commands Commands

This event supports following commands:
- @ref TimeLineEvent_Commands_AddTimeline "AddTimeline"
- @ref TimeLineEvent_Commands_DeleteTimeline "DeleteTimeline"
- @ref TimeLineEvent_Commands_DeleteTimeline "ForceDeleteTimeline"
- @ref TimeLineEvent_Commands_RenameTimeline "RenameTimeline"
- @ref TimeLineEvent_Commands_SetDuration "SetDuration"
- @ref TimeLineEvent_Commands_WrapMethods "SetWrapPreBehavior"
- @ref TimeLineEvent_Commands_WrapMethods "SetWrapPostBehavior"
- @ref TimeLineEvent_Commands_Play "Play"
- @ref TimeLineEvent_Commands_Stop "Stop"
- @ref TimeLineEvent_Commands_Play "PlayReverse"
- @ref TimeLineEvent_Commands_Goto "Goto"
- @ref TimeLineEvent_Commands_Goto "GotoAndPlay"

@subsection TimeLineEvent_Commands_AddTimeline Adding Timelines

Adds timeline as child of <b>TimelineName</b>. Since timeline structure consists only of scene timeline
and it's children, you should set <b>TimelineName</b> to name of @ref Scenes "Scene".

@code{.json}
{
    "EventID" : "26",
    "Event" : "TimeLineEvent",
    "Command" : "AddTimeline",
    "Type" : "Default",
    "TimelineName" : "Shapes2D_Examples.scn",
    "TimelineNewName" : "Timeline2"
}
@endcode

@subsection TimeLineEvent_Commands_DeleteTimeline Removing Timelines

Remove timeline. Set <b>TimelineName</b> to path to timeline. It consists of @ref Scenes "Scene" name and timeline name
separated by %.

@code{.json}
{
    "EventID" : "26",
    "Event" : "TimeLineEvent",
    "Command" : "DeleteTimeline",
    "TimelineName" : "Shapes2D_Examples.scn%Timeline1"
}
@endcode

Operation won't succeed if any @ref Parameters "Parameter" references this timeline. If you want
delete this timeline even then, send <b>ForceDeleteTimeline</b> command.
<b>TimelineNewName</b> contains name of new timeline, which is used to assing these parameters.

@code{.json}
{
    "EventID" : "26",
    "Event" : "TimeLineEvent",
    "Command" : "DeleteTimeline",
    "TimelineName" : "Shapes2D_Examples.scn%Timeline1",
    "TimelineNewName" : "Shapes2D_Examples.scn%FallbackTimeline"
}
@endcode

@subsection TimeLineEvent_Commands_RenameTimeline Renaming Timelines

@code{.json}
{
    "EventID" : "31862",
    "Event" : "TimeLineEvent",
    "Command" : "RenameTimeline",
    "TimelineName" : "Shapes2D_Examples.scn%Timeline1",
    "TimelineNewName" : "Timeline_1"
}
@endcode

@subsection TimeLineEvent_Commands_SetDuration Setting Timelines duration

@code{.json}
{
    "EventID" : "31862",
    "Event" : "TimeLineEvent",
    "Command" : "SetDuration",
    "TimelineName" : "Shapes2D_Examples.scn%Timeline1",
    "Duration" : "10000.0"
}
@endcode

@subsection TimeLineEvent_Commands_WrapMethods Setting Timelines wrap methods

@code{.json}
{
    "EventID" : "26",
    "Event" : "TimeLineEvent",
    "Command" : "SetWrapPreBehavior",
    "TimelineName" : "Shapes2D_Examples.scn%Timeline1",
    "WrapMethod" : "clamp"

}
@endcode

@code{.json}
{
    "EventID" : "26",
    "Event" : "TimeLineEvent",
    "Command" : "SetWrapPostBehavior",
    "TimelineName" : "Shapes2D_Examples.scn%Timeline1",
    "WrapMethod" : "clamp"

}
@endcode

@subsection TimeLineEvent_Commands_Play Play

@code{.json}
{
    "EventID" : "31957",
    "Event" : "TimeLineEvent",
    "Command" : "Play",
    "TimelineName" : "Shapes2D_Examples.scn%default"
}
@endcode

@code{.json}
{
    "EventID" : "31957",
    "Event" : "TimeLineEvent",
    "Command" : "PlayReverse",
    "TimelineName" : "Shapes2D_Examples.scn%default"
}
@endcode

@subsection TimeLineEvent_Commands_Stop Stop

@code{.json}
{
    "EventID" : "31862",
    "Event" : "TimeLineEvent",
    "Command" : "Stop",
    "TimelineName" : "Shapes2D_Examples.scn%default"
}
@endcode

@subsection TimeLineEvent_Commands_Goto Goto

@code{.json}
{
    "EventID" : "31766",
    "Event" : "TimeLineEvent",
    "Command" : "Goto",
    "TimelineName" : "Shapes2D_Examples.scn%default",
    "Time" : "5.000000"
}
@endcode

@code{.json}
{
    "EventID" : "31766",
    "Event" : "TimeLineEvent",
    "Command" : "GotoAndPlay",
    "TimelineName" : "Shapes2D_Examples.scn%default",
    "Time" : "5.000000"
}
@endcode

*/
class TimeLineEvent : public RemoteEvent
{
public:

    typedef enum
    {
        AddTimeline,
        DeleteTimeline,
        ForceDeleteTimeline,
        RenameTimeline,
        SetDuration,
        SetWrapPreBehavior,
        SetWrapPostBehavior,

        Play,
        Stop,
        PlayReverse,
        Goto,
        GotoAndPlay,
        Fail            ///< Wrong command
    } Command;

private:

    static const EventType      m_sEventType;
    static std::string          m_sEventName;

public:

    TimeLineEvent::Command      TimelineCommand;
    std::string                 TimelineName; //path?
    std::string                 NewTimelineName;
    float                       Time;
    TimelineType                TimelineType;
    TimeType                    Duration;
    TimelineWrapMethod          WrapMethod;

public:

    explicit                        TimeLineEvent   () {}

    virtual void                    Serialize           ( ISerializer & ser ) const;
    static IEventPtr                Create              ( IDeserializer & deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string &            Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;

};


DECLARE_ENUM_SERIALIZATION( TimeLineEvent::Command )
DEFINE_PTR_TYPE( TimeLineEvent )


}	// bv
