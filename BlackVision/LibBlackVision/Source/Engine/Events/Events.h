#pragma once

#include "Engine/Events/BaseEvent.h"
#include "Engine/Models/Plugins/Interfaces/IPlugin.h"
#include "Engine/Models/Interfaces/IModelNode.h"

#include  "glm/glm.hpp"


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

    virtual void                    Serialize       ( std::ostringstream & out ) const;
    virtual void                    Deserialize     ( std::istringstream & in );

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

    virtual void                    Serialize       ( std::ostringstream & out ) const;
    virtual void                    Deserialize     ( std::istringstream & in );

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

    virtual void                    Serialize           ( std::ostringstream & out ) const;
    virtual void                    Deserialize         ( std::istringstream & in );

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

    virtual void                    Serialize           ( std::ostringstream & out ) const;
    virtual void                    Deserialize         ( std::istringstream & in );

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

} //bv