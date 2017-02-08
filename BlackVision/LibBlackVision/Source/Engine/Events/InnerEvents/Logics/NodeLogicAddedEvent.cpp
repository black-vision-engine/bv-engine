#include "stdafx.h"
#include "NodeLogicAddedEvent.h"




namespace bv
{

const EventType     NodeLogicAddedEvent::m_sEventType = EventManager::RegisterType();
std::string         NodeLogicAddedEvent::m_sEventName = "NodeLogicAddedEvent";


//******************* NodeLogicAddedEvent *************

// *************************************
//
void                NodeLogicAddedEvent::Serialize            ( ISerializer & /*ser*/ ) const
{
    assert( !"" );
}

// *************************************
//
IEventPtr           NodeLogicAddedEvent::Create          ( IDeserializer & /*deser*/ )
{
    assert( !"" );
    return nullptr;
}
// *************************************
//
IEventPtr           NodeLogicAddedEvent::Clone             () const
{
    return IEventPtr( new NodeLogicAddedEvent( *this ) );
}

// *************************************
//
EventType           NodeLogicAddedEvent::Type()
{
    return m_sEventType;
}
// *************************************
//
std::string&        NodeLogicAddedEvent::Name()
{
    return m_sEventName;
}
// *************************************
//
const std::string&  NodeLogicAddedEvent::GetName() const
{
    return Name();
}
// *************************************
//
EventType           NodeLogicAddedEvent::GetEventType() const
{
    return this->m_sEventType;
}


}   // bv
