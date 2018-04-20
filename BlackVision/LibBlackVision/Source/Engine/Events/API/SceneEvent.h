#pragma once

#include "Engine/Events/BaseEvent.h"




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
- AttachScene
- DetachScene
- MoveScene
- CopyScene
- SetOutputChannel

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

