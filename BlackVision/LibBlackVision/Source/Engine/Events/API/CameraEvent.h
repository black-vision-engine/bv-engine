#pragma once

#include "Engine/Events/BaseEvent.h"
#include "Serialization/ConversionHelper.h"





namespace bv
{



/**@brief Cameras managment API.

@section CameraEvent_Commands Commands

This event supports following commands:
- @ref CameraEvent_Commands_AddCamera "AddCamera"
- @ref CameraEvent_Commands_RemoveCamera "RemoveCamera"
- @ref CameraEvent_Commands_SetCurrentCamera "SetCurrentCamera"

@subsection CameraEvent_Commands_AddCamera Adding Cameras

Adds new @ref Cameras "Camera" on the end of list.

@code{.json}
{
    "Event" : "CameraEvent",
    "EventID" : "3024",
    "Command" : "AddCamera",
    "SceneName" : "Shapes2D_Examples.scn"
}
@endcode

@subsection CameraEvent_Commands_RemoveCamera Removing Cameras

Removes @ref Cameras "Camera" under index passed in <b>AttachIndex</b> parameter.
@note This changes indicies of cameras following after this index.

@code{.json}
{
    "Event" : "CameraEvent",
    "EventID" : "3024",
    "Command" : "RemoveCamera",
    "SceneName" : "Shapes2D_Examples.scn",
    "AttachIndex" : "1"
}
@endcode

@subsection CameraEvent_Commands_SetCurrentCamera Setting active Camera

Sets one of Scene cameras as current. This camera is used in rendering of Scene.

@code{.json}
{
    "Event" : "CameraEvent",
    "EventID" : "3024",
    "Command" : "SetCurrentCamera",
    "SceneName" : "Shapes2D_Examples.scn",
    "AttachIndex" : "1"
}
@endcode




*/
class CameraEvent : public RemoteEvent
{
public:

    typedef enum
    {
        AddCamera,
        RemoveCamera,
        SetCurrentCamera,
        Fail            ///< Wrong command
    } Command;

private:

    static const EventType          m_sEventType;
    static std::string              m_sEventName;

public:

    CameraEvent::Command            CameraCommand;
    std::string                     SceneName;
    UInt32                          CameraIndex;

public:
    explicit                        CameraEvent   () {}

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};

DECLARE_ENUM_SERIALIZATION( CameraEvent::Command )
DEFINE_PTR_TYPE( CameraEvent )


}	// bv


