#include "SocketConnection.h"

#include "Tools/IncludeJSON.h"
#include "UseLoggerTCPModule.h"
#include "Engine/Events/Events.h"

#include <string>
#include <codecvt>


namespace bv
{
// ***********************
//
SocketConnection::SocketConnection( SOCKET socketID, QueueEventCallback callback )
    :   m_socketID( socketID ),
        m_sendCommandCallback( callback )
{
    m_logQueue = nullptr;
    m_logID = 0;
    m_state = SocketConnectionState::SCS_Uninitialized;
    m_end = false;
}

// ***********************
//
SocketConnection::~SocketConnection()
{}

// ***********************
// Always return after this function.
void SocketConnection::OnEndMainThread()
{
    closesocket( m_socketID );

    bv::Logger::GetLogger().RemoveLog( m_logID );
    m_logQueue = nullptr;

    m_state = SocketConnectionState::SCS_Ended;
}

void SocketConnection::InitThread          ()
{
    m_clientThread = std::thread( &SocketConnection::MainThread, this );
}

// ***********************
//
void SocketConnection::MainThread()
{
#define BUFFER_LENGTH 2048

	char buffer[ BUFFER_LENGTH + 1];
	int bufferLen = BUFFER_LENGTH;
	int bytecount;


    memset( buffer, 0, bufferLen );
    std::string command;

	const char START_TRANSMISSION = 0x02;
	const char END_TRANSMISSION = 0x03;
       
    // non blocking sockets, bitch!
    u_long iMode = 1;
    ioctlsocket( m_socketID, FIONBIO, &iMode );

    InitData initData = InitCommunication( m_socketID );

    if( !Authorization( m_socketID ) )
        return;

    if( initData.LogModules )
        m_logQueue = &bv::Logger::GetLogger().AddLogQueue( m_logID, (SeverityLevel)initData.SeverityLevel, initData.LogModules );

    m_state = SocketConnectionState::SCS_Running;

	while( !m_end )
	{

        if( initData.LogModules )
        {
            LogMsg logMsg;
            while( m_logQueue->TryPop( logMsg ) )
            {
                Json::Value responseJson;
                responseJson[ "cmd" ] = "Logger";
                responseJson[ "Severity" ] = std::move( logMsg.severity );
                responseJson[ "Module" ] = std::move( logMsg.module );
                responseJson[ "Message" ] = std::move( logMsg.message );

                ResponseMsg response;
                response.message = responseJson.toStyledString();
                response.socketID = (int)m_socketID;

                QueueResponse( std::move( response ) );
            }
        }

        ResponseMsg response;
        while( m_responseQueue.TryPop( response ) )
        {
            if( response.socketID == (int)m_socketID || response.socketID == SEND_BROADCAST_EVENT )
            {
                std::string toSend = START_TRANSMISSION + response.message + END_TRANSMISSION;
                SizeType bufferSize = toSend.length() + 1;

                // send the data
                int bufferSent = 0;
                while ( bufferSent < (int)bufferSize )
                {
                    int sentSize = send( m_socketID, toSend.c_str() + bufferSent, (int)bufferSize - bufferSent, 0);
					if (sentSize < 0)
					{
						int errorCode = WSAGetLastError();
						if (errorCode == 10054)
						{
							LOG_MESSAGE(SeverityLevel::info) << "connection reset by client, WSA_errorCode: " << errorCode;
							OnEndMainThread();
							break;
						}
						LOG_MESSAGE(SeverityLevel::info) << "send error ... -1 " << errorCode;
						if (errorCode == 10035)
							Sleep(100);
						if (errorCode == 10038)
						{
							OnEndMainThread();
							break;
						}
                        //OnEndMainThread();
                        //break;
                    }

                    bufferSent += sentSize;
                }
            }
		}


        if( ( bytecount = recv( m_socketID, (char*) buffer, bufferLen, 0 ) ) == SOCKET_ERROR )
        {
            int ierr = WSAGetLastError();
            if ( ierr == WSAEWOULDBLOCK )
            {  // currently no data available
                Sleep(50);  // wait and try again
                continue; 
            }

            LOG_MESSAGE( SeverityLevel::error ) << "Error receiving data: " << ierr <<". Closing socket...";

            OnEndMainThread();
            return;
		}

		if( bytecount > 0 )
			;
		else if( bytecount == 0 )
        {
			LOG_MESSAGE( SeverityLevel::info ) << "Client disconnected";

            OnEndMainThread();
			return;
		}


        int startOffset = 0;
        int endOffset = 0;

		for( ; endOffset < bytecount; endOffset++ )
		{
			if( buffer[ endOffset ] == START_TRANSMISSION )
			{
                startOffset = endOffset + 1;
                command.clear();
			}
            else if( buffer[ endOffset ] == END_TRANSMISSION )
			{
                buffer[ endOffset ] = '\0';             // Thanks to this replacement, we can pass buffer as null terminated string to command.
                command += ( buffer + startOffset );    // Append all characters from start.

                //std::wstring_convert < std::codecvt_utf8< wchar_t >, wchar_t > convert;
                //std::wstring str = convert.from_bytes( (const char*)command.c_str() );

                m_sendCommandCallback( command, (int)m_socketID );

                startOffset = endOffset + 1;            // This prevents from coping buffer second time if loop end here.
			}
		}

        if( startOffset < endOffset )
        {
            // Copy rest of buffer
            buffer[ endOffset ] = '\0';             // Thanks to this replacement, we can pass buffer as null terminated string to command.
            command += ( buffer + startOffset );    // Append all characters from start.
        }
			
		memset( buffer, 0, bufferLen );
	}
    OnEndMainThread();
}

// ***********************
//
InitData SocketConnection::InitCommunication   ( SOCKET socketID )
{
    InitData data;
	data.LogModules = 0;        // Don't use logger
    data.SeverityLevel = 0;     // Doesn't matter

    int bytesCount = 0;


    while( bytesCount < 8 )
    {
		if( ( bytesCount = recv( socketID, (char*)&data, sizeof( InitData ), 0 ) ) == SOCKET_ERROR )
        {
            int error = WSAGetLastError();
            if (error == WSAEWOULDBLOCK)
            {  // currently no data available
                Sleep(50);  // wait and try again
                continue;
            }

            return data;
		}
    }
        
    data.LogModules = ntohl( data.LogModules );
    data.SeverityLevel = ntohl( data.SeverityLevel );

    return data;
}

// ***********************
//
bool SocketConnection::Authorization       ( SOCKET /*socketID*/ )
{
    // Make real authorization here in future.
    return true;
}

// ***********************
//
void SocketConnection::QueueResponse       ( const ResponseMsg& message )
{
    m_responseQueue.Push( message );
}

// ***********************
//
void SocketConnection::QueueResponse       ( ResponseMsg&& message )
{
    m_responseQueue.Push( std::move( message ) );
}

// ***********************
//
void SocketConnection::KillClient          ()
{
    m_end = true;
    if( m_clientThread.joinable() )
        m_clientThread.join();      // Wait until MainThread function ends.
}

} //bv
