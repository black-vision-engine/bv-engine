#pragma once

#include "Engine/Events/BaseEvent.h"
#include "Engine/Models/Plugins/Interfaces/IPlugin.h"
#include "Engine/Models/Interfaces/IModelNode.h"
#include "Serialization/SerializationHelper.inl"

#include  "Mathematics/glm_inc.h"


namespace bv
{

namespace model
{
    class IModelNode;
}

const static unsigned int SEND_BROADCAST_EVENT = UINT_MAX;

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


template<> LoadAssetEvent::Command  SerializationHelper::String2T      ( const std::string& s, const LoadAssetEvent::Command& defaultVal );
template<> std::string              SerializationHelper::T2String      ( const LoadAssetEvent::Command & t );

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
        Fail            ///< Wrong command
    } Command;

    typedef enum
    {
        PluginParam,
        GlobalEffectParam,
        ResourceParam,
        FailTarget      ///< Wrong target type
    } TargetType;
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;
public:
    std::string                     NodeName;
    std::string                     PluginName;
    std::string                     ParamName;
    std::string                     ParamSubName;
    std::string                     SceneName;
    std::string                     Value;

    float                           Time;
    ParamKeyEvent::Command          ParamCommand;
    ParamKeyEvent::TargetType       ParamTargetType;

public:
    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;


    virtual EventType               GetEventType        () const;
    virtual const std::string &     GetName             () const;

    static EventType                Type                ();
    static std::string&             Name                ();
};

template<> ParamKeyEvent::Command       SerializationHelper::String2T      ( const std::string& s, const ParamKeyEvent::Command& defaultVal );
template<> std::string                  SerializationHelper::T2String      ( const ParamKeyEvent::Command & t );

template<> ParamKeyEvent::TargetType    SerializationHelper::String2T      ( const std::string& s, const ParamKeyEvent::TargetType& defaultVal );
template<> std::string                  SerializationHelper::T2String      ( const ParamKeyEvent::TargetType & t );

DEFINE_PTR_TYPE( ParamKeyEvent )


// ************************************* AssetEvent *************************************
class AssetEvent : public RemoteEvent
{

public:

    typedef enum
    {
        ClearUnusedCachedAssets,
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

template<> AssetEvent::Command      SerializationHelper::String2T   ( const std::string& s, const AssetEvent::Command& defaultVal );
template<> std::string              SerializationHelper::T2String   ( const AssetEvent::Command & t );

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

template<> SceneEvent::Command      SerializationHelper::String2T   ( const std::string& s, const SceneEvent::Command& defaultVal );
template<> std::string              SerializationHelper::T2String   ( const SceneEvent::Command & t );

DEFINE_PTR_TYPE( SceneEvent )


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
    UInt32							AttachIndex;
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

template<> NodeStructureEvent::Command      SerializationHelper::String2T      ( const std::string& s, const NodeStructureEvent::Command& defaultVal );
template<> std::string                      SerializationHelper::T2String      ( const NodeStructureEvent::Command & t );

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
    IDeserializer * 				Request;

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

template<> PluginStructureEvent::Command    SerializationHelper::String2T( const std::string& s, const PluginStructureEvent::Command& defaultVal );
template<> std::string                      SerializationHelper::T2String( const PluginStructureEvent::Command & t );

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

template<> ProjectEvent::Command    SerializationHelper::String2T  ( const std::string& s, const ProjectEvent::Command& defaultVal );
template<> std::string              SerializationHelper::T2String  ( const ProjectEvent::Command & t );

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
        MinimalSceneInfo,

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

template<> InfoEvent::Command       SerializationHelper::String2T       ( const std::string& s, const InfoEvent::Command& defaultVal );
template<> std::string              SerializationHelper::T2String       ( const InfoEvent::Command & t );

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
    TimelineWrapMethod			WrapMethod;

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

template<> TimeLineEvent::Command   SerializationHelper::String2T  ( const std::string& s, const TimeLineEvent::Command& defaultVal );
template<> std::string              SerializationHelper::T2String  ( const TimeLineEvent::Command & t );

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

template<> TimerEvent::Command      SerializationHelper::String2T    ( const std::string& s, const TimerEvent::Command& defaultVal );
template<> std::string              SerializationHelper::T2String    ( const TimerEvent::Command & t );

DEFINE_PTR_TYPE( TimerEvent )


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
    explicit                        NodeLogicEvent			(){ Action = nullptr; }
                                    ~NodeLogicEvent			(){ delete Action; }

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};

template<> NodeLogicEvent::Command      SerializationHelper::String2T      ( const std::string& s, const NodeLogicEvent::Command& defaultVal );
template<> std::string                  SerializationHelper::T2String      ( const NodeLogicEvent::Command & t );

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

    //IDeserializer *             Request;

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

template<> EngineStateEvent::Command    SerializationHelper::String2T  ( const std::string& s, const EngineStateEvent::Command& defaultVal );
template<> std::string                  SerializationHelper::T2String  ( const EngineStateEvent::Command & t );

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

template<> GlobalEffectEvent::Command   SerializationHelper::String2T  ( const std::string& s, const GlobalEffectEvent::Command& defaultVal );
template<> std::string                  SerializationHelper::T2String  ( const GlobalEffectEvent::Command & t );

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

template<> TimelineKeyframeEvent::Command   SerializationHelper::String2T  ( const std::string& s, const TimelineKeyframeEvent::Command& defaultVal );
template<> std::string                      SerializationHelper::T2String  ( const TimelineKeyframeEvent::Command & t );

template<> TimelineKeyframeEvent::KeyframeType   SerializationHelper::String2T   ( const std::string& s, const TimelineKeyframeEvent::KeyframeType& defaultVal );
template<> std::string                           SerializationHelper::T2String   ( const TimelineKeyframeEvent::KeyframeType & t );

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

template<> MouseEvent::Command          SerializationHelper::String2T  ( const std::string& s, const MouseEvent::Command& defaultVal );
template<> std::string                  SerializationHelper::T2String  ( const MouseEvent::Command & t );

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

template<> SceneVariableEvent::Command      SerializationHelper::String2T  ( const std::string& s, const SceneVariableEvent::Command& defaultVal );
template<> std::string                      SerializationHelper::T2String  ( const SceneVariableEvent::Command & t );

DEFINE_PTR_TYPE( SceneVariableEvent )



// ************************************* EngineStateEvent *************************************
class ConfigEvent : public RemoteEvent
{
public:
    typedef enum
    {
        ReadValue,
        SetValue,
        Fail            ///< Wrong command
    } Command;
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;
public:

    Command                 ConfigCommand;
    std::string             Key;
    std::string             ValueType;
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

template<> ConfigEvent::Command         SerializationHelper::String2T  ( const std::string& s, const ConfigEvent::Command& defaultVal );
template<> std::string                  SerializationHelper::T2String  ( const ConfigEvent::Command & t );

DEFINE_PTR_TYPE( ConfigEvent )



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

template<> HightmapEvent::Command       SerializationHelper::String2T      ( const std::string& s, const HightmapEvent::Command& defaultVal );
template<> std::string                  SerializationHelper::T2String      ( const HightmapEvent::Command & t );

DEFINE_PTR_TYPE( HightmapEvent )

} //bv
