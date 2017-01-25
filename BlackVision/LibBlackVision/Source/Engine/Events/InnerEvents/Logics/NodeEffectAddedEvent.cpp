#include "stdafx.h"
#include "NodeEffectAddedEvent.h"




namespace bv
{

const EventType     NodeEffectAddedEvent::m_sEventType = EventManager::RegisterType();
std::string         NodeEffectAddedEvent::m_sEventName = "NodeEffectAddedEvent";


//******************* NodeEffectAddedEvent *************

// *************************************
//
void                NodeEffectAddedEvent::Serialize            ( ISerializer & /*ser*/ ) const
{
    assert( !"" );
}

// *************************************
//
IEventPtr           NodeEffectAddedEvent::Create          ( IDeserializer & /*deser*/ )
{
    assert( !"" );
    return nullptr;
}
// *************************************
//
IEventPtr           NodeEffectAddedEvent::Clone             () const
{
    return IEventPtr( new NodeEffectAddedEvent( *this ) );
}

// *************************************
//
EventType           NodeEffectAddedEvent::Type()
{
    return m_sEventType;
}
// *************************************
//
std::string&        NodeEffectAddedEvent::Name()
{
    return m_sEventName;
}
// *************************************
//
const std::string&  NodeEffectAddedEvent::GetName() const
{
    return Name();
}
// *************************************
//
EventType           NodeEffectAddedEvent::GetEventType() const
{
    return this->m_sEventType;
}


}   // bv
