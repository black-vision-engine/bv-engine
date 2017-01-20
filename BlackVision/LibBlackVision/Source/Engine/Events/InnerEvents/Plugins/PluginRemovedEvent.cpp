#include "stdafx.h"
#include "PluginRemovedEvent.h"




namespace bv
{

const EventType     PluginRemovedEvent::m_sEventType = EventManager::RegisterType();
std::string         PluginRemovedEvent::m_sEventName = "PluginRemovedEvent";


//******************* PluginRemovedEvent *************

// *************************************
//
void                PluginRemovedEvent::Serialize            ( ISerializer & /*ser*/ ) const
{
    assert( !"" );
}

// *************************************
//
IEventPtr           PluginRemovedEvent::Create          ( IDeserializer & /*deser*/ )
{
    assert( !"" );
    return nullptr;
}
// *************************************
//
IEventPtr           PluginRemovedEvent::Clone             () const
{
    return IEventPtr( new PluginRemovedEvent( *this ) );
}

// *************************************
//
EventType           PluginRemovedEvent::Type()
{
    return m_sEventType;
}
// *************************************
//
std::string&        PluginRemovedEvent::Name()
{
    return m_sEventName;
}
// *************************************
//
const std::string&  PluginRemovedEvent::GetName() const
{
    return Name();
}
// *************************************
//
EventType           PluginRemovedEvent::GetEventType() const
{
    return this->m_sEventType;
}


}   // bv
