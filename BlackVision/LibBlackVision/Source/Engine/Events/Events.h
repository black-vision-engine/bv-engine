#pragma once

#include "Engine/Events/BaseEvent.h"
#include "Engine/Models/Plugins/Interfaces/IPlugin.h"
#include "Engine/Models/Interfaces/IModelNode.h"
#include "Engine/Events/EventHelpers.h"

#include  "Mathematics/glm_inc.h"


namespace bv
{

namespace model
{
    class IModelNode;
}

class Renderer;

// ************************************* PluginAddedEvent *************************************
class PluginAddedEvent : public BaseEvent
{
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;

    model::IPluginConstPtr      m_addedPlugin;
public:

    explicit                        PluginAddedEvent();
    explicit                        PluginAddedEvent( model::IPluginConstPtr plugin );

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;

    model::IPluginConstPtr          GetPlugin       () const;
};

DEFINE_PTR_TYPE(PluginAddedEvent)

// ************************************* NodeAddedEvent *************************************
class NodeAddedEvent;
DEFINE_PTR_TYPE(NodeAddedEvent)
DEFINE_CONST_PTR_TYPE(NodeAddedEvent)

class NodeAddedEvent : public BaseEvent
{
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;

    model::IModelNodeConstPtr   m_addedNode;
public:
    explicit                        NodeAddedEvent      ();
    explicit                        NodeAddedEvent      ( model::IModelNodeConstPtr node );

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;

    model::IModelNodeConstPtr       GetNode             () const;
};


// ************************************* FrameRenderedEvent *************************************
class FrameRenderedEvent : public BaseEvent
{
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;

    Renderer *          m_renderer;

    int                 m_width;
    int                 m_height;
public:
    explicit                        FrameRenderedEvent  ( Renderer * renderer );

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;

    Renderer *                      Renderer            ();

    void                            SetResolution       ( int w, int h );

    int                             Width               () const;
    int                             Height              () const;
};

DEFINE_PTR_TYPE(FrameRenderedEvent)

// ************************************* TransformSetEvent *************************************
class TransformSetEvent : public BaseEvent
{
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;

    glm::vec3           m_translation;
    glm::vec3           m_scale;
public:

    explicit                        TransformSetEvent   ();

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;

    void                            SetTranslation      ( const glm::vec3 & translation );
    void                            SetScale            ( const glm::vec3 & scale );

    const glm::vec3 &               Translation         () const;
    const glm::vec3 &               Scale               () const;
};

DEFINE_PTR_TYPE(TransformSetEvent)

// ***************************** SET TEXT EVENT *************************
class KeyPressedEvent : public BaseEvent
{
private:
    static const EventType          m_sEventType;
    static std::string              m_sEventName;
    
    char        m_char;
public:

    explicit                        KeyPressedEvent     ();

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;

    void                            SetChar             ( unsigned char c );
    unsigned char                   GetChar             () const;
};

DEFINE_PTR_TYPE( KeyPressedEvent )


// ************************************* LoadAssetEvent *************************************
class LoadAssetEvent : public BaseEvent
{
private:
    static const EventType          m_sEventType;
    static std::string              m_sEventName;
public:
    std::string                     NodeName;
    std::string                     SceneName;
    std::string                     PluginName;
    std::string                     AssetData;

    int                             SockID;
public:
    explicit                        LoadAssetEvent();

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};


DEFINE_PTR_TYPE( LoadAssetEvent )


// ************************************* ParamKeyEvent *************************************
class ParamKeyEvent : public BaseEvent
{
public:
    typedef enum
    {
        AddKey,
        UpdateKey,
        RemoveKey,
        SetInterpolatorType,
        SetInterpolatorPreWrapMethod,
        SetInterpolatorPostWrapMethod,
        Fail            ///< Wrong command
    } Command;
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;
public:
    std::string                     NodeName;
    std::string                     PluginName;
    std::string                     ParamName;
    std::string                     SceneName;
    std::wstring                    Value;

    float                           Time;
    ParamKeyEvent::Command          KeyCommand;

public:
    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;


    virtual EventType               GetEventType        () const;
    virtual const std::string &     GetName             () const;

    static EventType                Type                ();
    static std::string&             Name                ();
};
DEFINE_PTR_TYPE( ParamKeyEvent )


// ************************************* SceneEvent Event *************************************
class SceneEvent : public BaseEvent
{
public:
    typedef enum
    {
        AddScene,
        RemoveScene,
        SetSceneVisible,
        SetSceneInvisible,
        RenameScene,
        AttachScene,
        DetachScene,
        MoveScene,
        CopyScene,
        Fail            ///< Wrong command
    } Command;
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;
public:
    SceneEvent::Command				SceneCommand;
    std::string                     SceneName;
    std::string                     NewSceneName;
	UInt32							AttachIndex;

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

DEFINE_PTR_TYPE( SceneEvent )


// ************************************* SceneStructureEvent Event *************************************
class NodeStructureEvent : public BaseEvent
{
public:
    typedef enum
    {
        AddNode,
        RemoveNode,
        SetNodeVisible,
        SetNodeInvisible,
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
	UInt32							AttachIndex;
    std::string                     Request;

    std::string                     TimelinePath; //not used
public:
    explicit                        NodeStructureEvent   () {}

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};

DEFINE_PTR_TYPE( NodeStructureEvent )


// ************************************* PluginStructureEvent Event *************************************
class PluginStructureEvent : public BaseEvent
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
	std::string						Request;
    unsigned int                    AttachIndex;
public:
    explicit                        PluginStructureEvent    () {}

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};

DEFINE_PTR_TYPE( PluginStructureEvent )


// ************************************* ProjectEvent Event *************************************
class ProjectEvent : public BaseEvent
{
public:
    typedef enum
    {
        NewProject,
        SetCurrentProject,
        SaveScene,
        LoadScene,
        RemoveScene,
        CopyScene,
        MoveScene,
        LoadProject,
        CopyAsset,
        MoveAsset,
        RemoveAsset,
        ImportAsset,
        Fail            ///< Wrong command
    } Command;
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;
public:
    ProjectEvent::Command           ProjectCommand;
    std::string                     Request;
public:
    explicit                        ProjectEvent   () {}

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};

DEFINE_PTR_TYPE( ProjectEvent )


// ************************************* Response Event *************************************
class ResponseEvent : public BaseEvent
{
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;
public:
     std::wstring               Response;
public:
    explicit                        ResponseEvent() {}

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};

DEFINE_PTR_TYPE( ResponseEvent )

    
// ************************************* Information Event *************************************
//
class InfoEvent : public BaseEvent
{
public:
    typedef enum
    {
        TreeStructure,
        ListAssets,
        ListProjectNames,
        ListScenes,
        ListAssetsPaths,
        ListCategoriesNames,
        ListProjects,
        Performance,
        Timelines,
        NodeInfo,
        MinimalSceneInfo,
        Videocards,
        Fail            ///< Wrong command
    } Command;
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;
public:
    InfoEvent::Command          InfoCommand;
    std::string                 Request;

public:
    explicit                        InfoEvent   () {}

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};

template<> InfoEvent::Command       SerializationHelper::WString2T       ( const std::wstring& s );
template<> const std::wstring&      SerializationHelper::T2WString       ( InfoEvent::Command t );

DEFINE_PTR_TYPE( InfoEvent )


// ************************************* TimelineCmd *************************************
class TimeLineEvent : public BaseEvent
{
public:
    typedef enum
    {
		AddTimeline,
        DeleteTimeline,
        ForceDeleteTimeline,
        RenameTimeline,
        SetDuration,
        SetWrapPreBehavior,
        SetWrapPostBehavior,

        Play,
        Stop,
        PlayReverse,
        Goto,
        GotoAndPlay,
        Fail            ///< Wrong command
    } Command;
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;
public:
    TimeLineEvent::Command      TimelineCommand;
    std::string                 SceneName;
    std::string                 TimelineName; //path?
    std::string                 NewTimelineName;
    float                       Time;
    TimeType                    Duration;
	TimelineWrapMethod			WrapMethod;

public:
    explicit                        TimeLineEvent   () {}

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};

DEFINE_PTR_TYPE( TimeLineEvent )


// ************************************* TimerEvent *************************************
class TimerEvent : public BaseEvent
{
public:
    typedef enum
    {
        Start,
        Stop,
        Reset,
        SetTime,
        SetTimeStart,
        SetTimeStop,
        Fail            ///< Wrong command
    } Command;
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;
public:
    std::string                 NodeName;
    std::string                 SceneName;
    Command                     TimerCommand;
    float                       Hours;
    float                       Minutes;
    float                       Seconds;
    float                       Milliseconds;
public:

    explicit                        TimerEvent   (){};

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string&      GetName             () const;
    virtual EventType               GetEventType        () const;
};

DEFINE_PTR_TYPE( TimerEvent )


// ************************************* WidgetEvent *************************************
class WidgetEvent : public BaseEvent
{
public:
    typedef enum
    {
        Crawl,
        Counter,
        Fail            ///< Wrong command
    } Command;
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;
public:
    Command                     WidgetCommand;
    std::string                 NodeName;
    std::string                 SceneName;
	std::string                 Action;
    float						Time;
public:
    explicit                        WidgetEvent			(){};

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};

DEFINE_PTR_TYPE( WidgetEvent )


// ************************************* VideoCardEvent *************************************
class VideoCardEvent : public BaseEvent
{
public:
    typedef enum
    {
        EnableOutput,
        DisableOutput,
        EnableKey,
        DisableKey,
        ReferenceMode,
        ReferenceOffsetH,
        ReferenceOffsetV,
        EnableInput,
        DisableInput,
        Fail            ///< Wrong command
    } Command;

    typedef enum
    {
        FreeRun,
        AnalogBlackBurst,
        AnalogTriLevel,
        DigitalInput1,
        DigitalInput2,
        FailMode            ///< Wrong ReferenceMode
    } VideoReferenceMode;
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;
public:
    Command                     VideoCommand;
    int                         Number;
    float                       Value;
    VideoReferenceMode          Mode;
public:

    explicit                        VideoCardEvent   () {};


    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};

template<> const std::wstring& SerializationHelper::T2WString   ( VideoCardEvent::Command t );

DEFINE_PTR_TYPE( VideoCardEvent )

// ************************************* RenderingModeEvent *************************************
class RenderingModeEvent : public BaseEvent
{
public:
    typedef enum
    {
        RenderOffscreen,
        ScreenShot,
        Fail            ///< Wrong command
    } Command;
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;
public:
    float                       FPS;
    unsigned int                NumFrames;
    std::string                 FilePath;
    Command                     RenderingCommand;
public:

    explicit                        RenderingModeEvent   () {};

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};

DEFINE_PTR_TYPE( RenderingModeEvent )

// ************************************* HightmapEvent *************************************
class HightmapEvent : public BaseEvent
{
public:
    typedef enum
    {
        Enable,
        Start,
        Reset,
        Show,
        Zoom,
        ZoomOut,
        Set,
        Anim,
        Anim2,
        Set3,
        Fail            ///< Wrong command
    } Command;
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;
public:
    Command                     HightmapCommand;
    float                       Hours;
    float                       Minutes;
    float                       Seconds;
    float                       Milliseconds;
public:
    explicit                        HightmapEvent   () {};

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};

DEFINE_PTR_TYPE( HightmapEvent )

} //bv
