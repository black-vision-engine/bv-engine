#include "stdafx.h"
#include "NodeLogicRemovedEvent.h"




namespace bv
{

const EventType     NodeLogicRemovedEvent::m_sEventType = EventManager::RegisterType();
std::string         NodeLogicRemovedEvent::m_sEventName = "NodeLogicRemovedEvent";


//******************* NodeLogicRemovedEvent *************

// *************************************
//
void                NodeLogicRemovedEvent::Serialize            ( ISerializer & /*ser*/ ) const
{
    assert( !"" );
}

// *************************************
//
IEventPtr           NodeLogicRemovedEvent::Create          ( IDeserializer & /*deser*/ )
{
    assert( !"" );
    return nullptr;
}
// *************************************
//
IEventPtr           NodeLogicRemovedEvent::Clone             () const
{
    return IEventPtr( new NodeLogicRemovedEvent( *this ) );
}

// *************************************
//
EventType           NodeLogicRemovedEvent::Type()
{
    return m_sEventType;
}
// *************************************
//
std::string&        NodeLogicRemovedEvent::Name()
{
    return m_sEventName;
}
// *************************************
//
const std::string&  NodeLogicRemovedEvent::GetName() const
{
    return Name();
}
// *************************************
//
EventType           NodeLogicRemovedEvent::GetEventType() const
{
    return this->m_sEventType;
}


}   // bv
