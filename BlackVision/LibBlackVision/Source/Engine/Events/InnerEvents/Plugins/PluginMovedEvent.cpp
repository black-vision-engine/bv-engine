#include "stdafx.h"
#include "PluginMovedEvent.h"




namespace bv
{

const EventType     PluginMovedEvent::m_sEventType = EventManager::RegisterType();
std::string         PluginMovedEvent::m_sEventName = "PluginMovedEvent";


//******************* PluginMovedEvent *************

// *************************************
//
void                PluginMovedEvent::Serialize            ( ISerializer & /*ser*/ ) const
{
    assert( !"" );
}

// *************************************
//
IEventPtr           PluginMovedEvent::Create          ( IDeserializer & /*deser*/ )
{
    assert( !"" );
    return nullptr;
}
// *************************************
//
IEventPtr           PluginMovedEvent::Clone             () const
{
    return IEventPtr( new PluginMovedEvent( *this ) );
}

// *************************************
//
EventType           PluginMovedEvent::Type()
{
    return m_sEventType;
}
// *************************************
//
std::string&        PluginMovedEvent::Name()
{
    return m_sEventName;
}
// *************************************
//
const std::string&  PluginMovedEvent::GetName() const
{
    return Name();
}
// *************************************
//
EventType           PluginMovedEvent::GetEventType() const
{
    return this->m_sEventType;
}


}   // bv
