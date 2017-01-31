#include "stdafx.h"
#include "LightRemovedEvent.h"




namespace bv
{

const EventType     LightRemovedEvent::m_sEventType = EventManager::RegisterType();
std::string         LightRemovedEvent::m_sEventName = "LightRemovedEvent";


//******************* LightRemovedEvent *************

// *************************************
//
void                LightRemovedEvent::Serialize            ( ISerializer & /*ser*/ ) const
{
    assert( !"" );
}

// *************************************
//
IEventPtr           LightRemovedEvent::Create          ( IDeserializer & /*deser*/ )
{
    assert( !"" );
    return nullptr;
}
// *************************************
//
IEventPtr           LightRemovedEvent::Clone             () const
{
    return IEventPtr( new LightRemovedEvent( *this ) );
}

// *************************************
//
EventType           LightRemovedEvent::Type()
{
    return m_sEventType;
}
// *************************************
//
std::string&        LightRemovedEvent::Name()
{
    return m_sEventName;
}
// *************************************
//
const std::string&  LightRemovedEvent::GetName() const
{
    return Name();
}
// *************************************
//
EventType           LightRemovedEvent::GetEventType() const
{
    return this->m_sEventType;
}


}   // bv
