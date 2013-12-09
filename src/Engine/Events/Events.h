#pragma once

#include "Engine/Events/BaseEvent.h"


namespace bv
{

namespace model
{
    class IPlugin;
    class IModelNode;
}

class Renderer;

// ************************************* PluginAddedEvent *************************************
class PluginAddedEvent : public BaseEvent
{
private:

    const model::IPlugin *      m_addedPlugin;

public:

    static const EventType      m_sEventType;
    static std::string          m_sEventName;

public:

    explicit                        PluginAddedEvent();
    explicit                        PluginAddedEvent( const model::IPlugin * plugin );

    virtual EventType               GetEventType    () const;

    virtual void                    Serialize       ( std::ostringstream & out ) const;
    virtual void                    Deserialize     ( std::istringstream & in );

    virtual IEventPtr               Clone           () const;

    virtual const std::string &     GetName         () const;

    const model::IPlugin *          GetPlugin       () const;

    static EventType                Type            ();

};

typedef std::shared_ptr<PluginAddedEvent> PluginAddEventPtr;


// ************************************* NodeAddedEvent *************************************
class NodeAddedEvent : public BaseEvent
{
private:

    const model::IModelNode *   m_addedNode;

public:

    static const EventType      m_sEventType;
    static std::string          m_sEventName;

public:

    explicit                        NodeAddedEvent  ();
    explicit                        NodeAddedEvent  ( const model::IModelNode * node );

    virtual EventType               GetEventType    () const;

    virtual void                    Serialize       ( std::ostringstream & out ) const;
    virtual void                    Deserialize     ( std::istringstream & in );

    virtual IEventPtr               Clone           () const;

    virtual const std::string &     GetName         () const;

    const model::IModelNode *       GetNode         () const;

    static EventType                Type            ();

};

typedef std::shared_ptr<NodeAddedEvent> NodeAddedEventPtr;


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

typedef std::shared_ptr<FrameRenderedEvent> FrameRenderedEventPtr;


} //bv
