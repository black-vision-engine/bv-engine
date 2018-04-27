#pragma once

#include "Engine/Events/BaseEvent.h"
#include "Engine/Models/Plugins/Interfaces/IPlugin.h"
#include "Engine/Models/Interfaces/IModelNode.h"
#include "Serialization/SerializationHelper.inl"

#include "VideoCardManagerUtils.h"

#include  "Mathematics/glm_inc.h"

#include "Engine/Events/ParamAddress.h"
#include "Engine/Models/Gizmos/GizmoAddress.h"


// Include events headers
#include "API/SceneEvent.h"
#include "API/ProjectEvent.h"
#include "API/InfoEvent.h"
#include "API/CameraEvent.h"
#include "API/LightEvent.h"
#include "API/NodeStructureEvent.h"
#include "API/NodeLogicEvent.h"
#include "API/PluginsStructureEvent.h"
#include "API/GlobalEffectEvent.h"
#include "API/EngineStateEvent.h"
#include "API/ParamKeyEvent.h"
#include "API/LoadAssetEvent.h"
#include "API/UndoRedoEvent.h"
#include "API/TimeLineEvent.h"
#include "API/TimelineKeyframeEvent.h"


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
        SetReferenceMode,
        SetReferenceOffsetH,
        SetReferenceOffsetV,
        EnableInput,
        DisableInput,
        GetReferenceSignalInfo,
        IsLocked,
        Fail            ///< Wrong command
    } Command;

private:

    static const EventType      m_sEventType;
    static std::string          m_sEventName;

public:

    Command                     VideoCommand;

    UInt32                      VideoCardID;
    UInt32                      ChannelID;

    int                         Number;
    float                       Value;
    videocards::ReferenceMode   Mode;

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

DECLARE_ENUM_SERIALIZATION( VideoCardEvent::Command )
DEFINE_PTR_TYPE( VideoCardEvent )



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
        RemoveGizmo,
        ListGizmos,

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
