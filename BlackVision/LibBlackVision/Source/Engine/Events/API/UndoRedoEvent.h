#pragma once


#include "Engine/Events/BaseEvent.h"
#include "Serialization/ConversionHelper.h"




namespace bv
{




/**@brief Undo/Redo operation.

@section CameraEvent_Commands Commands

This event supports following commands:
- @ref UndoRedoEvent_Commands_Undo "Undo"
- @ref UndoRedoEvent_Commands_Redo "Redo"
- @ref UndoRedoEvent_Commands_SetOperationsBufferSize "SetOperationsBufferSize"

Note that Undo/Redo functionality is devided into parts:
- @ref Nodes, @ref Plugins, @ref NodeEffects "Effects", @ref NodeLogics "Logics", @ref Timelines and @ref Lights structure is engine responsibility.
- Setting @ref Parameters, handling @ref NodeLogics "Logics" commands, loading assets are resposibility of external tools.

Check @ref IRevertable interface and derived classes to see which undo/redo operations are implemented in engine.

@subsection UndoRedoEvent_Commands_Undo Undo operation

Undo last operations on @ref Scenes "Scene".

@code{.json}
{
	"Event" : "UndoRedoEvent",
	"Command" : "Undo",
	"Steps" : "1",
    "SceneName" : "Shapes2D_Examples.scn"
}
@endcode

@subsection UndoRedoEvent_Commands_Redo Redo operation

Redo last operations on @ref Scenes "Scene".

@code{.json}
{
	"Event" : "UndoRedoEvent",
	"Command" : "Redo",
	"Steps" : "1",
    "SceneName" : "Shapes2D_Examples.scn"
}
@endcode

@subsection UndoRedoEvent_Commands_SetOperationsBufferSize Set operations buffer size

Set maximal number of operations which can be rewinded.

@code{.json}
{
	"Event" : "UndoRedoEvent",
	"Command" : "Redo",
	"MaxOperations" : "40"
    "SceneName" : "Shapes2D_Examples.scn"
}
@endcode

*/
class UndoRedoEvent : public RemoteEvent
{
public:
    typedef enum
    {
        Undo,
        Redo,
        SetOperationsBufferSize,

        Fail            ///< Wrong command
    } Command;

private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;
public:

    Command         UndoCommand;
    std::string     SceneName;
    UInt16          NumSteps;
    UInt16          Size;

public:

    explicit                        UndoRedoEvent       () {};

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};


DECLARE_ENUM_SERIALIZATION( UndoRedoEvent::Command )
DEFINE_PTR_TYPE( UndoRedoEvent )


}	// bv


