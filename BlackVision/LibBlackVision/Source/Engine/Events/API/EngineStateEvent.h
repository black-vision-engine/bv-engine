#pragma once

#include "Engine/Events/BaseEvent.h"
#include "Serialization/ConversionHelper.h"




namespace bv
{




/**@brief Engine state API.

@section EngineStateEvent_Commands Commands

This event supports following commands:
- RenderOffscreen
- ScreenShot
- CloseApplication
- LockEventQueue
- SetGain
- @ref EngineStateEvent_Commands_ListChannelsMapping OutputCommand
- SwitchEditMode

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
         "ChannelIdx" : "1",
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

