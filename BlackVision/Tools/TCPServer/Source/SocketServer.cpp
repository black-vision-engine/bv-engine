#include "SocketServer.h"
#include "UseLoggerTCPModule.h"
#include "Engine/Events/Events.h"

#include <thread>
#include "Threading/ScopedCriticalSection.h"

#include "win_sock.h"

// Fixme: Link this library in project settings.
#pragma comment(lib, "Ws2_32.lib")

namespace bv
{

std::map<SOCKET, SocketConnectionPtr>               SocketServer::clientsMap;
CriticalSection                                 SocketServer::clientsCriticalSection;


// ***********************
//
IEventServer* IEventServer::CreateServerObject  ()
{
    return new SocketServer();
}

// ***********************
//
SocketServer::SocketServer()
{}
SocketServer::~SocketServer()
{}

// ***********************
//
bool SocketServer::InitializeServer( const QueueEventCallback& callback, int port )
{
    m_port = port;
    if( callback.empty() )
        return false;

    m_sendCommandCallback = callback;
    std::thread waitForConnectionThread = std::thread( &SocketServer::WaitForConnection, std::ref( *this ) );
    waitForConnectionThread.detach();

    return true;
}

// ***********************
//
void SocketServer::WaitForConnection       ()
{
//The port you want the server to listen on
	unsigned short wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD( 2, 2 );
 	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 || ( LOBYTE( wsaData.wVersion ) != 2 ||
			HIBYTE( wsaData.wVersion ) != 2 ))
    {
		LOG_MESSAGE( SeverityLevel::error ) << "Server Socket - could not find lib " << WSAGetLastError();
		return;
	}

	//Initialize sockets and set any options
	SOCKET hsock;
	int * p_int ;
	hsock = socket(AF_INET, SOCK_STREAM, 0);
	if(hsock == -1)
    {
		LOG_MESSAGE( SeverityLevel::error ) << "Error initializing socket " << WSAGetLastError();
		return;
	}
	
	p_int = (int*)malloc(sizeof(int));
	*p_int = 1;
	if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
		(setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) )
    {
		LOG_MESSAGE( SeverityLevel::error ) << "Error setting options %d\n" << WSAGetLastError();
		free(p_int);
		return;
	}
	free(p_int);

	//Bind and listen
	struct sockaddr_in my_addr;

	my_addr.sin_family = AF_INET ;
	my_addr.sin_port = htons( static_cast<u_short>(m_port) );
	
	memset(&(my_addr.sin_zero), 0, 8);
	my_addr.sin_addr.s_addr = INADDR_ANY ;
	
	if( ::bind( hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1 )
    {
		LOG_MESSAGE( SeverityLevel::error ) << "Error binding to socket, make sure listening port is free " << WSAGetLastError();
		return;
	}
	if(listen( hsock, 10) == -1 )
    {
		LOG_MESSAGE( SeverityLevel::error ) << "Error listening " << WSAGetLastError();
		return;
	}
	
		
    SOCKET socketID;
    sockaddr_in sadr;
    int	addr_size = sizeof(SOCKADDR);

    for(;;)
    {
        if( ( socketID = accept( hsock, (SOCKADDR*)&sadr, &addr_size) )!= INVALID_SOCKET )
        {
            // All unused clients have to be removed before inserting new client. In other case
            // new connection can't be inserted into clientsMap and will be illigally destroyed what causes exception.
            RemoveUnusedClients();

            {
                ScopedCriticalSection lock( clientsCriticalSection );

                SocketConnectionPtr newClient = std::make_shared<SocketConnection>( socketID, m_sendCommandCallback );
                newClient->InitThread();
                clientsMap.insert( std::make_pair( socketID, newClient ) );
            }

            LOG_MESSAGE( SeverityLevel::info ) << "Client connected: " + std::string( inet_ntoa( sadr.sin_addr ) );
        }
        else
        {
            LOG_MESSAGE( SeverityLevel::error ) <<"Error accepting client " << WSAGetLastError();
        }
    }
}

// ***********************
//
void SocketServer::RemoveUnusedClients     ()
{
    ScopedCriticalSection lock( clientsCriticalSection );

    auto iterator = clientsMap.begin();
    while( iterator != clientsMap.end() )
    {
        if( iterator->second->GetState() == SocketConnectionState::SCS_Ended )
        {
            iterator->second->KillClient();
            iterator = clientsMap.erase( iterator );
        }
        else
            ++iterator;
    }
}

// ***********************
//
void SocketServer::SendResponse( ResponseMsg& message )
{
    ScopedCriticalSection lock( clientsCriticalSection );

    if( message.socketID == SEND_BROADCAST_EVENT )
    {
        for( auto& client : clientsMap )
            client.second->QueueResponse( message );
    }
    else
    {
        auto client = clientsMap.find( message.socketID );
        if( client != clientsMap.end() )
            client->second->QueueResponse( std::move( message ) );
    }
}

// ***********************
//
void SocketServer::DeinitializeServer      ()
{
    ScopedCriticalSection lock( clientsCriticalSection );

    auto iterator = clientsMap.begin();
    while( iterator != clientsMap.end() )
    {
        iterator->second->KillClient();
        ++iterator;
    }
}

} //bv
