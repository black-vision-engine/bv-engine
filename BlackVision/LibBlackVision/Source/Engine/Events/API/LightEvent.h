#pragma once

#include "Engine/Events/BaseEvent.h"
#include "Serialization/ConversionHelper.h"




namespace bv
{



/**@brief Lights managment API.

@section LightEvent_Commands Commands

This event supports following commands:
- AddLight
- RemoveLight

@subsection LightEvent_Commands_AddLight Adding Lights

Adds @ref Lights "Light" on the end of Scene lights list.

@code{.json}
{
    "EventID" : "10811",
    "Event" : "LightEvent",
    "Command" : "AddLight",
    "SceneName" : "Shapes2D_Examples.scn",
    "TimelineName" : "Shapes2D_Examples.scn%default",
    "LightType" : "spot"
}
@endcode

@subsection LightEvent_Commands_RemoveLight Removing Lights

Removes light from <b>Index</b>.

@code{.json}
{
    "EventID" : "10815",
    "Event" : "LightEvent",
    "Command" : "RemoveLight",
    "SceneName" : "Shapes2D_Examples.scn",
    "Index" : "0"
}
@endcode

*/
class LightEvent : public RemoteEvent
{
public:

    typedef enum
    {
        AddLight,
        RemoveLight,
        Fail            ///< Wrong command
    } Command;

private:

    static const EventType          m_sEventType;
    static std::string              m_sEventName;

public:

    LightEvent::Command             SceneCommand;
    std::string                     SceneName;
    std::string                     LightType;
    UInt32                          LightIndex;
    std::string                     TimelinePath;

public:
    explicit                        LightEvent   () {}

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};

DECLARE_ENUM_SERIALIZATION( LightEvent::Command )
DEFINE_PTR_TYPE( LightEvent )




}	// bv
