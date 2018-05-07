#pragma once

#include "Engine/Events/BaseEvent.h"
#include "Serialization/ConversionHelper.h"




namespace bv
{




/**@brief Engine state API.

@section EngineStateEvent_Commands Commands

This event supports following commands:
- @ref EngineStateEvent_Commands_RenderOffscreen "RenderOffscreen"
- @ref EngineStateEvent_Commands_ScreenShot "ScreenShot"
- CloseApplication
- LockEventQueue
- SetGain
- @ref EngineStateEvent_Commands_ListChannelsMapping "OutputCommand"
- @ref EngineStateEvent_Commands_SwitchEditMode "SwitchEditMode"

@subsection EngineStateEvent_Commands_RenderOffscreen RenderOffscreen

@code{.json}
{
	"Event" : "EngineStateEvent",
	"Command" : "RenderOffscreen",
	"FilePath" : "render/Sequence",
	"FPS" : "50",
	"NumberFrames" : "100"
}
@endcode

@subsection EngineStateEvent_Commands_ScreenShot ScreenShot

@code{.json}
{
	"Event" : "EngineStateEvent",
	"Command" : "ScreenShot",
	"FilePath" : "screenshots/Test"
}
@endcode

@subsection EngineStateEvent_Commands_ListChannelsMapping List Channels Mapping

List mapping between @ref Scenes "Scenes" and @ref RenderChannels "Render Channels".

@code{.json}
{
    "Event" : "EngineStateEvent",
    "Command" : "OutputCommand",
    "Request" :
    {
        "Action" : "ListChannelsMapping"
    }
}
@endcode

<b>Example response:</b>

@code{.json}
{
   "ChannelsMapping" : [
      {
         "ChannelIdx" : "2",
         "SceneName" : "FlowerChannel2.scn"
      },
      {
         "ChannelIdx" : "0",
         "SceneName" : "Shapes2D_Examples.scn"
      },
      {
         "ChannelIdx" : "0",
         "SceneName" : "BluredShapes.scn"
      }
   ],
   "EventID" : "222",
   "Success" : "true",
   "cmd" : "OutputCommand"
}
@endcode

@subsection EngineStateEvent_Commands_AssignChannel Assign Render Channel

Assign @ref RenderChannels "Render Channel" for Scene.

@code{.json}
{
    "Event" : "EngineStateEvent",
    "Command" : "OutputCommand",
    "Request" :
    {
        "Action" : "AssignChannel",
        "SceneName" : "Shapes2D_Examples.scn",
        "ChannelIdx" : "0"
    }
}
@endcode

@subsection EngineStateEvent_Commands_SwitchEditMode Switch edit mode

In edit mode engine renders bounding boxes, grid lines and other helpers for editor.
To properly render these helpers with fullscreen effects, engine must make some additional
depth buffer passes (effects override depth in render target). This causes performance penalty for
working in edit mode.

@code{.json}
{
	"Event" : "EngineStateEvent",
	"Command" : "SwitchEditMode"
}
@endcode

*/
class EngineStateEvent : public RemoteEvent
{
public:
    typedef enum
    {
        RenderOffscreen,
        ScreenShot,
        CloseApplication,
        LockEventQueue,
        SetGain,
        OutputCommand,
        SwitchEditMode,
        Fail            ///< Wrong command
    } Command;
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;
public:

    float                       FPS;
    unsigned int                NumFrames;
    std::string                 FilePath;
    Command                     RenderingCommand;
    float						Gain;

    IDeserializer *             Request;

public:

    explicit                        EngineStateEvent   () {};

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};


DECLARE_ENUM_SERIALIZATION( EngineStateEvent::Command )
DEFINE_PTR_TYPE( EngineStateEvent )




}	// bv

