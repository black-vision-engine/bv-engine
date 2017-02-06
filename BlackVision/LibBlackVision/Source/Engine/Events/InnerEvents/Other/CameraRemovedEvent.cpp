#include "stdafx.h"
#include "CameraRemovedEvent.h"




namespace bv
{

const EventType     CameraRemovedEvent::m_sEventType = EventManager::RegisterType();
std::string         CameraRemovedEvent::m_sEventName = "CameraRemovedEvent";


//******************* CameraRemovedEvent *************

// *************************************
//
void                CameraRemovedEvent::Serialize            ( ISerializer & /*ser*/ ) const
{
    assert( !"" );
}

// *************************************
//
IEventPtr           CameraRemovedEvent::Create          ( IDeserializer & /*deser*/ )
{
    assert( !"" );
    return nullptr;
}
// *************************************
//
IEventPtr           CameraRemovedEvent::Clone             () const
{
    return IEventPtr( new CameraRemovedEvent( *this ) );
}

// *************************************
//
EventType           CameraRemovedEvent::Type()
{
    return m_sEventType;
}
// *************************************
//
std::string&        CameraRemovedEvent::Name()
{
    return m_sEventName;
}
// *************************************
//
const std::string&  CameraRemovedEvent::GetName() const
{
    return Name();
}
// *************************************
//
EventType           CameraRemovedEvent::GetEventType() const
{
    return this->m_sEventType;
}


}   // bv
