#include "stdafx.h"
#include "SlotAddedEvent.h"




namespace bv
{

const EventType     SlotAddedEvent::m_sEventType = EventManager::RegisterType();
std::string         SlotAddedEvent::m_sEventName = "SlotAddedEvent";


//******************* SlotAddedEvent *************

// *************************************
//
void                SlotAddedEvent::Serialize            ( ISerializer & /*ser*/ ) const
{
    assert( !"" );
}

// *************************************
//
IEventPtr           SlotAddedEvent::Create          ( IDeserializer & /*deser*/ )
{
    assert( !"" );
    return nullptr;
}
// *************************************
//
IEventPtr           SlotAddedEvent::Clone             () const
{
    return IEventPtr( new SlotAddedEvent( *this ) );
}

// *************************************
//
EventType           SlotAddedEvent::Type()
{
    return m_sEventType;
}
// *************************************
//
std::string&        SlotAddedEvent::Name()
{
    return m_sEventName;
}
// *************************************
//
const std::string&  SlotAddedEvent::GetName() const
{
    return Name();
}
// *************************************
//
EventType           SlotAddedEvent::GetEventType() const
{
    return this->m_sEventType;
}


}   // bv
