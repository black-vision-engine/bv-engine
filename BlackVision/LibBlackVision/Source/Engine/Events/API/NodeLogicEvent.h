#pragma once


#include "Engine/Events/BaseEvent.h"
#include "Serialization/ConversionHelper.h"





namespace bv
{




/**@brief Node Logics manipulation API.

@section NodeLogicEvent_Commands Commands

This event supports following commands:
- @ref NodeLogicEvent_Commands_AddNodeLogic "AddNodeLogic"
- @ref NodeLogicEvent_Commands_DeleteNodeLogic "DeleteNodeLogic"
- @ref NodeLogicEvent_Commands_LogicAction "LogicAction"

@subsection NodeLogicEvent_Commands_AddNodeLogic Adding Logic

Adds Logic to Scene. Parameter <b>type</b> contains Logic type that will be created.
Json Object <b>logic</b> can have logic dependent content necessary to initialization.

@code{.json}
{
    "EventID" : "10840",
    "Event" : "NodeLogicEvent",
    "SceneName" : "Shapes2D_Examples.scn",
    "NodeName" : "#0/#0",
    "Command" : "AddNodeLogic",
    "Action" : 
    {
        "TimelineName":"Shapes2D_Examples.scn%default",
        "logic":
        {
            "type":"MaxSize"
        }
    }
}
@endcode

@subsection NodeLogicEvent_Commands_DeleteNodeLogic Removing Logic

Removes Logic.
@todo Empty <b>Action</b> field is required. Remove in future.

@code{.json}
{
    "EventID" : "10840",
    "Event" : "NodeLogicEvent",
    "SceneName" : "Shapes2D_Examples.scn",
    "NodeName" : "#0/#0",
    "Command" : "DeleteNodeLogic",
    "Action" : {}
}
@endcode

@subsection NodeLogicEvent_Commands_LogicAction Custom Logic Commands

Sends custom Logic command. Fill parameter <b>Action</b> with Logic dependent content.

@code{.json}
{
    "EventID" : "10840",
    "Event" : "NodeLogicEvent",
    "SceneName" : "Shapes2D_Examples.scn",
    "NodeName" : "#0/#0",
    "Command" : "LogicAction",
    "Action" : {}
}
@endcode


*/
class NodeLogicEvent : public RemoteEvent
{
public:

    typedef enum
    {
        AddNodeLogic,
        DeleteNodeLogic,
        LogicAction,
        Fail            ///< Wrong command
    } Command;

private:

    static const EventType      m_sEventType;
    static std::string          m_sEventName;

public:

    Command                     WidgetCommand;
    std::string                 NodeName;
    std::string                 SceneName;
    IDeserializer *             Action;

public:
    explicit                        NodeLogicEvent      (){ Action = nullptr; }
                                    ~NodeLogicEvent     (){ delete Action; }

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};


DECLARE_ENUM_SERIALIZATION( NodeLogicEvent::Command )
DEFINE_PTR_TYPE( NodeLogicEvent )


}	// bv
