#pragma once


#include "Engine/Events/BaseEvent.h"
#include "Serialization/ConversionHelper.h"






namespace bv
{



/**@brief Plugins manipulation API.

@section PluginStructureEvent_Commands Commands

This event supports following commands:
- @ref PluginStructureEvent_Commands_AddPlugin "AddPlugin"
- @ref PluginStructureEvent_Commands_RemovePlugin "RemovePlugin"
- @ref PluginStructureEvent_Commands_AttachingDetaching "AttachPlugin"
- @ref PluginStructureEvent_Commands_AttachingDetaching "DetachPlugin"
- @ref PluginStructureEvent_Commands_CopyPlugin "CopyPlugin"
- @ref PluginStructureEvent_Commands_MovePlugin "MovePlugin"

@subsection PluginStructureEvent_Commands_AddPlugin Adding Plugins

Adds new Plugins to Node's plugins list.


Parameter name         	| Type          | Description
----------------------- | ------------- | -----------
SceneName               | string        | Scene name.
NodeName                | string        | Address of node.
PluginUID               | string        | Plugin's unique id used to create descriptor.
PluginName              | string        | Name of Plugins, that will be used in future to access this plugin.
AttachIndex             | UInt32        | Index of plugin in plugins list. It can be greater than length of this list - plugins will be added on end.


@code{.json}
{
    "EventID" : "10911",
    "Event" : "PluginStructureEvent",
    "SceneName" : "Shapes2D_Examples.scn",
    "AttachIndex" : "2",
    "PluginUID" : "DEFAULT_TRIANGLE",
    "NodeName" : "#0/#1/#1",
    "PluginName" : "triangle",
    "Command" : "AddPlugin"
}
@endcode

@subsection PluginStructureEvent_Commands_RemovePlugin RemovingPlugins

Removes Plugins from list.

@code{.json}
{
    "EventID" : "10915",
    "Event" : "PluginStructureEvent",
    "SceneName" : "Shapes2D_Examples.scn",
    "AttachIndex" : "0",
    "NodeName" : "#0/#1/#1",
    "PluginName" : "triangle",
    "Command" : "RemovePlugin"
}
@endcode

@subsection PluginStructureEvent_Commands_AttachingDetaching Attaching/Detaching Plugins

Detach Plugin.

@code{.json}
{
    "EventID" : "10915",
    "Event" : "PluginStructureEvent",
    "SceneName" : "Shapes2D_Examples.scn",
    "NodeName" : "#0/#0/#1",
    "PluginName" : "circle",
    "Command" : "DetachPlugin"
}
@endcode

Attach Plugin.

@code{.json}
{
    "EventID" : "10915",
    "Event" : "PluginStructureEvent",
    "SceneName" : "Shapes2D_Examples.scn",
    "NodeName" : "#0/#0/#1",
    "PluginName" : "circle",
    "Command" : "AttachPlugin",
    "AttachIndex" : "2"
}
@endcode

@subsection PluginStructureEvent_Commands_CopyPlugin Coping Plugins

Copies plugins between Nodes in the same @ref Scenes "Scene.

@code{.json}
{
    "EventID" : "10779",
    "Event" : "PluginStructureEvent",
    "Command" : "CopyPlugin",
    "Request" :
    {
        "DestPath" : "#0/#1",
        "DestIndex" : "0",
        "SrcSceneName" : "Shapes2D_Examples.scn",
        "SrcPath" : "#0/#0/#0",
        "SrcName" : "circle"
    }
}
@endcode

@subsection PluginStructureEvent_Commands_MovePlugin Moving Plugins

Moves plugins between Nodes in the same @ref Scenes "Scene.

@code{.json}
{
    "EventID" : "10780",
    "Event" : "PluginStructureEvent",
    "Command" : "MovePlugin",
    "Request" :
    {
        "DestPath" : "#0/#1",
        "DestIndex" : "0",
        "SrcSceneName" : "Shapes2D_Examples.scn",
        "SrcPath" : "#0/#0/#0",
        "SrcName" : "circle"
    }
}
@endcode


*/
class PluginStructureEvent : public RemoteEvent
{
public:

    typedef enum
    {
        AddPlugin,
        RemovePlugin,
        AttachPlugin,
        DetachPlugin,
        CopyPlugin,
        MovePlugin,
        Fail            ///< Wrong command
    } Command;

private:

    static const EventType      m_sEventType;
    static std::string          m_sEventName;

public:

    PluginStructureEvent::Command   PluginCommand;
    std::string                     NodePath;
    std::string                     SceneName;
    std::string                     PluginName;
    std::string                     PluginUID;
    std::string                     TimelinePath;
    unsigned int                    AttachIndex;
    IDeserializer *                 Request;

public:
    explicit                        PluginStructureEvent    () { Request = nullptr; }
                                    ~PluginStructureEvent   () { delete Request; }

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};


DECLARE_ENUM_SERIALIZATION( PluginStructureEvent::Command )
DEFINE_PTR_TYPE( PluginStructureEvent )




}	// bv
