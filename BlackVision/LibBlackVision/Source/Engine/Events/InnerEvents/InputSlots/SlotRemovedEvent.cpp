#include "stdafx.h"
#include "SlotRemovedEvent.h"




namespace bv
{

const EventType     SlotRemovedEvent::m_sEventType = EventManager::RegisterType();
std::string         SlotRemovedEvent::m_sEventName = "SlotRemovedEvent";


//******************* SlotRemovedEvent *************

// *************************************
//
void                SlotRemovedEvent::Serialize            ( ISerializer & /*ser*/ ) const
{
    assert( !"" );
}

// *************************************
//
IEventPtr           SlotRemovedEvent::Create          ( IDeserializer & /*deser*/ )
{
    assert( !"" );
    return nullptr;
}
// *************************************
//
IEventPtr           SlotRemovedEvent::Clone             () const
{
    return IEventPtr( new SlotRemovedEvent( *this ) );
}

// *************************************
//
EventType           SlotRemovedEvent::Type()
{
    return m_sEventType;
}
// *************************************
//
std::string&        SlotRemovedEvent::Name()
{
    return m_sEventName;
}
// *************************************
//
const std::string&  SlotRemovedEvent::GetName() const
{
    return Name();
}
// *************************************
//
EventType           SlotRemovedEvent::GetEventType() const
{
    return this->m_sEventType;
}


}   // bv
