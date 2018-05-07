#pragma once

#include "Engine/Events/BaseEvent.h"
#include "Serialization/ConversionHelper.h"

#include "Mathematics/glm_inc.h"


namespace bv
{





/**@brief Mouse API

@section MouseEvent_Commands Commands

This event supports following commands:
- MouseUp
- @ref MouseEvent_Commands_MouseDown "MouseDown"
- MouseMove

@subsection MouseEvent_Commands_MouseDown Mouse Down

Editor can send event that emulates clicking application screen. Event returns
clicked node. If you set <b>AutoSelect</b> flag, clicked node will be automatically selected.

<b>MouseX and MouseY</b> are pixel coordinates scaled to Full HD screen.

@code{.json}
{
    "EventID" : "68",
    "Event" : "MouseEvent",
    "MouseX" : "866",
    "MouseY" : "568",
    "Command" : "MouseDown",
    "AutoSelect" : "true"
    "AutoSelectColor" : "1,0,0,1"
}
@endcode

<b>Example response:</b>

@code{.json}
{
   "ClickedNode" : "#0/#0/#2",
   "EventID" : "68",
   "Scene" : "Shapes2D_Examples.scn",
   "Success" : "true",
   "cmd" : "MouseDown"
}
@endcode

*/
class MouseEvent : public RemoteEvent
{
public:

    typedef enum
    {
        MouseUp,
        MouseDown,
        MouseMove,
        Fail            ///< Wrong command
    } Command;

private:

    static const EventType      m_sEventType;
    static std::string          m_sEventName;

public:

    MouseEvent::Command             MouseCommand;

    Float32                         MouseX;
    Float32                         MouseY;
    bool                            AutoSelect;
    glm::vec4                       AutoSelectColor;

public:
    explicit                        MouseEvent          () {}

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};


DECLARE_ENUM_SERIALIZATION( MouseEvent::Command )
DEFINE_PTR_TYPE( MouseEvent )

}

