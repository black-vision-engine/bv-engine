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





	// ciach

	// ************************************* VideoCardEvent *************************************
class VideoCardEvent : public BaseEvent
{
private:

    glm::vec3   m_translation;
    glm::vec3   m_scale;

   

public:

    static const EventType      m_sEventType;
    static std::string          m_sEventName;

     std::wstring               command;      // move to private



public:

    explicit                        VideoCardEvent   ();

    virtual EventType               GetEventType        () const;

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;
    static EventType                Type                ();

    virtual const std::string &     GetName             () const;
    void                            SetData(std::wstring cmd){command=cmd;};

};

DEFINE_PTR_TYPE(VideoCardEvent)


// ************************************* Response Event *************************************
class ResponseEvent : public BaseEvent
{
private:

    glm::vec3   m_translation;
    glm::vec3   m_scale;

   

public:

    static const EventType      m_sEventType;
    static std::string          m_sEventName;

     std::wstring               command;      // move to private
     std::wstring               response;

public:

    explicit                        ResponseEvent   ();

    virtual EventType               GetEventType        () const;

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;
    static EventType                Type                ();

    virtual const std::string &     GetName             () const;
    void                            SetData(std::wstring cmd){command=cmd;};

};


DEFINE_PTR_TYPE(ResponseEvent)


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

	// ************************************* Information Event *************************************
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

DEFINE_PTR_TYPE(SceneStructureEvent)


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

    virtual EventType               GetEventType        () const;

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;
    static EventType                Type                ();

    virtual const std::string &     GetName             () const;
    //void                            SetData(std::wstring cmd){command=cmd;};

};

DEFINE_PTR_TYPE(WidgetCmd)


// ************************************* LoadAssetEvent *************************************
class LoadAssetEvent : public BaseEvent
{
public:

    static const EventType      m_sEventType;
    static std::string          m_sEventName;
    std::string                 NodeName;
    std::string                 PluginName;
    std::string                 AssetData;

    int                         SockID;
public:
    explicit                        LoadAssetEvent();

    virtual EventType               GetEventType        () const;

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;
    static EventType                Type                ();

    virtual const std::string &     GetName             () const;
};


DEFINE_PTR_TYPE(LoadAssetEvent)


// ******************************************************************************************
// ************************************* new Events *****************************************
// ******************************************************************************************


// ************************************* ParamKeyEvent *************************************
class ParamKeyEvent : public BaseEvent
{
public:
    typedef enum
    {
        AddKey,
        UpdateKey,
        RemoveKey
    } Command;
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;
public:
    std::string                 NodeName;
    std::string                 PluginName;
    std::string                 ParamName;
    std::wstring                Value;

    float                       Time;
    ParamKeyEvent::Command      KeyCommand;

    virtual EventType               GetEventType        () const;
    virtual const std::string &     GetName             () const;

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;
};
DEFINE_PTR_TYPE( ParamKeyEvent )


// ************************************* SetParamEvent *************************************
//class SetParamEvent : public BaseEvent
//{
//private:
//
//    glm::vec3   m_translation;
//    glm::vec3   m_scale;
//
//   
//
//public:
//
//    static const EventType      m_sEventType;
//    static std::string          m_sEventName;
//
//    std::wstring               command;      // move to private
//
//    std::wstring               NodeName;
//    std::wstring               PluginName;
//    std::wstring               ParamName;
//    std::wstring               Value;
//    float                       x;
//    float                       y;
//    float                       z;
//	float                      time;
//
//
//public:
//
//    explicit                        SetParamEvent   ();
//
//    virtual EventType               GetEventType        () const;
//
//    virtual void                    Serialize           ( ISerializer& ser ) const;
//    virtual void                    Deserialize         ( std::istringstream & in );
//    virtual IEventPtr               Clone               () const;
//    static EventType                Type                ();
//
//    virtual const std::string &     GetName             () const;
//    void                            SetData(std::wstring cmd){command=cmd;};
//
//};
//
//DEFINE_PTR_TYPE(SetParamEvent)



} //bv
