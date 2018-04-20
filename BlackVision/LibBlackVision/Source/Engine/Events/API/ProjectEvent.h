#pragma once

#include "Engine/Events/BaseEvent.h"
#include "Serialization/ConversionHelper.h"





namespace bv
{


/**@brief Project manipulation API.

@section ProjectEvent_Commands Commands

- NewProject
- LoadProject
- SetCurrentProject
- MoveProject
- DeleteProject
- RenameProject
- CreateFolder
- DeleteFolder
- RenameFolder
- MoveFolder
- CopyFolder
- AddExistingSceneToProject
- CreateSceneInProject
- @ref ProjectEvent_Commands_SaveScene "SaveScene"
- @ref ProjectEvent_Commands_LoadScene "LoadScene"
- RemoveScene
- CopyScene
- MoveScene
- SavePreset
- LoadPreset
- EditPreset
- CopyAsset
- MoveAsset
- RemoveAsset
- ImportAsset
- GenerateMeshThumbnail

@subsection ProjectEvent_Commands_SaveScene Saving Scenes

Save @ref Scenes "Scene" to path relative to ProjectManager scenes directory passed in <b>saveTo</b> parameter.
If path already exists, scene won't be saved and event will return false. If you want to overwrite existing scene, set <b>forceSave</b> to true.

Event will rename existing scene. If you don't want this behavior set <b>renameScene</b> flag to false.

@code{.json}
{
    "Event" : "ProjectEvent",
    "EventID" : "2",
    "Command" : "SaveScene",
    "Request" :
    {
        "sceneName" : "Shapes2D_Examples.scn",
        "saveTo" : "Basic/Shapes2D.scn",
        "forceSave" : "true",
        "renameScene" : "false"
    }
}
@endcode

@subsection ProjectEvent_Commands_LoadScene Loading Scenes

Loads scene from Project Manager directory.

@code{.json}
{
    "Event" : "ProjectEvent",
    "EventID" : "3",
    "Command" : "LoadScene",
    "Request" :
    {
        "sceneName" : "Shapes2D_Examples.scn"
    }
}
@endcode

*/
class ProjectEvent : public RemoteEvent
{
public:

    typedef enum
    {
        NewProject,
        LoadProject,
        SetCurrentProject,
        MoveProject,
        DeleteProject,
        RenameProject,

        CreateFolder,
        DeleteFolder,
        RenameFolder,
        MoveFolder,
        CopyFolder,

        AddExistingSceneToProject,
        CreateSceneInProject,

        SaveScene,
        LoadScene,
        RemoveScene,
        CopyScene,
        MoveScene,

        SavePreset,
        LoadPreset,
        EditPreset,

        CopyAsset,
        MoveAsset,
        RemoveAsset,
        ImportAsset,

        GenerateMeshThumbnail,//temp testing event

        Fail            ///< Wrong command
    } Command;

private:

    static const EventType      m_sEventType;
    static std::string          m_sEventName;

public:

    ProjectEvent::Command           ProjectCommand;
    IDeserializer *                 Request;

public:
    explicit                        ProjectEvent   () { Request = nullptr; }
    ~ProjectEvent   () { delete Request; }

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};


DECLARE_ENUM_SERIALIZATION( ProjectEvent::Command )
DEFINE_PTR_TYPE( ProjectEvent )


}	// bv


