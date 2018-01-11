#include "stdafxBVApp.h"

#include "JsonCommandsListener.h"

#include "Serialization/Json/JsonDeserializeObject.h"
#include "Serialization/Json/JsonSerializeObject.h"
#include "Engine/Events/Interfaces/IEventManager.h"

#include "Threading/ScopedCriticalSection.h"

#include "System/Time.h"
#include "System/Path.h"
#include "IO/FileIO.h"
#include <ctime>


#include "UseLoggerBVAppModule.h"

#undef CreateEvent
#undef max

namespace bv
{

// ***********************
//
JsonCommandsListener::JsonCommandsListener()
    :   m_eventServer( nullptr )
    ,   m_debugLayer( false )
    ,   m_numEvents( 0 )
    ,   m_numResponses( 0 )
{
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &JsonCommandsListener::SendResponse ), ResponseEvent::Type() );
}

// ***********************
//
JsonCommandsListener::~JsonCommandsListener()
{
    DeinitDebugLayer();
    delete m_eventServer;
}

// ***********************
//
/// @param[in] eventString String to parse.
/// @param[in] socketID Event sender identifier.
void                JsonCommandsListener::QueueEvent          ( const std::string& eventString, int socketID )
{
    // Works only when m_debugLayer is set to true.
    DebugLayerProcessEvent( eventString );

    auto eventsVec = GetDefaultEventManager().GetEventFactory().ParseEventsListJSON( eventString, socketID );

    for( auto & evt : eventsVec )
    {
        GetDefaultEventManager().ConcurrentQueueEvent( evt );

        m_numEvents++;
    }
}


// ***********************
//
void                JsonCommandsListener::SendResponse      ( const IEventPtr evt )
{
    if( evt->GetEventType() != bv::ResponseEvent::Type() )
        return;

    ResponseEventPtr response = std::static_pointer_cast<ResponseEvent>( evt );

    ResponseMsg responseMessage;
    responseMessage.socketID = response->SocketID;
    responseMessage.message = response->Response;   //std::move( response->Response );      // Maybe we could move this string... But we should guarantee only one listener listens to this event.

    // Works only when m_debugLayer is set to true.
    DebugLayerProcessResponse( responseMessage );

    m_eventServer->SendResponse( responseMessage );
}

// ***********************
//
bool                JsonCommandsListener::InitializeServer  ( int port )
{
    m_eventServer = IEventServer::CreateServerObject();

    bool result = m_eventServer->InitializeServer( fastdelegate::MakeDelegate( this, &JsonCommandsListener::QueueEvent ), port );
    return result;
}

// ***********************
//
void                JsonCommandsListener::DeinitializeServer  ()
{    m_eventServer->DeinitializeServer();   }



// ========================================================================= //
// Debug layer
// ========================================================================= //

const char* EVENT_BEGIN_HEADER      = "#Evt#";
const char* RESPONSE_BEGIN_HEADER   = "#Res#";
const char* END_OF_INSCRIPTION      = "#End#\n";

// ***********************
//
void                JsonCommandsListener::InitializeDebugLayer  ( const std::string & resultPath )
{
    m_debugLayer = true;
    m_resultDirectory = resultPath;

    auto resultFileName = MakeDebugResultFilePath();

    if( !Path::Exists( resultFileName ) )
    {
        File::Touch( resultFileName );
    }

    m_resultFile.open( resultFileName, std::ios_base::out );
    
    if( m_resultFile.fail() )
    {
        m_debugLayer = false;
    }
}

// ***********************
//
void                JsonCommandsListener::DeinitDebugLayer      ()
{
    if( m_debugLayer )
    {
        if( m_resultFile.is_open() )
        {
            m_resultFile.close();
        }
    }
}

// ***********************
//
void                JsonCommandsListener::DebugLayerProcessResponse   ( ResponseMsg & response )
{
    m_numResponses++;

    if( !m_debugLayer )
        return;

    std::lock_guard< std::mutex > lock( m_debugFileMutex );

    m_resultFile << RESPONSE_BEGIN_HEADER;
    m_resultFile << GetFormattedTime() << "#\n";

    m_resultFile << response.message << std::endl;

    m_resultFile << END_OF_INSCRIPTION;
}

// ***********************
//
void                JsonCommandsListener::DebugLayerProcessEvent        ( const std::string & eventString )
{
    if( !m_debugLayer )
        return;

    std::lock_guard< std::mutex > lock( m_debugFileMutex );

    m_resultFile << EVENT_BEGIN_HEADER;
    m_resultFile << GetFormattedTime() << "#\n";

    m_resultFile << eventString << std::endl;

    m_resultFile << END_OF_INSCRIPTION;

    m_resultFile.flush();
}

#pragma warning( push )
#pragma warning( disable : 4996 )

// ***********************
//
std::string         JsonCommandsListener::MakeDebugResultFilePath     ()
{
    char timestamp[ 20 ];
    time_t data = time( nullptr );
    tm* now = localtime( &data );
    strftime( timestamp, 20, "%Y.%m.%d_%H.%M.%S", now );

    auto resultFile = Path( m_resultDirectory ) / Path( "test_" + std::string( timestamp ) + ".bvtest" );
    return resultFile.Str();
}



// ***********************
//
std::string         JsonCommandsListener::GetFormattedTime()
{
    auto now = Time::Now();

    int hours = static_cast< int >( now / (1000 * 60 * 60) );
    int minutes = static_cast< int >( now / (1000 * 60) % 60 );
    int seconds = static_cast< int >( now / 1000 ) % 60;
    int millis = static_cast< int >( now % 1000 );

    char timestamp[ 15 ];

    sprintf( timestamp, "%04d", hours );
    sprintf( timestamp + 5, "%02d", minutes );
    sprintf( timestamp + 8, "%02d", seconds );
    sprintf( timestamp + 11, "%03d", millis );

    timestamp[ 4 ] = ':';
    timestamp[ 7 ] = ':';
    timestamp[ 10 ] = ':';
    timestamp[ 14 ] = '\0';

    std::string result = timestamp;

    return result;
}

#pragma warning( pop )

} //bv
