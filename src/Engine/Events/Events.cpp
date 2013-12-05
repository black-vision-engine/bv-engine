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


// ************************************* PluginAddedEvent *************************************

// *************************************
//
PluginAddedEvent::PluginAddedEvent                          ()
    : m_addedPlugin( nullptr )
{
}

// *************************************
//
PluginAddedEvent::PluginAddedEvent                          ( const model::IPlugin * plugin )
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
const model::IPlugin *  PluginAddedEvent::GetPlugin         () const
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
NodeAddedEvent::NodeAddedEvent                              ( const model::IModelNode * node )
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
const model::IModelNode *   NodeAddedEvent::GetNode         () const
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
FrameRenderedEvent::FrameRenderedEvent                          ()
    : m_frameData( nullptr )
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
void                    FrameRenderedEvent::SetFrameDataPtr     ( const char * data, int w, int h )
{
    m_width = w;
    m_height = h;
    m_frameData = data;
}

// *************************************
//
const char *            FrameRenderedEvent::FrameDataPtr        () const
{
    return m_frameData;
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

}
