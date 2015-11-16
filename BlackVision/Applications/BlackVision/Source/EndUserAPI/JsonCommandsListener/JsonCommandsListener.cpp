#include "JsonCommandsListener.h"

#include "Serialization/JsonSpirit/JsonSpiritDeserializeObject.h"
#include "Serialization/JsonSpirit/JsonSpiritSerializeObject.h"
#include "Engine/Events/Interfaces/IEventManager.h"
#include "Engine/Events/EventHelpers.h"

#include "UseLogger.h"

namespace bv
{

// ***********************
//
JsonCommandsListener::JsonCommandsListener()
    : m_eventServer( nullptr )
{
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &JsonCommandsListener::SendResponse ), ResponseEvent::Type() );
}

JsonCommandsListener::~JsonCommandsListener()
{}

// ***********************
//
/// @param[in] eventString String to parse.
/// @param[in] socketID Event sender identifier.
void                JsonCommandsListener::QueueEvent          ( const std::wstring& eventString, int socketID )
{
    JsonSpiritDeserializeObject deser;

    if( !deser.LoadWString( eventString ) )
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Commands converter can't parse command: \n" + toString( eventString );
        return;
    }

    if( deser.EnterChild( L"Events" ) )
    {
        do
        {
            BaseEventPtr newEvent = std::static_pointer_cast<BaseEvent>( CreateEvent( deser ) );
            newEvent->SocketID = socketID;

            if( newEvent != nullptr )
                GetDefaultEventManager().ConcurrentQueueEvent( newEvent );
        }
        while( deser.NextChild() );
        deser.ExitChild();
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
    responseMessage.message = response->Response;   //std::move( response->Response );      // Maybe we could move this string... But we should guarantee only one listener lsitens to this event.

    m_eventServer->SendResponse( responseMessage );
}


bool JsonCommandsListener::InitializeServer    ( int port )
{
    m_eventServer = IEventServer::CreateServerObject();
    return m_eventServer->InitializeServer( this, port );
}

} //bv
