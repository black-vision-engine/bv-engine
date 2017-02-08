#include "stdafx.h"
#include "NodeRemovedEvent.h"




namespace bv
{

const EventType     NodeRemovedEvent::m_sEventType          = EventManager::RegisterType();
std::string         NodeRemovedEvent::m_sEventName          = "NodeRemovedEvent";


//******************* NodeRemovedEvent *************

// *************************************
//
void                NodeRemovedEvent::Serialize            ( ISerializer & /*ser*/ ) const
{
    assert( !"" );
}

// *************************************
//
IEventPtr           NodeRemovedEvent::Create          ( IDeserializer & /*deser*/ )
{
    assert( !"" );
    return nullptr;    
}
// *************************************
//
IEventPtr           NodeRemovedEvent::Clone             () const
{   return IEventPtr( new NodeRemovedEvent( *this ) );  }

// *************************************
//
EventType           NodeRemovedEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        NodeRemovedEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  NodeRemovedEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           NodeRemovedEvent::GetEventType() const
{   return this->m_sEventType; }


}   // bv
