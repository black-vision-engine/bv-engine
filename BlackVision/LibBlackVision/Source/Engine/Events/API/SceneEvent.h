#pragma once

#include "Engine/Events/BaseEvent.h"
#include "Serialization/ConversionHelper.h"



namespace bv
{




/**@brief Scene manipulation API.

@section SceneEvent_Commands Commands

This event supports following commands:
- @ref SceneEvent_Commands_AddScene "AddScene"
- @ref SceneEvent_Commands_RemoveScene "RemoveScene"
- @ref SceneEvent_Commands_RemoveAllScenes "RemoveAllScenes"
- @ref SceneEvent_Commands_SceneVisiblitity "SetSceneVisible"
- @ref SceneEvent_Commands_SceneVisiblitity "SetSceneInvisible"
- @ref SceneEvent_Commands_RenameScene "RenameScene"
- @ref SceneEvent_Commands_AttachDetach "AttachScene"
- @ref SceneEvent_Commands_AttachDetach "DetachScene"
- @ref SceneEvent_Commands_AttachDetach "MoveScene"
- @ref SceneEvent_Commands_CopyScene "CopyScene"
- @ref SceneEvent_Commands_SetOutputChannel "SetOutputChannel"

@subsection SceneEvent_Commands_AddScene Adding Scene

Creates and adds new scene on the end of scenes list.

@code{.json}
{
    "Event" : "SceneEvent",
    "EventID" : "3024",
    "Command" : "AddScene",
    "NewSceneName" : "Shapes2D_Examples.scn"
}
@endcode

@subsection SceneEvent_Commands_RemoveScene Removing Scene

Unloads scene from engine.

@code{.json}
{
    "Event" : "SceneEvent",
    "EventID" : "3040",
    "Command" : "RemoveScene",
    "SceneName" : "Shapes2D_Examples.scn"
}
@endcode

@subsection SceneEvent_Commands_RemoveAllScenes Removing All Scenes

Unloads all loaded @ref Scenes "Scenes".
@code{.json}
{
    "Event" : "SceneEvent",
    "EventID" : "3042",
    "Command" : "RemoveAllScenes"
}
@endcode

@subsection SceneEvent_Commands_SceneVisiblitity Scene visibility

Send this event to make Scene visible.
@code{.json}
{
    "Event" : "SceneEvent",
    "EventID" : "3047",
    "Command" : "SetSceneVisible",
    "SceneName" : "Shapes2D_Examples.scn"
}
@endcode

To hide scene:

@code{.json}
{
    "Event" : "SceneEvent",
    "EventID" : "3051",
    "Command" : "SetSceneInvisible",
    "SceneName" : "Shapes2D_Examples.scn"
}
@endcode

@subsection SceneEvent_Commands_RenameScene Renaming Scene

Unloads scene from engine.

@code{.json}
{
    "Event" : "SceneEvent",
    "EventID" : "3062",
    "Command" : "RenameScene",
    "SceneName" : "Shapes2D_Examples.scn",
    "NewSceneName" : "Shapes.scn"
}
@endcode

@subsection SceneEvent_Commands_AttachDetach Attaching/Detaching Scene

DetachScene removes @ref Scenes "Scene" from list but doesn't deletes engine objects.
Detached Scene can be then attached again. This way user can change order of scenes and
what results in changing rendering order.

@code{.json}
{
    "Event" : "SceneEvent",
    "EventID" : "0",
    "Command" : "DetachScene",
    "SceneName" : "Shapes2D_Examples.scn"
}
@endcode

@code{.json}
{
    "Event" : "SceneEvent",
    "EventID" : "0",
    "Command" : "AttachScene",
    "SceneName" : "Shapes2D_Examples.scn",
    "AttachIndex" : "0"
}
@endcode

The same effect can be achived using MoveScene:
@code{.json}
{
    "Event" : "SceneEvent",
    "EventID" : "0",
    "Command" : "MoveScene",
    "SceneName" : "Shapes2D_Examples.scn",
    "AttachIndex" : "0"
}
@endcode

@subsection SceneEvent_Commands_CopyScene Coping Scene

Creates new scene with the same content. Scene name consist of prefix <b>Copy_</b> and copied scene name.
@code{.json}
{
    "Event" : "SceneEvent",
    "EventID" : "0",
    "Command" : "CopyScene",
    "SceneName" : "Shapes2D_Examples.scn"
}
@endcode

@subsection SceneEvent_Commands_SetOutputChannel Setting Scene Output Channel

Assigns @ref Scenes "Scene" to one of 4 RenderChannels.

@code{.json}
{
    "Event" : "SceneEvent",
    "EventID" : "0",
    "Command" : "SetOutputChannel",
    "SceneName" : "Shapes2D_Examples.scn",
    "AttachIndex" : "7"
}
@endcode

*/
class SceneEvent : public RemoteEvent
{
public:

    typedef enum
    {
        AddScene,
        RemoveScene,
        RemoveAllScenes,
        SetSceneVisible,
        SetSceneInvisible,
        RenameScene,
        AttachScene,
        DetachScene,
        MoveScene,
        CopyScene,
        SetOutputChannel,
        Fail            ///< Wrong command
    } Command;

private:

    static const EventType      m_sEventType;
    static std::string          m_sEventName;

public:

    SceneEvent::Command             SceneCommand;
    std::string                     SceneName;
    std::string                     NewSceneName;
    UInt32                          AttachIndex;

public:
    explicit                        SceneEvent   () {}

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};

DECLARE_ENUM_SERIALIZATION( SceneEvent::Command )
DEFINE_PTR_TYPE( SceneEvent )


}	// bv

