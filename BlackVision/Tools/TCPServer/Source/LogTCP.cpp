#include "LogTCP.h"

#include "win_sock.h"
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>


#pragma comment(lib, "ws2_32.lib") 

#define   SOCKET_ERRNO   WSAGetLastError() 



namespace bv
{

LogTCP::~LogTCP()
{
    m_end = true;

    if( m_mainThread.joinable() )
        m_mainThread.join();
}
    
void LogTCP::Initialize      ( const std::string& address, unsigned short port )
{
    m_address = address;
    m_port = port;

    m_mainThread = std::thread( &LogTCP::MainThread, this );
}

SOCKET LogTCP::ConnectToPort()
{ 
    struct sockaddr_in addr; 
    SOCKET hSocket; 

    // Create socket 
    hSocket = socket( PF_INET, SOCK_STREAM, 0 ); 
    if( hSocket == INVALID_SOCKET ) 
    { 
        printf( "socket() error %d\n", SOCKET_ERRNO ); 
        return (SOCKET) -1;
    } 

    // Connexion setting for local connexion 
    addr.sin_family = AF_INET ; 
    addr.sin_addr.s_addr = inet_addr(m_address.c_str()); 
    addr.sin_port = htons (m_port); 

    // Connect 
    if( connect( hSocket, (struct sockaddr *)&addr, sizeof(addr) ) == SOCKET_ERROR ) 
    { 
        // As we are in non-blocking mode we'll always have the error 
        // WSAEWOULDBLOCK whichis actually not one 
        if( SOCKET_ERRNO != WSAEWOULDBLOCK ) 
        { 
            printf( "connect() error (%d)\n", SOCKET_ERRNO ); 
            return (SOCKET)-1 ;
        } 
    } 

    return hSocket; 
} 

void LogTCP::MainThread()
{

    WSADATA stack_info; 
    SOCKET ahSocket[1]; 
    WSAEVENT ahEvents[1]; 
    DWORD dwEvent; 
    WSANETWORKEVENTS NetworkEvents; 
    int rc; 

    // Initialize Winsock 
    WSAStartup(MAKEWORD(2,0), &stack_info) ; 

    // Create event 
    ahEvents[0] = WSACreateEvent(); 

    // Create and connect a socket on the server socket 
    ahSocket[0]= ConnectToPort(); 
    if(ahSocket[0] ==-1)
    {
        printf("Logger socket error :(\r\n");
        return;
    }

    // not sure if I have to use or not 
    /*u_long arg = 1; 
    ioctlsocket( ahSocket[0] , FIONBIO, &arg );*/ 

    // the application wants to receive notification of a completed connection 
    rc = WSAEventSelect(ahSocket[0], ahEvents[0], FD_CONNECT  ); 
    if( rc == SOCKET_ERROR ) 
    { 
        printf( "WSAEventSelect() error %d\n", SOCKET_ERRNO ); 
        return;
    } 

    while( !m_end ) 
    { 
        //Sleep(100);  
        // look for events 
        dwEvent = WSAWaitForMultipleEvents( 1, ahEvents, FALSE, 500, FALSE); 
        printf ("sock : %d\n",dwEvent);
        switch (dwEvent) 
        { 
        case WSA_WAIT_FAILED: 
            printf("WSAEventSelect: %d\n", WSAGetLastError()); 
            break; 
        case WAIT_IO_COMPLETION: 
            break;
        case WSA_WAIT_TIMEOUT: 
            printf ("TIMEOUT\n",dwEvent);
            break; 

        default: 

            printf("while\n"); 

            //if there is one dwEvent-WSA_WAIT_EVENT_0 has to be substracted so as to dwEvent correspond to the index of the concerned socket 
            dwEvent -= WSA_WAIT_EVENT_0; 

            // enumeration of the events on the socket[dwEvent] 
            if (SOCKET_ERROR == WSAEnumNetworkEvents(ahSocket[dwEvent], ahEvents[dwEvent], &NetworkEvents)) 
            { 
                printf("WSAEnumNetworkEvent: %d lNetworkEvent %X\n", WSAGetLastError(), NetworkEvents.lNetworkEvents); 
                NetworkEvents.lNetworkEvents = 0; 
            } 
            else  
            { 


                if (FD_CONNECT & NetworkEvents.lNetworkEvents) 
                { 
                    //connexion is OK 
                    printf( "FD_CONNECT ok (dwEvent=%d)\n", dwEvent ); 

                    // now that we are connected we want to send data or be aware when the other socket is disconnected 
                    rc = WSAEventSelect(ahSocket[dwEvent], ahEvents[dwEvent], FD_CLOSE | FD_WRITE ); 
                    if( rc == SOCKET_ERROR ) 
                    { 
                        printf( "WSAEventSelect() error %d\n", SOCKET_ERRNO ); 
                        exit(1); 
                    } 
                } 
                if (FD_CLOSE   & NetworkEvents.lNetworkEvents) 
                { 
                    printf( "FD_CLOSE ok (dwEvent=%d)\n", dwEvent ); 

                    WSACloseEvent( ahEvents[0] ); 
                    exit(0); 
                } 

                if (FD_WRITE & NetworkEvents.lNetworkEvents) 
                { 

                    int cbBuffer; 

                    printf( "FD_WRITE ok (dwEvent=%d)\n", dwEvent ); 

                    // create string and return the size 
                    while( !m_end )
                    {
                        Sleep(50);

                        LogMsg logMsg;
                        if( !m_queue.TryPop( logMsg ) )
                        {
                            int msg_size=20+(int)logMsg.message.size()+(int)logMsg.module.size()+(int)logMsg.severity.size();
                            wchar_t *tmpBuffer = new wchar_t[msg_size];
                            cbBuffer = swprintf(tmpBuffer,msg_size,L"\r\n+log|%ls|%ls|info|%ls\r\n\0",logMsg.module.c_str(),logMsg.severity.c_str(),logMsg.message.c_str() ); 


                            // send the string with 0 at the end 
                            //rc = send( ahSocket[dwEvent], tmpBuffer, cbBuffer + 1, 0 ); 

                            size_t buffer_size;
                            wcstombs_s(&buffer_size, NULL, 0, tmpBuffer, _TRUNCATE);

                            // do the actual conversion
                            char *buffer = (char*) malloc(buffer_size);
                            wcstombs_s(&buffer_size, buffer, (int)buffer_size, tmpBuffer, _TRUNCATE);

                            // send the data
                            int buffer_sent = 0;
                            while (buffer_sent < (int) buffer_size)
                            {
                                int sent_size = send(ahSocket[dwEvent], buffer+buffer_sent, (int)buffer_size-buffer_sent, 0);
                                buffer_sent += sent_size;
                            }

                            if (SOCKET_ERROR ==  rc) 
                            { 
                                printf("WSAEnumNetworkEvent: %d lNetworkEvent %X\n",  WSAGetLastError(), NetworkEvents.lNetworkEvents); 
                            }
                        } 
                    }

                    // not sure if I have to use it 
                    //WSAResetEvent(ahEvents[0]); 

                } 

            } 
        } 
    } 
}


} //bv
