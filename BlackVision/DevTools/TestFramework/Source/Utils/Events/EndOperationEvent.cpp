#include "stdafx.h"
#include "EndOperationEvent.h"





namespace bv
{

const EventType     EndOperationEvent::m_sEventType = EventManager::RegisterType();
std::string         EndOperationEvent::m_sEventName = "EndOperationEvent";


//******************* NodeRemovedEvent *************

// *************************************
//
void                EndOperationEvent::Serialize            ( ISerializer & /*ser*/ ) const
{
    assert( !"" );
}

// *************************************
//
IEventPtr           EndOperationEvent::Create          ( IDeserializer & /*deser*/ )
{
    assert( !"" );
    return nullptr;
}
// *************************************
//
IEventPtr           EndOperationEvent::Clone             () const
{
    return IEventPtr( new EndOperationEvent( *this ) );
}

// *************************************
//
EventType           EndOperationEvent::Type()
{
    return m_sEventType;
}
// *************************************
//
std::string&        EndOperationEvent::Name()
{
    return m_sEventName;
}
// *************************************
//
const std::string&  EndOperationEvent::GetName() const
{
    return Name();
}
// *************************************
//
EventType           EndOperationEvent::GetEventType() const
{
    return this->m_sEventType;
}


}   // bv
