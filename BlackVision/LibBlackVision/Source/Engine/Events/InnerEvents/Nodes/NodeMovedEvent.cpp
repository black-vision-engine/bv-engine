#include "stdafx.h"
#include "NodeMovedEvent.h"





namespace bv
{

const EventType     NodeMovedEvent::m_sEventType = EventManager::RegisterType();
std::string         NodeMovedEvent::m_sEventName = "NodeMovedEvent";


//******************* NodeRemovedEvent *************

// *************************************
//
void                NodeMovedEvent::Serialize            ( ISerializer & /*ser*/ ) const
{
    assert( !"" );
}

// *************************************
//
IEventPtr           NodeMovedEvent::Create          ( IDeserializer & /*deser*/ )
{
    assert( !"" );
    return nullptr;
}
// *************************************
//
IEventPtr           NodeMovedEvent::Clone             () const
{
    return IEventPtr( new NodeMovedEvent( *this ) );
}

// *************************************
//
EventType           NodeMovedEvent::Type()
{
    return m_sEventType;
}
// *************************************
//
std::string&        NodeMovedEvent::Name()
{
    return m_sEventName;
}
// *************************************
//
const std::string&  NodeMovedEvent::GetName() const
{
    return Name();
}
// *************************************
//
EventType           NodeMovedEvent::GetEventType() const
{
    return this->m_sEventType;
}


}   // bv
