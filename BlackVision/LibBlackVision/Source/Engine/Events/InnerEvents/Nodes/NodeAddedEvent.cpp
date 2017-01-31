#include "stdafx.h"
#include "NodeAddedEvent.h"





namespace bv
{

const EventType     NodeAddedEvent::m_sEventType = EventManager::RegisterType();
std::string         NodeAddedEvent::m_sEventName = "NodeAddedEvent";


//******************* NodeRemovedEvent *************

// *************************************
//
void                NodeAddedEvent::Serialize            ( ISerializer & /*ser*/ ) const
{
    assert( !"" );
}

// *************************************
//
IEventPtr           NodeAddedEvent::Create          ( IDeserializer & /*deser*/ )
{
    assert( !"" );
    return nullptr;
}
// *************************************
//
IEventPtr           NodeAddedEvent::Clone             () const
{
    return IEventPtr( new NodeAddedEvent( *this ) );
}

// *************************************
//
EventType           NodeAddedEvent::Type()
{
    return m_sEventType;
}
// *************************************
//
std::string&        NodeAddedEvent::Name()
{
    return m_sEventName;
}
// *************************************
//
const std::string&  NodeAddedEvent::GetName() const
{
    return Name();
}
// *************************************
//
EventType           NodeAddedEvent::GetEventType() const
{
    return this->m_sEventType;
}


}   // bv
