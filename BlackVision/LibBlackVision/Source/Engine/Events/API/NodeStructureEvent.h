#pragma once

#include "Engine/Events/BaseEvent.h"
#include "Serialization/ConversionHelper.h"





namespace bv
{



/**@brief Nodes manipulation API.

@section NodeStructureEvent_Commands Commands

- @ref NodeStructureEvent_Commands_AddNode "AddNode"
- @ref NodeStructureEvent_Commands_RemoveNode "RemoveNode"
- @ref NodeStructureEvent_Commands_Visibility "SetNodeVisible"
- @ref NodeStructureEvent_Commands_Visibility "SetNodeInvisible"
- SelectNode
- UnselectNodes
- @ref NodeStructureEvent_Commands_RenameNode "RenameNode"
- AttachNode
- DetachNode
- @ref NodeStructureEvent_Commands_MoveNode "MoveNode"
- @ref NodeStructureEvent_Commands_CopyNode "CopyNode"

@subsection NodeStructureEvent_Commands_AddNode Adding Nodes

Adds new node As child of Node specified in <b>NodeName</b> parameter.

@code{.json}
{
    "EventID" : "10715",
    "Event" : "NodeStructureEvent",
    "Command" : "AddNode",
    "SceneName" : "Shapes2D_Examples.scn",
    "NodeName" : "#0",
    "NewNodeName" : "NewNode",
    "Request" :
    {
        "AddTransformPlugin" : "true"
    }
}
@endcode

@subsection NodeStructureEvent_Commands_RemoveNode Removing Nodes

Removes node from scene.

@code{.json}
{
    "EventID" : "10715",
    "Event" : "NodeStructureEvent",
    "Command" : "RemoveNode",
    "SceneName" : "Shapes2D_Examples.scn",
    "NodeName" : "#0/#2"
}
@endcode

@subsection NodeStructureEvent_Commands_Visibility Nodes Visibility

Makes node and whole subtree invisible.

@code{.json}
{
    "EventID" : "10715",
    "Event" : "NodeStructureEvent",
    "Command" : "SetNodeInvisible",
    "SceneName" : "Shapes2D_Examples.scn",
    "NodeName" : "#0/#1"
}
@endcode

Make Node and whole subtree visible. If some Nodes in subtree where invisible they stay invisible after this event.

@code{.json}
{
    "EventID" : "10715",
    "Event" : "NodeStructureEvent",
    "Command" : "SetNodeVisible",
    "SceneName" : "Shapes2D_Examples.scn",
    "NodeName" : "#0/#1"
}
@endcode

@subsection NodeStructureEvent_Commands_RenameNode Renaming Nodes

@code{.json}
{
    "EventID" : "10763",
    "Event" : "NodeStructureEvent",
    "Command" : "RenameNode",
    "SceneName" : "Shapes2D_Examples.scn",
    "NodeName" : "#0/#0/#1",
    "NewNodeName" : "solid circle"
}
@endcode

@subsection NodeStructureEvent_Commands_MoveNode Moving Nodes

Moves Node in tree.

Parameter name         	| Type          | Description
----------------------- | ------------- | -----------
DestSceneName           | string        | Destination Scene, where Node will be moved.
DestPath                | string        | New parent for moved Node.
DestIndex               | string        | Index of the Node in new subtree.
SrcSceneName            | string        | Scene of Node to move.
SrcPath                 | string        | Path of Node to move.



@code{.json}
{
    "EventID" : "10779",
    "Event" : "NodeStructureEvent",
    "Command" : "MoveNode",
    "Request" :
    {
        "DestSceneName" : "Shapes2D_Examples.scn",
        "DestPath" : "#0/#1",
        "DestIndex" : "0",
        "SrcSceneName" : "Shapes2D_Examples.scn",
        "SrcPath" : "#0/#0/#0"
    }
}
@endcode

@subsection NodeStructureEvent_Commands_CopyNode Coping Nodes

Copies Node. Meaning of parameters is the same as in @ref NodeStructureEvent_Commands_MoveNode "MoveNode" command.

@code{.json}
{
    "EventID" : "10799",
    "Event" : "NodeStructureEvent",
    "Command" : "CopyNode",
    "Request" :
    {
        "DestSceneName" : "Shapes2D_Examples.scn",
        "DestPath" : "#0/#1",
        "DestIndex" : "0",
        "SrcSceneName" : "Shapes2D_Examples.scn",
        "SrcPath" : "#0/#0"
    }
}
@endcode


*/
class NodeStructureEvent : public RemoteEvent
{
public:

    typedef enum
    {
        AddNode,
        RemoveNode,
        SetNodeVisible,
        SetNodeInvisible,
        SelectNode,
        UnselectNodes,
        RenameNode,
        AttachNode,
        DetachNode,
        MoveNode,
        CopyNode,
        Fail            ///< Wrong command
    } Command;

private:

    static const EventType      m_sEventType;
    static std::string          m_sEventName;

public:

    NodeStructureEvent::Command     SceneCommand;
    std::string                     SceneName;
    std::string                     NodePath;
    std::string                     NewNodeName;
    UInt32                          AttachIndex;
    IDeserializer *                 Request;

public:

    explicit                        NodeStructureEvent    () { Request = nullptr; }
    ~NodeStructureEvent   () { delete Request; }

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};


DECLARE_ENUM_SERIALIZATION( NodeStructureEvent::Command )
DEFINE_PTR_TYPE( NodeStructureEvent )

}	// bv

