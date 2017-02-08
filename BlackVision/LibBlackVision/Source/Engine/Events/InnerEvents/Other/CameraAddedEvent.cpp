#include "stdafx.h"
#include "CameraAddedEvent.h"




namespace bv
{

const EventType     CameraAddedEvent::m_sEventType = EventManager::RegisterType();
std::string         CameraAddedEvent::m_sEventName = "CameraAddedEvent";


//******************* CameraAddedEvent *************

// *************************************
//
void                CameraAddedEvent::Serialize            ( ISerializer & /*ser*/ ) const
{
    assert( !"" );
}

// *************************************
//
IEventPtr           CameraAddedEvent::Create          ( IDeserializer & /*deser*/ )
{
    assert( !"" );
    return nullptr;
}
// *************************************
//
IEventPtr           CameraAddedEvent::Clone             () const
{
    return IEventPtr( new CameraAddedEvent( *this ) );
}

// *************************************
//
EventType           CameraAddedEvent::Type()
{
    return m_sEventType;
}
// *************************************
//
std::string&        CameraAddedEvent::Name()
{
    return m_sEventName;
}
// *************************************
//
const std::string&  CameraAddedEvent::GetName() const
{
    return Name();
}
// *************************************
//
EventType           CameraAddedEvent::GetEventType() const
{
    return this->m_sEventType;
}


}   // bv
