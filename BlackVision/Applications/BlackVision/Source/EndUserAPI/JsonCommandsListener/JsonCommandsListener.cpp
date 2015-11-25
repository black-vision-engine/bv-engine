#include "JsonCommandsListener.h"

#include "Serialization/JsonSpirit/JsonSpiritDeserializeObject.h"
#include "Serialization/JsonSpirit/JsonSpiritSerializeObject.h"
#include "Engine/Events/Interfaces/IEventManager.h"
#include "Engine/Events/EventHelpers.h"

#include "Threading/ScopedCriticalSection.h"

#include "UseLogger.h"

#undef CreateEvent

namespace bv
{

// ***********************
//
JsonCommandsListener::JsonCommandsListener()
    :   m_eventServer( nullptr ),
        m_remoteLog( nullptr )
{
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &JsonCommandsListener::SendResponse ), ResponseEvent::Type() );
}

JsonCommandsListener::~JsonCommandsListener()
{
    delete m_remoteLog;
    delete m_eventServer;
}

// ***********************
//
/// @param[in] eventString String to parse.
/// @param[in] socketID Event sender identifier.
void                JsonCommandsListener::QueueEvent          ( const std::wstring& eventString, int socketID )
{
    JsonSpiritDeserializeObject deser;

    if( !deser.LoadWString( eventString ) )
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Remote controller can't parse command: \n" + toString( eventString );
        return;
    }

    TryParseRegularEvent( deser, socketID );
    TryParseEventsGroup( deser, socketID );
}

// ***********************
//
void                JsonCommandsListener::TryParseEventsGroup ( IDeserializer& deser, int socketID )
{
    if( deser.EnterChild( L"EventsGroups" ) )
    {
        unsigned int frameTrigger = std::stoul( deser.GetAttribute( L"FrameTrigger" ) );

        if( deser.EnterChild( L"Events" ) )
        {
            do
            {
                BaseEventPtr newEvent = std::static_pointer_cast<BaseEvent>( DeserializeEvent( deser ) );
                newEvent->SocketID = socketID;

                if( newEvent != nullptr )
                    AddTriggeredEvent( frameTrigger, newEvent );
            }
            while( deser.NextChild() );
            deser.ExitChild();
        }
    }
}

// ***********************
//
void                JsonCommandsListener::AddTriggeredEvent   ( unsigned int requestedFrame, BaseEventPtr& eventPtr )
{
    ScopedCriticalSection lock( m_eventsMapLock );
    m_triggeredEvents.insert( std::make_pair( requestedFrame, eventPtr ) );
}

// ***********************
//
void                JsonCommandsListener::TryParseRegularEvent( IDeserializer& deser, int socketID )
{
    if( deser.EnterChild( L"Events" ) )
    {
        do
        {
            BaseEventPtr newEvent = std::static_pointer_cast<BaseEvent>( DeserializeEvent( deser ) );
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
bool JsonCommandsListener::InitializeRemoteLog ( const std::string& /*address*/, unsigned short /*port*/, SeverityLevel /*minLevel*/, int /*modules*/ )
{
    //QueueConcurrent<LogMsg>& queue = Logger::GetLogger().AddLogQueue( minLevel, modules );
    //m_remoteLog = new LogTCP( queue );
    //m_remoteLog->Initialize( address, port );

    return true;
}


} //bv
