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


template<> LoadAssetEvent::Command  SerializationHelper::WString2T      ( const std::wstring& s, const LoadAssetEvent::Command& defaultVal );
template<> const std::wstring&      SerializationHelper::T2WString      ( LoadAssetEvent::Command t );

DEFINE_PTR_TYPE( LoadAssetEvent )


// ************************************* ParamKeyEvent *************************************
class ParamKeyEvent : public RemoteEvent
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
        AssignTimeline,
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
    std::wstring                    Value;

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

template<> ParamKeyEvent::Command       SerializationHelper::WString2T      ( const std::wstring& s, const ParamKeyEvent::Command& defaultVal );
template<> const std::wstring&          SerializationHelper::T2WString      ( ParamKeyEvent::Command t );

template<> ParamKeyEvent::TargetType    SerializationHelper::WString2T      ( const std::wstring& s, const ParamKeyEvent::TargetType& defaultVal );
template<> const std::wstring&          SerializationHelper::T2WString      ( ParamKeyEvent::TargetType t );

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

template<> AssetEvent::Command SerializationHelper::WString2T   ( const std::wstring& s, const AssetEvent::Command& defaultVal );
template<> const std::wstring& SerializationHelper::T2WString   ( AssetEvent::Command t );

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

template<> SceneEvent::Command SerializationHelper::WString2T ( const std::wstring& s, const SceneEvent::Command& defaultVal );
template<> const std::wstring& SerializationHelper::T2WString    ( SceneEvent::Command t );

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

template<> NodeStructureEvent::Command      SerializationHelper::WString2T      ( const std::wstring& s, const NodeStructureEvent::Command& defaultVal );
template<> const std::wstring&              SerializationHelper::T2WString      ( NodeStructureEvent::Command t );

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

template<> PluginStructureEvent::Command    SerializationHelper::WString2T( const std::wstring& s, const PluginStructureEvent::Command& defaultVal );
template<> const std::wstring&              SerializationHelper::T2WString( PluginStructureEvent::Command t );

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

        AddExistingSceneToProject,
        CreateSceneInProject,

        SaveScene,
        LoadScene,
        RemoveScene,
        CopyScene,
        MoveScene,

        SavePreset,
        LoadPreset,

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

template<> ProjectEvent::Command    SerializationHelper::WString2T  ( const std::wstring& s, const ProjectEvent::Command& defaultVal );
template<> const std::wstring&      SerializationHelper::T2WString  ( ProjectEvent::Command t );

DEFINE_PTR_TYPE( ProjectEvent )


// ************************************* Response Event *************************************
class ResponseEvent : public RemoteEvent
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

        Timelines,
        CheckTimelineTime,
        ListTimelineKeyframes,

        Performance,
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

template<> InfoEvent::Command       SerializationHelper::WString2T       ( const std::wstring& s, const InfoEvent::Command& defaultVal );
template<> const std::wstring&      SerializationHelper::T2WString       ( InfoEvent::Command t );

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

template<> TimeLineEvent::Command   SerializationHelper::WString2T  ( const std::wstring& s, const TimeLineEvent::Command& defaultVal );
template<> const std::wstring &     SerializationHelper::T2WString  ( TimeLineEvent::Command t );

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

template<> TimerEvent::Command      SerializationHelper::WString2T    ( const std::wstring& s, const TimerEvent::Command& defaultVal );
template<> const std::wstring&      SerializationHelper::T2WString    ( TimerEvent::Command t );

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
	std::string                 Action;

public:
    explicit                        NodeLogicEvent			(){};

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};

template<> NodeLogicEvent::Command     SerializationHelper::WString2T      ( const std::wstring& s, const NodeLogicEvent::Command& defaultVal );
template<> const std::wstring&      SerializationHelper::T2WString      ( NodeLogicEvent::Command t );

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

template<> const std::wstring& SerializationHelper::T2WString   ( VideoCardEvent::Command t );

DEFINE_PTR_TYPE( VideoCardEvent )

// ************************************* RenderingModeEvent *************************************
class RenderingModeEvent : public RemoteEvent
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

template<> RenderingModeEvent::Command  SerializationHelper::WString2T  ( const std::wstring& s, const RenderingModeEvent::Command& defaultVal );
template<> const std::wstring&          SerializationHelper::T2WString  ( RenderingModeEvent::Command t );

DEFINE_PTR_TYPE( RenderingModeEvent )


// ************************************* ProjectEvent Event *************************************
class GlobalEffectEvent : public RemoteEvent
{
public:
    typedef enum
    {
        SetGlobalEffect,
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

template<> GlobalEffectEvent::Command   SerializationHelper::WString2T  ( const std::wstring& s, const GlobalEffectEvent::Command& defaultVal );
template<> const std::wstring&          SerializationHelper::T2WString  ( GlobalEffectEvent::Command t );

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

template<> TimelineKeyframeEvent::Command   SerializationHelper::WString2T  ( const std::wstring& s, const TimelineKeyframeEvent::Command& defaultVal );
template<> const std::wstring&              SerializationHelper::T2WString  ( TimelineKeyframeEvent::Command t );

template<> TimelineKeyframeEvent::KeyframeType  SerializationHelper::WString2T   ( const std::wstring& s, const TimelineKeyframeEvent::KeyframeType& defaultVal );
template<> const std::wstring&                  SerializationHelper::T2WString   ( TimelineKeyframeEvent::KeyframeType t );

DEFINE_PTR_TYPE( TimelineKeyframeEvent )


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

template<> HightmapEvent::Command       SerializationHelper::WString2T      ( const std::wstring& s, const HightmapEvent::Command& defaultVal );
template<> const std::wstring&          SerializationHelper::T2WString      ( HightmapEvent::Command t );

DEFINE_PTR_TYPE( HightmapEvent )

} //bv
