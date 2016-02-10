#include "JsonCommandsListener.h"

#include "Serialization/Json/JsonDeserializeObject.h"
#include "Serialization/Json/JsonSerializeObject.h"
#include "Engine/Events/Interfaces/IEventManager.h"
#include "Engine/Events/EventHelpers.h"

#include "Threading/ScopedCriticalSection.h"

#include "../../UseLoggerBVAppModule.h"

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
    JsonDeserializeObject deser;

    if( !deser.Load( eventString ) )
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Remote controller can't parse command: \n" + toString( eventString );
        return;
    }

    TryParseRegularEvent( deser, socketID );
}

// ***********************
//
void                JsonCommandsListener::AddTriggeredEvent   ( unsigned int requestedFrame, RemoteEventPtr& eventPtr )
{
    ScopedCriticalSection lock( m_eventsMapLock );
    m_triggeredEvents.insert( std::make_pair( requestedFrame, eventPtr ) );
}

// ***********************
//
void                JsonCommandsListener::TryParseRegularEvent( IDeserializer & deser, int socketID )
{
    if( !deser.EnterChild( "Events" ) )
        return;

    if( deser.EnterChild( "Event" ) )
    {
        do
        {
            auto newEvent = DeserializeEvent( deser );
            if( newEvent != nullptr )
            {
                RemoteEventPtr newEventBase = std::static_pointer_cast< RemoteEvent >( newEvent );
                newEventBase->SocketID = socketID;
                newEventBase->EventID = SerializationHelper::String2T( deser.GetAttribute( "EventID" ), std::numeric_limits< int >::max() );

                GetDefaultEventManager().ConcurrentQueueEvent( newEventBase );
            }

        } while( deser.NextChild() );

        deser.ExitChild();  // Event
    }
        
    deser.ExitChild();  // Events

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
