#include "Events.h"

#include <cassert>


namespace bv 
{

// ************************************* Static Events' Definitions *************************************

const EventType PluginAddedEvent::m_sEventType      = 0x00000001;
std::string PluginAddedEvent::m_sEventName          = "Event_PluginAdded";

const EventType NodeAddedEvent::m_sEventType        = 0x00000002;
std::string NodeAddedEvent::m_sEventName            = "Event_NodeAdded";

const EventType FrameRenderedEvent::m_sEventType    = 0x00000003;
std::string FrameRenderedEvent::m_sEventName        = "Event_FrameRendered";

const EventType TransformSetEvent::m_sEventType     = 0x00000004;
std::string TransformSetEvent::m_sEventName         = "Event_TransformSet";

const EventType KeyPressedEvent::m_sEventType       = 0x00000005;
std::string KeyPressedEvent::m_sEventName           = "Event_KeyPressedEvent";

// FIXME: MORE EVENTS IN CrawlerEvents.cpp. VERY VERY BAD IDEA. WE NEED EVENTS REGISTER SYSTEM ASAP.


// ************************************* PluginAddedEvent *************************************

// *************************************
//
PluginAddedEvent::PluginAddedEvent                          ()
    : m_addedPlugin( nullptr )
{
}

// *************************************
//
PluginAddedEvent::PluginAddedEvent                          ( model::IPluginConstPtr plugin )
    : m_addedPlugin( plugin )
{
}

// *************************************
//
EventType               PluginAddedEvent::GetEventType      () const
{
    return m_sEventType;
}

// *************************************
//
void                    PluginAddedEvent::Serialize         ( std::ostringstream & out ) const
{
    std::string err = GetName() + " should not be serialized";
    assert( !err.c_str()  );
}

// *************************************
//
void                    PluginAddedEvent::Deserialize       ( std::istringstream & in )
{
    std::string err = GetName() + " should not be deserialized";
    assert( !err.c_str()  );
}

// *************************************
//
IEventPtr               PluginAddedEvent::Clone             () const
{
    return IEventPtr( new PluginAddedEvent( *this ) );
}

// *************************************
//
const std::string &     PluginAddedEvent::GetName           () const
{
    return m_sEventName;
}

// *************************************
//
model::IPluginConstPtr  PluginAddedEvent::GetPlugin         () const
{
    return m_addedPlugin;
}

// *************************************
//
EventType               PluginAddedEvent::Type              ()
{
    return m_sEventType;
}


// ************************************* NodeAddedEvent *************************************

// *************************************
//
NodeAddedEvent::NodeAddedEvent                              ()
    : m_addedNode( nullptr )
{
}

// *************************************
//
NodeAddedEvent::NodeAddedEvent                              ( model::IModelNodeConstPtr node )
    : m_addedNode( node )
{
}

// *************************************
//
EventType               NodeAddedEvent::GetEventType        () const
{
    return m_sEventType;
}

// *************************************
//
void                    NodeAddedEvent::Serialize           ( std::ostringstream & out ) const
{
    std::string err = GetName() + " should not be serialized";
    assert( !err.c_str()  );
}

// *************************************
//
void                    NodeAddedEvent::Deserialize         ( std::istringstream & in )
{
    std::string err = GetName() + " should not be deserialized";
    assert( !err.c_str()  );
}

// *************************************
//
IEventPtr               NodeAddedEvent::Clone               () const
{
    return IEventPtr( new NodeAddedEvent( *this ) );
}

// *************************************
//
const std::string &     NodeAddedEvent::GetName             () const
{
    return m_sEventName;
}

// *************************************
//
model::IModelNodeConstPtr  NodeAddedEvent::GetNode         () const
{
    return m_addedNode;
}

// *************************************
//
EventType                   NodeAddedEvent::Type            ()
{
    return m_sEventType;
}


// ************************************* FrameRenderedEvent *************************************

// *************************************
//
FrameRenderedEvent::FrameRenderedEvent                          ( bv::Renderer * renderer )
    : m_renderer( renderer )
{
}

// *************************************
//
EventType               FrameRenderedEvent::GetEventType        () const
{
    return m_sEventType;
}

// *************************************
//
void                    FrameRenderedEvent::Serialize           ( std::ostringstream & out ) const
{
    std::string err = GetName() + " should not be serialized";
    assert( !err.c_str()  );
}

// *************************************
//
void                    FrameRenderedEvent::Deserialize         ( std::istringstream & in )
{
    std::string err = GetName() + " should not be deserialized";
    assert( !err.c_str()  );
}

// *************************************
//
IEventPtr               FrameRenderedEvent::Clone               () const
{
    return IEventPtr( new FrameRenderedEvent( *this ) );
}

// *************************************
//
const std::string &     FrameRenderedEvent::GetName             () const
{
    return m_sEventName;
}

// *************************************
//
Renderer *              FrameRenderedEvent::Renderer            ()
{
    return m_renderer;
}

// *************************************
//
void                    FrameRenderedEvent::SetResolution       ( int w, int h )
{
    m_width = w;
    m_height = h;
}

// *************************************
//
int                     FrameRenderedEvent::Width               () const
{
    return m_width;
}

// *************************************
//
int                     FrameRenderedEvent::Height              () const
{
    return m_height;
}

// *************************************
//
EventType               FrameRenderedEvent::Type                ()
{
    return m_sEventType;
}


// ************************************* FrameRenderedEvent *************************************

// *************************************
//
TransformSetEvent::TransformSetEvent                        ()
    : m_translation( 0.f, 0.f, 0.f )
    , m_scale( 1.f, 1.f, 1.f )
{
}

// *************************************
//
EventType           TransformSetEvent::GetEventType         () const
{
    return m_sEventType;
}

// *************************************
//
void                TransformSetEvent::Serialize            ( std::ostringstream & out ) const
{
    assert( false );
}

// *************************************
//
void                TransformSetEvent::Deserialize          ( std::istringstream & in )
{
    assert( false );
}

// *************************************
//
IEventPtr           TransformSetEvent::Clone                () const
{
    return IEventPtr( new TransformSetEvent( *this ) );
}

// *************************************
//
const std::string & TransformSetEvent::GetName              () const
{
    return m_sEventName;
}

// *************************************
//
void                TransformSetEvent::SetTranslation       ( const glm::vec3 & translation )
{
    m_translation = translation;
}

// *************************************
//
void                TransformSetEvent::SetScale             ( const glm::vec3 & scale )
{
    m_scale = scale;
}

// *************************************
//
const glm::vec3 &   TransformSetEvent::Translation          () const
{
    return m_translation;
}

// *************************************
//
const glm::vec3 &   TransformSetEvent::Scale                () const
{
    return m_scale;
}

// *************************************
//
EventType           TransformSetEvent::Type                 ()
{
    return m_sEventType;
}

// *************************************
//
KeyPressedEvent::KeyPressedEvent        ()
{}

// *************************************
//
EventType               KeyPressedEvent::GetEventType      () const
{
    return m_sEventType;
}

// *************************************
//
IEventPtr               KeyPressedEvent::Clone             () const
{
    return IEventPtr( new KeyPressedEvent( *this ) );
}

// *************************************
//
const std::string &     KeyPressedEvent::GetName           () const
{
    return m_sEventName;
}

// *************************************
//
void                    KeyPressedEvent::SetChar             ( unsigned char  c )
{
    m_char = c;
}

// *************************************
//
unsigned char           KeyPressedEvent::GetChar             () const
{
    return m_char;
}

// *************************************
//
EventType               KeyPressedEvent::Type                ()
{
    return m_sEventType;
}


} //bv
