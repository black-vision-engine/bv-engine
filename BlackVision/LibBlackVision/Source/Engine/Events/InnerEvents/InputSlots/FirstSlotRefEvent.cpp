#include "stdafx.h"
#include "FirstSlotRefEvent.h"




namespace bv
{

const EventType     FirstSlotRefEvent::m_sEventType = EventManager::RegisterType();
std::string         FirstSlotRefEvent::m_sEventName = "FirstSlotRefEvent";


//******************* FirstSlotRefEvent *************

// *************************************
//
void                FirstSlotRefEvent::Serialize            ( ISerializer & /*ser*/ ) const
{
    assert( !"" );
}

// *************************************
//
IEventPtr           FirstSlotRefEvent::Create          ( IDeserializer & /*deser*/ )
{
    assert( !"" );
    return nullptr;
}
// *************************************
//
IEventPtr           FirstSlotRefEvent::Clone             () const
{
    return IEventPtr( new FirstSlotRefEvent( *this ) );
}

// *************************************
//
EventType           FirstSlotRefEvent::Type()
{
    return m_sEventType;
}
// *************************************
//
std::string&        FirstSlotRefEvent::Name()
{
    return m_sEventName;
}
// *************************************
//
const std::string&  FirstSlotRefEvent::GetName() const
{
    return Name();
}
// *************************************
//
EventType           FirstSlotRefEvent::GetEventType() const
{
    return this->m_sEventType;
}


}   // bv
