#include "stdafx.h"
#include "LightAddedEvent.h"




namespace bv
{

const EventType     LightAddedEvent::m_sEventType = EventManager::RegisterType();
std::string         LightAddedEvent::m_sEventName = "LightAddedEvent";


//******************* LightAddedEvent *************

// *************************************
//
void                LightAddedEvent::Serialize            ( ISerializer & /*ser*/ ) const
{
    assert( !"" );
}

// *************************************
//
IEventPtr           LightAddedEvent::Create          ( IDeserializer & /*deser*/ )
{
    assert( !"" );
    return nullptr;
}
// *************************************
//
IEventPtr           LightAddedEvent::Clone             () const
{
    return IEventPtr( new LightAddedEvent( *this ) );
}

// *************************************
//
EventType           LightAddedEvent::Type()
{
    return m_sEventType;
}
// *************************************
//
std::string&        LightAddedEvent::Name()
{
    return m_sEventName;
}
// *************************************
//
const std::string&  LightAddedEvent::GetName() const
{
    return Name();
}
// *************************************
//
EventType           LightAddedEvent::GetEventType() const
{
    return this->m_sEventType;
}


}   // bv
