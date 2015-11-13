#pragma once

#include "Engine/Events/BaseEvent.h"
#include "Engine/Models/Plugins/Interfaces/IPlugin.h"
#include "Engine/Models/Interfaces/IModelNode.h"

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

    model::IPluginConstPtr      m_addedPlugin;

public:

    static const EventType      m_sEventType;
    static std::string          m_sEventName;

public:

    explicit                        PluginAddedEvent();
    explicit                        PluginAddedEvent( model::IPluginConstPtr plugin );

    virtual EventType               GetEventType    () const;

    virtual void                    Serialize       ( ISerializer& ser ) const;
    static IEventPtr                Create          ( IDeserializer& deser );

    virtual IEventPtr               Clone           () const;

    virtual const std::string &     GetName         () const;

    model::IPluginConstPtr          GetPlugin       () const;

    static EventType                Type            ();

};

DEFINE_PTR_TYPE(PluginAddedEvent)

// ************************************* NodeAddedEvent *************************************
class NodeAddedEvent;
DEFINE_PTR_TYPE(NodeAddedEvent)
DEFINE_CONST_PTR_TYPE(NodeAddedEvent)

class NodeAddedEvent : public BaseEvent
{
private:

    model::IModelNodeConstPtr   m_addedNode;

public:

    static const EventType      m_sEventType;
    static std::string          m_sEventName;

public:

    explicit                        NodeAddedEvent  ();
    explicit                        NodeAddedEvent  ( model::IModelNodeConstPtr node );

    virtual EventType               GetEventType    () const;

    virtual void                    Serialize       ( ISerializer& ser ) const;
    static IEventPtr                Create          ( IDeserializer& deser );

    virtual IEventPtr               Clone           () const;

    virtual const std::string &     GetName         () const;

    model::IModelNodeConstPtr       GetNode         () const;

    static EventType                Type            ();

};


// ************************************* FrameRenderedEvent *************************************
class FrameRenderedEvent : public BaseEvent
{
private:

    Renderer *          m_renderer;

    int                 m_width;
    int                 m_height;

public:

    static const EventType      m_sEventType;
    static std::string          m_sEventName;

public:

    explicit                        FrameRenderedEvent  ( Renderer * renderer );

    virtual EventType               GetEventType        () const;

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );

    virtual IEventPtr               Clone               () const;

    virtual const std::string &     GetName             () const;

    Renderer *                      Renderer            ();

    void                            SetResolution       ( int w, int h );

    int                             Width               () const;
    int                             Height              () const;

    static EventType                Type                ();

};

DEFINE_PTR_TYPE(FrameRenderedEvent)

// ************************************* TransformSetEvent *************************************
class TransformSetEvent : public BaseEvent
{
private:

    glm::vec3   m_translation;
    glm::vec3   m_scale;

public:

    static const EventType      m_sEventType;
    static std::string          m_sEventName;

public:

    explicit                        TransformSetEvent   ();

    virtual EventType               GetEventType        () const;

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );

    virtual IEventPtr               Clone               () const;

    virtual const std::string &     GetName             () const;

    void                            SetTranslation      ( const glm::vec3 & translation );
    void                            SetScale            ( const glm::vec3 & scale );

    const glm::vec3 &               Translation         () const;
    const glm::vec3 &               Scale               () const;

    static EventType                Type                ();

};

DEFINE_PTR_TYPE(TransformSetEvent)

// ***************************** SET TEXT EVENT *************************
class KeyPressedEvent : public BaseEvent
{
    char        m_char;

public:

    explicit                        KeyPressedEvent     ();

    virtual EventType               GetEventType        () const;

    virtual IEventPtr               Clone               () const;

    virtual const std::string &     GetName             () const;

    void                            SetChar             ( unsigned char c );
    unsigned char                   GetChar             () const;

    static EventType                Type                ();

    static const EventType          m_sEventType;
    static std::string              m_sEventName;
};

DEFINE_PTR_TYPE(KeyPressedEvent)



// ************************************* Information Event *************************************
class InfoEvent : public BaseEvent
{
private:

    glm::vec3   m_translation;
    glm::vec3   m_scale;

public:

    static const EventType      m_sEventType;
    static std::string          m_sEventName;

     std::wstring               command;      // move to private
     std::wstring               request;
	 std::wstring				NodeName;
     int                        sock_id;

     std::wstring                m_additionalStrData;  // FIXME: temporary

public:

    explicit                        InfoEvent   ();

    virtual EventType               GetEventType        () const;

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;
    static EventType                Type                ();

    virtual bool                    ForceSync           () const;

    virtual const std::string &     GetName             () const;
    void                            SetData             ( std::wstring cmd ){ command=cmd; };

    void                            SetAddStrData       ( const std::wstring & data ) { m_additionalStrData = data; }
    const std::wstring &             GetAddStrData       () const;

};

DEFINE_PTR_TYPE(InfoEvent)


// ************************************* SceneStructureEvent Event *************************************
class SceneStructureEvent : public BaseEvent
{
private:

    glm::vec3   m_translation;
    glm::vec3   m_scale;

public:

    static const EventType      m_sEventType;
    static std::string          m_sEventName;

     std::wstring               command;      // move to private
     std::wstring               request;
	 std::wstring				NodeName;
	 std::wstring				NodeName2;
     int                        sock_id;

     std::wstring                m_additionalStrData;  // FIXME: temporary

public:

    explicit                        SceneStructureEvent   ();

    virtual EventType               GetEventType        () const;

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;
    static EventType                Type                ();

    virtual bool                    ForceSync           () const;

    virtual const std::string &     GetName             () const;
    void                            SetData             ( std::wstring cmd ){ command=cmd; };

    void                            SetAddStrData       ( const std::wstring & data ) { m_additionalStrData = data; }
    const std::wstring &             GetAddStrData       () const;

};

DEFINE_PTR_TYPE( SceneStructureEvent )


// ************************************* SetParamEvent *************************************
class SetParamEvent : public BaseEvent
{
private:

    glm::vec3   m_translation;
    glm::vec3   m_scale;

   

public:

    static const EventType      m_sEventType;
    static std::string          m_sEventName;

    std::wstring               command;      // move to private

    std::wstring               NodeName;
    std::wstring               PluginName;
    std::wstring               ParamName;
    std::wstring               Value;
    float                       x;
    float                       y;
    float                       z;
	float                      time;


public:

    explicit                        SetParamEvent   ();

    virtual EventType               GetEventType        () const;

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;
    static EventType                Type                ();

    virtual const std::string &     GetName             () const;
    void                            SetData(std::wstring cmd){command=cmd;};

};

DEFINE_PTR_TYPE(SetParamEvent)

// ************************************* TimelineCmd *************************************
class TimeLineCmd : public BaseEvent
{
private:

    glm::vec3   m_translation;
    glm::vec3   m_scale;

   

public:

    static const EventType      m_sEventType;
    static std::string          m_sEventName;
    float                       value;
    std::wstring                cmd;      // move to private

    std::wstring                TimelineName;
   

public:

    explicit                        TimeLineCmd   ();

    virtual EventType               GetEventType        () const;

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;
    static EventType                Type                ();

    virtual const std::string &     GetName             () const;
    //void                            SetData(std::wstring cmd){command=cmd;};

};

DEFINE_PTR_TYPE(TimeLineCmd)


// ************************************* TimerCmd *************************************
class TimerCmd : public BaseEvent
{
private:

    glm::vec3   m_translation;
    glm::vec3   m_scale;

   

public:

    static const EventType      m_sEventType;
    static std::string          m_sEventName;
    float                       value;
    std::wstring                cmd;      // move to private
    std::wstring                NodeName;
    float                       H;
    float                       M;
    float                       S;
    float                       MS;
   

public:

    explicit                        TimerCmd   ();

    virtual EventType               GetEventType        () const;

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;
    static EventType                Type                ();

    virtual const std::string &     GetName             () const;
    //void                            SetData(std::wstring cmd){command=cmd;};

};

DEFINE_PTR_TYPE(TimerCmd)

// ************************************* WidgetCmd *************************************
class WidgetCmd : public BaseEvent
{
private:

    glm::vec3   m_translation;
    glm::vec3   m_scale;

   

public:

    static const EventType      m_sEventType;
    static std::string          m_sEventName;
    std::wstring                Param;      // move to private
    std::wstring                NodeName;
	std::wstring                Action;
	float						Time;
	std::wstring                Value;
	std::wstring                WidgetName;
   

public:

    explicit                        WidgetCmd			();


    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};

DEFINE_PTR_TYPE(WidgetCmd)




// ******************************************************************************************
// ************************************* new Events *****************************************
// ******************************************************************************************


// ************************************* LoadAssetEvent *************************************
class LoadAssetEvent : public BaseEvent
{
private:
    static const EventType          m_sEventType;
    static std::string              m_sEventName;
public:
    std::string                     NodeName;
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
        Fail            ///< Wrong command
    } Command;
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;
public:
    std::string                     NodeName;
    std::string                     PluginName;
    std::string                     ParamName;
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
private:
    static std::wstring             CommandToWString    ( Command cmd );
    static Command                  WStringToCommand    ( const std::wstring& string );
};
DEFINE_PTR_TYPE( ParamKeyEvent )


// ************************************* SceneStructureEvent Event *************************************
class NodeStructureEvent : public BaseEvent
{
public:
    typedef enum
    {
        AddNode,
        RemoveNode,
        DetachPlugin,
        AttachPlugin,
        SetNodeVisible,
        SetNodeInvisible,
        Fail            ///< Wrong command
    } Command;
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;
public:
    NodeStructureEvent::Command     SceneCommand;
    std::string                     NodeName;
    std::string                     PluginName;
    std::string                     NewNodeName;
public:
    explicit                        NodeStructureEvent   () {}

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;

private:
    static std::wstring             CommandToWString    ( Command cmd );
    static Command                  WStringToCommand    ( const std::wstring& string );
};

DEFINE_PTR_TYPE( NodeStructureEvent )

// ************************************* ProjectEvent Event *************************************
class ProjectEvent : public BaseEvent
{
public:
    typedef enum
    {
        NewProject,
        SetCurrentProject,
        SaveScene,
        LoadProject,
        ListProjectNames,
        ListScenes,
        ListAssetsPaths,
        ListCategoriesNames,
        ListProjects,
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

private:
    static std::wstring             CommandToWString    ( Command cmd );
    static Command                  WStringToCommand    ( const std::wstring& string );
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
// Rename me later
class NewInfoEvent : public BaseEvent
{
public:
    typedef enum
    {
        TreeStructure,
        Performance,
        Timelines,
        NodeInfo,
        Videocards,
        Fail            ///< Wrong command
    } Command;
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;
public:
    NewInfoEvent::Command       InfoRequest;
    std::string                 NodeName;

public:
    explicit                        NewInfoEvent   () {}

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;

private:
    static std::wstring             CommandToWString    ( Command cmd );
    static Command                  WStringToCommand    ( const std::wstring& string );
};

DEFINE_PTR_TYPE( NewInfoEvent )


// ************************************* TimelineCmd *************************************
class TimeLineEvent : public BaseEvent
{
public:
    typedef enum
    {
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
    std::string                 TimelineName;
    float                       Time;

public:
    explicit                        TimeLineEvent   () {}

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
private:
    static std::wstring             CommandToWString    ( Command cmd );
    static Command                  WStringToCommand    ( const std::wstring& string );
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
private:
    static std::wstring             CommandToWString    ( Command cmd );
    static Command                  WStringToCommand    ( const std::wstring& string );
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
private:
    static std::wstring             CommandToWString    ( Command cmd );
    static Command                  WStringToCommand    ( const std::wstring& string );
};

DEFINE_PTR_TYPE( WidgetEvent )


// ciach

// ************************************* VideoCardEvent *************************************
class VideoCardEvent : public BaseEvent
{
public:
    typedef enum
    {
        VideoCardOn,
        VideoCardOff,
        KeyOn,
        KeyOff,
        Fail            ///< Wrong command
    } Command;
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;
public:
    Command                     VideoCommand;
    std::string                 Action;         // For future use.
public:

    explicit                        VideoCardEvent   () {};


    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
public:
    static std::wstring             CommandToWString    ( Command cmd );
    static Command                  WStringToCommand    ( const std::wstring& string );
};

DEFINE_PTR_TYPE( VideoCardEvent )

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
public:
    static std::wstring             CommandToWString    ( Command cmd );
    static Command                  WStringToCommand    ( const std::wstring& string );
};

DEFINE_PTR_TYPE( HightmapEvent )

} //bv
