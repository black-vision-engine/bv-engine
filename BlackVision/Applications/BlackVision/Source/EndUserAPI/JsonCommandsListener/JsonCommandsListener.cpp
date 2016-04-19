#include "JsonCommandsListener.h"

#include "Serialization/Json/JsonDeserializeObject.h"
#include "Serialization/Json/JsonSerializeObject.h"
#include "Engine/Events/Interfaces/IEventManager.h"

#include "Threading/ScopedCriticalSection.h"

#include "UseLoggerBVAppModule.h"

#undef CreateEvent
#undef max

namespace bv
{

// ***********************
//
JsonCommandsListener::JsonCommandsListener()
    :   m_eventServer( nullptr )
{
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &JsonCommandsListener::SendResponse ), ResponseEvent::Type() );
}

JsonCommandsListener::~JsonCommandsListener()
{
    delete m_eventServer;
}

// ***********************
//
/// @param[in] eventString String to parse.
/// @param[in] socketID Event sender identifier.
void                JsonCommandsListener::QueueEvent          ( const std::string& eventString, int socketID )
{
    auto eventsVec = GetDefaultEventManager().GetEventFactory().ParseEventsListJSON( eventString, socketID );

    for( auto & evt : eventsVec )
    {
        GetDefaultEventManager().ConcurrentQueueEvent( evt );
    }
}


// ***********************
//
void        JsonCommandsListener::SendResponse           ( const IEventPtr evt )
{
    if( evt->GetEventType() != bv::ResponseEvent::Type() )
        return;
    ResponseEventPtr response = std::static_pointer_cast<ResponseEvent>( evt );

    ResponseMsg responseMessage;
    responseMessage.socketID = response->SocketID;
    responseMessage.message = response->Response;   //std::move( response->Response );      // Maybe we could move this string... But we should guarantee only one listener listens to this event.

    m_eventServer->SendResponse( responseMessage );
}

// ***********************
//
bool JsonCommandsListener::InitializeServer    ( int port )
{
    m_eventServer = IEventServer::CreateServerObject();

    bool result = m_eventServer->InitializeServer( fastdelegate::MakeDelegate( this, &JsonCommandsListener::QueueEvent ), port );
    return result;
}

// ***********************
//
void JsonCommandsListener::DeinitializeServer  ()
{    m_eventServer->DeinitializeServer();   }


} //bv
