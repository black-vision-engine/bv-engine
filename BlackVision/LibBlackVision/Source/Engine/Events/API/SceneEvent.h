#pragma once

#include "Engine/Events/BaseEvent.h"




namespace bv
{




/**@brief Scene manipulation API.

@section SceneEvent_Commands Commands

This event supports following commands:
- AddScene
- RemoveScene
- RemoveAllScenes
- SetSceneVisible
- SetSceneInvisible
- RenameScene
- AttachScene
- DetachScene
- MoveScene
- CopyScene
- SetOutputChannel

@subsection SceneEvent_Commands_AddScene Adding Scene



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

