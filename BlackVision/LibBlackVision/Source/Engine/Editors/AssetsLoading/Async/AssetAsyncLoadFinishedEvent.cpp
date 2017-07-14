#include "stdafx.h"
#include "AssetAsyncLoadFinishedEvent.h"





namespace bv
{

const EventType     AssetAsyncLoadFinishedEvent::m_sEventType = EventManager::RegisterType();
std::string         AssetAsyncLoadFinishedEvent::m_sEventName = "AssetAsyncLoadFinishedEvent";


//******************* NodeRemovedEvent *************

// *************************************
//
void                AssetAsyncLoadFinishedEvent::Serialize      ( ISerializer & /*ser*/ ) const
{
    assert( !"" );
}

// *************************************
//
IEventPtr           AssetAsyncLoadFinishedEvent::Create          ( IDeserializer & /*deser*/ )
{
    assert( !"" );
    return nullptr;
}
// *************************************
//
IEventPtr           AssetAsyncLoadFinishedEvent::Clone             () const
{
    return IEventPtr( new AssetAsyncLoadFinishedEvent( *this ) );
}

// *************************************
//
EventType           AssetAsyncLoadFinishedEvent::Type()
{
    return m_sEventType;
}
// *************************************
//
std::string&        AssetAsyncLoadFinishedEvent::Name()
{
    return m_sEventName;
}
// *************************************
//
const std::string&  AssetAsyncLoadFinishedEvent::GetName() const
{
    return Name();
}
// *************************************
//
EventType           AssetAsyncLoadFinishedEvent::GetEventType() const
{
    return this->m_sEventType;
}


}   // bv
