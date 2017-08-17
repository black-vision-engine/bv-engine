#include "stdafx.h"
#include "NodeCopiedEvent.h"





namespace bv
{

const EventType     NodeCopiedEvent::m_sEventType = EventManager::RegisterType();
std::string         NodeCopiedEvent::m_sEventName = "NodeCopiedEvent";


//******************* NodeRemovedEvent *************

// *************************************
//
void                NodeCopiedEvent::Serialize            ( ISerializer & /*ser*/ ) const
{
    assert( !"" );
}

// *************************************
//
IEventPtr           NodeCopiedEvent::Create          ( IDeserializer & /*deser*/ )
{
    assert( !"" );
    return nullptr;
}
// *************************************
//
IEventPtr           NodeCopiedEvent::Clone             () const
{
    return IEventPtr( new NodeCopiedEvent( *this ) );
}

// *************************************
//
EventType           NodeCopiedEvent::Type()
{
    return m_sEventType;
}
// *************************************
//
std::string&        NodeCopiedEvent::Name()
{
    return m_sEventName;
}
// *************************************
//
const std::string&  NodeCopiedEvent::GetName() const
{
    return Name();
}
// *************************************
//
EventType           NodeCopiedEvent::GetEventType() const
{
    return this->m_sEventType;
}


}   // bv
