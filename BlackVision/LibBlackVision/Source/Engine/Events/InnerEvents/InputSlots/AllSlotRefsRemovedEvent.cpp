#include "stdafx.h"
#include "AllSlotRefsRemovedEvent.h"




namespace bv
{

const EventType     AllSlotRefsRemovedEvent::m_sEventType = EventManager::RegisterType();
std::string         AllSlotRefsRemovedEvent::m_sEventName = "AllSlotRefsRemovedEvent";


//******************* AllSlotRefsRemovedEvent *************

// *************************************
//
void                AllSlotRefsRemovedEvent::Serialize            ( ISerializer & /*ser*/ ) const
{
    assert( !"" );
}

// *************************************
//
IEventPtr           AllSlotRefsRemovedEvent::Create          ( IDeserializer & /*deser*/ )
{
    assert( !"" );
    return nullptr;
}
// *************************************
//
IEventPtr           AllSlotRefsRemovedEvent::Clone             () const
{
    return IEventPtr( new AllSlotRefsRemovedEvent( *this ) );
}

// *************************************
//
EventType           AllSlotRefsRemovedEvent::Type()
{
    return m_sEventType;
}
// *************************************
//
std::string&        AllSlotRefsRemovedEvent::Name()
{
    return m_sEventName;
}
// *************************************
//
const std::string&  AllSlotRefsRemovedEvent::GetName() const
{
    return Name();
}
// *************************************
//
EventType           AllSlotRefsRemovedEvent::GetEventType() const
{
    return this->m_sEventType;
}


}   // bv
