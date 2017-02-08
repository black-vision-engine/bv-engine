#include "stdafx.h"
#include "NodeEffectRemovedEvent.h"




namespace bv
{

const EventType     NodeEffectRemovedEvent::m_sEventType = EventManager::RegisterType();
std::string         NodeEffectRemovedEvent::m_sEventName = "NodeEffectRemovedEvent";


//******************* NodeEffectRemovedEvent *************

// *************************************
//
void                NodeEffectRemovedEvent::Serialize            ( ISerializer & /*ser*/ ) const
{
    assert( !"" );
}

// *************************************
//
IEventPtr           NodeEffectRemovedEvent::Create          ( IDeserializer & /*deser*/ )
{
    assert( !"" );
    return nullptr;
}
// *************************************
//
IEventPtr           NodeEffectRemovedEvent::Clone             () const
{
    return IEventPtr( new NodeEffectRemovedEvent( *this ) );
}

// *************************************
//
EventType           NodeEffectRemovedEvent::Type()
{
    return m_sEventType;
}
// *************************************
//
std::string&        NodeEffectRemovedEvent::Name()
{
    return m_sEventName;
}
// *************************************
//
const std::string&  NodeEffectRemovedEvent::GetName() const
{
    return Name();
}
// *************************************
//
EventType           NodeEffectRemovedEvent::GetEventType() const
{
    return this->m_sEventType;
}


}   // bv
