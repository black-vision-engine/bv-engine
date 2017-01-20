#include "stdafx.h"
#include "PluginAddedEvent.h"




namespace bv
{

const EventType     PluginAddedEvent::m_sEventType = EventManager::RegisterType();
std::string         PluginAddedEvent::m_sEventName = "PluginAddedEvent";


//******************* PluginAddedEvent *************

// *************************************
//
void                PluginAddedEvent::Serialize            ( ISerializer & /*ser*/ ) const
{
    assert( !"" );
}

// *************************************
//
IEventPtr           PluginAddedEvent::Create          ( IDeserializer & /*deser*/ )
{
    assert( !"" );
    return nullptr;
}
// *************************************
//
IEventPtr           PluginAddedEvent::Clone             () const
{
    return IEventPtr( new PluginAddedEvent( *this ) );
}

// *************************************
//
EventType           PluginAddedEvent::Type()
{
    return m_sEventType;
}
// *************************************
//
std::string&        PluginAddedEvent::Name()
{
    return m_sEventName;
}
// *************************************
//
const std::string&  PluginAddedEvent::GetName() const
{
    return Name();
}
// *************************************
//
EventType           PluginAddedEvent::GetEventType() const
{
    return this->m_sEventType;
}


}   // bv
