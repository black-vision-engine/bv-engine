#pragma once

#include "Engine/Events/BaseEvent.h"
#include "Engine/Models/Plugins/Interfaces/IPlugin.h"
#include "Engine/Models/Interfaces/IModelNode.h"
#include "Serialization/SerializationHelper.inl"

#include  "Mathematics/glm_inc.h"

#include "Engine/Events/ParamAddress.h"
#include "Engine/Models/Gizmos/GizmoAddress.h"


namespace bv {


namespace model
{
    class IModelNode;
}

const static unsigned int SEND_BROADCAST_EVENT = UINT_MAX;

class Renderer;



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


// ***************************** ScreenShotRendered *************************
class ScreenShotRenderedEvent : public BaseEvent
{
private:
    static const EventType          m_sEventType;
    static std::string              m_sEventName;
    
public:

    explicit                        ScreenShotRenderedEvent     ();

    std::string                     FilePath;
    bool                            Result;


    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};

DEFINE_PTR_TYPE( ScreenShotRenderedEvent )

// ************************************* LoadAssetEvent *************************************
class LoadAssetEvent : public RemoteEvent
{
public:
    typedef enum
    {
        LoadAsset,
        Fail            ///< Wrong command
    } Command;
private:
    static const EventType          m_sEventType;
    static std::string              m_sEventName;
public:
    std::string                     NodeName;
    std::string                     SceneName;
    std::string                     PluginName;
    IDeserializer *                 AssetData;
    bool                            AsyncLoad;

public:
    explicit                        LoadAssetEvent      () { AssetData = nullptr; }
                                    ~LoadAssetEvent     () { delete AssetData; }

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};


DECLARE_ENUM_SERIALIZATION( LoadAssetEvent::Command )
DEFINE_PTR_TYPE( LoadAssetEvent )


// ************************************* ParamKeyEvent *************************************
class ParamKeyEvent : public RemoteEvent
{
public:
    typedef enum
    {
        AddKey,
        RemoveKey,
        MoveKey,
        SetInterpolatorType,
        SetAddedInterpolatorType,
        SetInterpolatorPreWrapMethod,
        SetInterpolatorPostWrapMethod,
        AssignTimeline,
        SampleCurve,
        ListKeys,
        Fail            ///< Wrong command
    } Command;


private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;
public:
    
    ParameterAddress                ParamAddress;
    std::string                     Value;

    float                           Time;
    ParamKeyEvent::Command          ParamCommand;

public:
    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;


    virtual EventType               GetEventType        () const;
    virtual const std::string &     GetName             () const;

    static EventType                Type                ();
    static std::string&             Name                ();
};

DECLARE_ENUM_SERIALIZATION( ParamKeyEvent::Command )
DEFINE_PTR_TYPE( ParamKeyEvent )


// ************************************* ParamDescriptorEvent *************************************
class ParamDescriptorEvent : public RemoteEvent
{
public:
    typedef enum
    {
        AddParamDescriptor,
        RemoveParamDescriptor,
        AddAvaibleKeyTimes,
        RemoveAvaibleKeyTimes,
        SetDescriptorParameters,
        Fail            ///< Wrong command
    } Command;


private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;
public:
    
    ParamDescriptorEvent::Command   ParamCommand;
    ParameterAddress                ParamAddress;

    IDeserializer *                 Request;

public:
    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;


    virtual EventType               GetEventType        () const;
    virtual const std::string &     GetName             () const;

    static EventType                Type                ();
    static std::string&             Name                ();
};

DECLARE_ENUM_SERIALIZATION( ParamDescriptorEvent::Command )
DEFINE_PTR_TYPE( ParamDescriptorEvent )

// ************************************* AssetEvent *************************************
class AssetEvent : public RemoteEvent
{

public:

    typedef enum
    {
        ClearCache,
        Fail            ///< Wrong command
    } Command;

private:

    static const EventType      m_sEventType;
    static std::string          m_sEventName;

public:

    AssetEvent::Command         AssetCommand;

public:
    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;


    virtual EventType               GetEventType        () const;
    virtual const std::string &     GetName             () const;

    static EventType                Type                ();
    static std::string&             Name                ();
};

DECLARE_ENUM_SERIALIZATION( AssetEvent::Command )
DEFINE_PTR_TYPE( AssetEvent )

// ************************************* SceneEvent Event *************************************
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


// ************************************* LightEvent *************************************
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
    LightEvent::Command                SceneCommand;
    std::string                     SceneName;
    std::string                     LightType;
    UInt32                            LightIndex;
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


// ************************************* CameraEvent *************************************
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
    //std::string                     TimelinePath;

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


// ************************************* SceneStructureEvent Event *************************************
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
    UInt32                            AttachIndex;
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


// ************************************* PluginStructureEvent Event *************************************
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


// ************************************* ProjectEvent Event *************************************
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


// ************************************* Response Event *************************************
class ResponseEvent : public RemoteEvent
{
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;
public:
     std::string                Response;
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
class InfoEvent : public RemoteEvent
{
public:
    typedef enum
    {
        TreeStructure,
        MinimalTreeStructure,
        NodeInfo,
        PluginInfo,
        EffectInfo,
        ParamInfo,
        LogicInfo,
        MinimalSceneInfo,
        LightsInfo,
        CamerasInfo,
        ListParamDescriptors,
        GetParamDescriptor,

        ListSceneAssets,
        ListProjectNames,
        ListScenes,
        ListPresets,
        ListAssetsPaths,
        ListCategoriesNames,
        ListProjects,
        ListAllScenes,
        ListAllFolders,
        ListResourcesInFolders,
        ListAllResources,
        GetAssetDescriptor,
        GetAssetThumbnail,
        GetSceneThumbnail,
        GetPresetThumbnail,
        GetPMItemStats,

        Timelines,
        CheckTimelineTime,
        ListTimelineKeyframes,
        ListTimelinesParams,

        Performance,
        Videocards,
        Fail            ///< Wrong command
    } Command;
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;
public:
    InfoEvent::Command          InfoCommand;
    IDeserializer *             Request;

public:
    explicit                        InfoEvent   ()  { Request = nullptr; }
                                    ~InfoEvent   () { delete Request; }

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};


DECLARE_ENUM_SERIALIZATION( InfoEvent::Command )
DEFINE_PTR_TYPE( InfoEvent )


// ************************************* TimelineCmd *************************************
class TimeLineEvent : public RemoteEvent
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
    std::string                 TimelineName; //path?
    std::string                 NewTimelineName;
    float                       Time;
    TimelineType                TimelineType;
    TimeType                    Duration;
    TimelineWrapMethod            WrapMethod;

public:

    explicit                        TimeLineEvent   () {}

    virtual void                    Serialize           ( ISerializer & ser ) const;
    static IEventPtr                Create              ( IDeserializer & deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string &            Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;

};


DECLARE_ENUM_SERIALIZATION( TimeLineEvent::Command )
DEFINE_PTR_TYPE( TimeLineEvent )


// ************************************* TimerEvent *************************************
class TimerEvent : public RemoteEvent
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
        SetTimePatern,
        Fail            ///< Wrong command
    } Command;
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;
public:
    std::string                 NodeName;
    std::string                 SceneName;
    std::string                 TimePatern;
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


DECLARE_ENUM_SERIALIZATION( TimerEvent::Command )
DEFINE_PTR_TYPE( TimerEvent )


// ************************************* VideoDecoderEvent *************************************
class VideoDecoderEvent : public RemoteEvent
{
public:

    typedef enum
    {
        HasFinished,
        Fail            ///< Wrong command
    } Command;

private:

    static const EventType      m_sEventType;
    static std::string          m_sEventName;

public:

    Command                     EventCommand;
    std::string                 AssetPath;

public:

    explicit                        VideoDecoderEvent          (){};

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string&      GetName             () const;
    virtual EventType               GetEventType        () const;
};


DECLARE_ENUM_SERIALIZATION( VideoDecoderEvent::Command )
DEFINE_PTR_TYPE( VideoDecoderEvent )


// ************************************* NodeLogicEvent *************************************
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
    explicit                        NodeLogicEvent            (){ Action = nullptr; }
                                    ~NodeLogicEvent            (){ delete Action; }

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


// ************************************* VideoCardEvent *************************************
class VideoCardEvent : public RemoteEvent
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

template<> std::string              SerializationHelper::T2String   ( const VideoCardEvent::Command & t );

DEFINE_PTR_TYPE( VideoCardEvent )

// ************************************* EngineStateEvent *************************************
class EngineStateEvent : public RemoteEvent
{
public:
    typedef enum
    {
        RenderOffscreen,
        ScreenShot,
        CloseApplication,
        LockEventQueue,
		SetGain,
        OutputCommand,
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
	float						Gain;

    IDeserializer *             Request;

public:

    explicit                        EngineStateEvent   () {};

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};


DECLARE_ENUM_SERIALIZATION( EngineStateEvent::Command )
DEFINE_PTR_TYPE( EngineStateEvent )


// ************************************* GlobalEffectEvent *************************************
class GlobalEffectEvent : public RemoteEvent
{
public:
    typedef enum
    {
        SetGlobalEffect,
        LoadGlobalEffectAsset,
        Fail            ///< Wrong command
    } Command;
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;

public:
    GlobalEffectEvent::Command      EffectCommand;
    std::string                     NodePath;
    std::string                     SceneName;
    std::string                     EffectName;
    std::string                     TimelinePath;
    IDeserializer *                 AssetData;
    IDeserializer *                 Request;

public:
    explicit                        GlobalEffectEvent   () {}

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};


DECLARE_ENUM_SERIALIZATION( GlobalEffectEvent::Command )
DEFINE_PTR_TYPE( GlobalEffectEvent )


// ************************************* TimelineKeyframeEvent *************************************
class TimelineKeyframeEvent : public RemoteEvent
{
public:
    typedef enum
    {
        AddKeyframe,
        RemoveKeyframe,
        Fail            ///< Wrong command
    } Command;

    typedef enum
    {
        StopKeyframe,
        LoopReverseKeyframe,
        LoopJumpKeyframe,
        LoopRestartKeyframe,
        TriggerEventKeyframe,
        NullKeyframe,
        KeyframeTypeFail
    } KeyframeType;
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;
public:
    TimelineKeyframeEvent::Command      KeyframeCommand;
    TimelineKeyframeEvent::KeyframeType NewKeyframeType;
    std::string                         TimelinePath;
    std::string                         KeyframeName;
    int                                 KeyframeIndex;

    float                               Time;
    float                               JumpToTime;
    unsigned int                        TotalLoopCount;
    std::string                         TriggerEvents;

public:
    explicit                        TimelineKeyframeEvent   () {}

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};


DECLARE_ENUM_SERIALIZATION( TimelineKeyframeEvent::Command )
DECLARE_ENUM_SERIALIZATION( TimelineKeyframeEvent::KeyframeType )
DEFINE_PTR_TYPE( TimelineKeyframeEvent )


// ************************************* MouseEvent *************************************
class MouseEvent : public RemoteEvent
{
public:
    typedef enum
    {
        MouseUp,
        MouseDown,
        MouseMove,
        Fail            ///< Wrong command
    } Command;
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;

public:
    MouseEvent::Command             MouseCommand;

    Float32                         MouseX;
    Float32                         MouseY;
    bool                            AutoSelect;
    glm::vec4                       AutoSelectColor;

public:
    explicit                        MouseEvent          () {}

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};


DECLARE_ENUM_SERIALIZATION( MouseEvent::Command )
DEFINE_PTR_TYPE( MouseEvent )


// ************************************* SceneVariableEvent *************************************
class SceneVariableEvent : public RemoteEvent
{
public:
    typedef enum
    {
        AddVariable,
        GetVariable,
        DeleteVariable,
        Fail            ///< Wrong command
    } Command;
private:

    static const EventType      m_sEventType;
    static std::string          m_sEventName;

public:

    Command                     VariableCommand;
    std::string                 SceneName;
    std::string                 VariableName;
    std::string                 VariableContent;

public:
    explicit                        SceneVariableEvent  (){}
                                    ~SceneVariableEvent (){}

    virtual void                    Serialize           ( ISerializer & ser ) const;
    static IEventPtr                Create              ( IDeserializer & deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string &            Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};


DECLARE_ENUM_SERIALIZATION( SceneVariableEvent::Command )
DEFINE_PTR_TYPE( SceneVariableEvent )



// ************************************* ConfigEvent *************************************
class ConfigEvent : public RemoteEvent
{
public:
    typedef enum
    {
        ReadValue,
        SetValue,
        SaveConfig,
        Fail            ///< Wrong command
    } Command;
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;
public:

    Command                 ConfigCommand;
    std::string             Key;
    std::string             Value;

public:

    explicit                        ConfigEvent         () {};

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};


DECLARE_ENUM_SERIALIZATION( ConfigEvent::Command )
DEFINE_PTR_TYPE( ConfigEvent )


// ************************************* GridLineEvent *************************************
class GridLineEvent : public RemoteEvent
{
public:
    typedef enum
    {
        SetGridLinePosition,
        RenameGridLine,
        RemoveGridLine,
        AlignToGridLine,
        ShowGridLines,
        HideGridLines,
        Fail            ///< Wrong command
    } Command;

private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;
public:

    Command                 GridLineCommand;
    std::string             SceneName;
    std::string             NodeName;
    std::string             GridLineName;
    std::string             GridLineType;
    std::string             AlignementType;
    float                   GridLinePosition;
    int                     GridLineIndex;

public:

    explicit                        GridLineEvent        () {};

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};


DECLARE_ENUM_SERIALIZATION( GridLineEvent::Command )
DEFINE_PTR_TYPE( GridLineEvent )



// ************************************* UndoRedoEvent *************************************
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


// ************************************* GenericEvent *************************************
class GenericEvent : public RemoteEvent
{
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;
public:

    std::string                     CommandName;
    IDeserializer *                 Request;

public:

    explicit                        GenericEvent        ()
    {};

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};

DEFINE_PTR_TYPE( GenericEvent )


// ************************************* HightmapEvent *************************************
class HightmapEvent : public RemoteEvent
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


DECLARE_ENUM_SERIALIZATION( HightmapEvent::Command )
DEFINE_PTR_TYPE( HightmapEvent )


// ***************************** AssetTrackerInternalEvent *************************
class AssetTrackerInternalEvent : public BaseEvent
{
public:
    typedef enum
    {
        RegisterAsset,
        UnregisterAsset,
        PlayAudio,
        PauseAudio,
        StopAudio,
        EOFAudio,
        ReleaseAudioResource,
        Fail            ///< Wrong command
    } Command;


private:

    static const EventType          m_sEventType;
    static std::string              m_sEventName;
    static uintptr_t                m_sDefaultUID;

public:

    explicit                        AssetTrackerInternalEvent   ( Command eventCommand );
    explicit                        AssetTrackerInternalEvent   ( Command eventCommand, uintptr_t assetUID );
    explicit                        AssetTrackerInternalEvent   ( Command eventCommand, std::string assetKey );

    Command                         EventCommand;
    const model::IPlugin *          PluginOwner;
    const SceneNode *               SceneNodeOwner;

    uintptr_t                       AssetUID;
    std::string                     AssetKey;

    virtual void                    Serialize           ( ISerializer & ser ) const;
    static IEventPtr                Create              ( IDeserializer & deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string &            Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;

    bool                            HasUID              () const;
    bool                            HasKey              () const;

};

DEFINE_PTR_TYPE( AssetTrackerInternalEvent )


// ************************************* GizmoEvent *************************************
class GizmoEvent : public RemoteEvent
{
public:

    typedef enum
    {
        CreateGizmo,
        Fail            ///< Wrong command
    } Command;

private:

    static const EventType      m_sEventType;
    static std::string          m_sEventName;

public:

    Command                     CommandName;
    GizmoAddress                Address;

public:
    explicit                        GizmoEvent     () {};

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};

DECLARE_ENUM_SERIALIZATION( GizmoEvent::Command )
DEFINE_PTR_TYPE( GizmoEvent );


} //bv
