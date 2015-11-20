#pragma warning(disable :4996)
#include <codecvt>
#include<locale>
#include "SocketWrapper.h"
#include <math.h>

#include "UseLogger.h"

#include <iostream>
#pragma comment(lib, "Ws2_32.lib")

namespace bv{
	

    IEventServer* IEventServer::CreateServerObject  ()
    {
        return new SocketWrapper();
    }

	//DWORD WINAPI SocketHandler(void*);
    std::vector<ResponseMsg> SocketWrapper::Responses;
    QueueEventCallback SocketWrapper::SendCommandCallback = nullptr;

	SocketWrapper::SocketWrapper()
	{}

	SocketWrapper::~SocketWrapper()
    {}


    bool SocketWrapper::InitializeServer( const QueueEventCallback& callback, int port )
    {
        m_port = port;
        if( callback.empty() )
            return false;

        SendCommandCallback = callback;
        return InitServer();
    }

    void SocketWrapper::SendResponse        ( ResponseMsg& message )
    {
        Responses.push_back( std::move( message ) );
    }

	bool SocketWrapper::InitServer()
	{
		CreateThread(0,0,(LPTHREAD_START_ROUTINE)SocketInitHandler, this , 0,0);	
		return true;
	}


    bool SocketWrapper::thread_clients(void)
	{
//The port you want the server to listen on
		int host_port = m_port;

		
		unsigned short wVersionRequested;
		WSADATA wsaData;
		int err;
		wVersionRequested = MAKEWORD( 2, 2 );
 		err = WSAStartup( wVersionRequested, &wsaData );
		if ( err != 0 || ( LOBYTE( wsaData.wVersion ) != 2 ||
				HIBYTE( wsaData.wVersion ) != 2 )) {
			fprintf(stderr, "Server Socket - could not find lib %d\n",WSAGetLastError());
			return false;
		}

		//Initialize sockets and set any options
		SOCKET hsock;
		int * p_int ;
		hsock = socket(AF_INET, SOCK_STREAM, 0);
		if(hsock == -1){
			printf("Error initializing socket %d\n",WSAGetLastError());
			return false;
		}
	
		p_int = (int*)malloc(sizeof(int));
		*p_int = 1;
		if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
			(setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) ){
			printf("Error setting options %d\n", WSAGetLastError());
			free(p_int);
			return false;
		}
		free(p_int);

		//Bind and listen
		struct sockaddr_in my_addr;

		my_addr.sin_family = AF_INET ;
		my_addr.sin_port = htons( static_cast<u_short>(host_port) );
	
		memset(&(my_addr.sin_zero), 0, 8);
		my_addr.sin_addr.s_addr = INADDR_ANY ;
	
		if( ::bind( hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1 ){
			fprintf(stderr,"Error binding to socket, make sure listening port is free %d\n",WSAGetLastError());
			return false;
		}
		if(listen( hsock, 10) == -1 ){
			fprintf(stderr, "Error listening %d\n",WSAGetLastError());
			return false;
		}
	
		

        //int* csock;
        SOCKET* csock;
        sockaddr_in sadr;
        int	addr_size = sizeof(SOCKADDR);

        for(;;)
        {
            printf("waiting for clients...\n");
            csock = (SOCKET*)malloc(sizeof(int));

            if((*csock = accept( hsock, (SOCKADDR*)&sadr, &addr_size))!= INVALID_SOCKET ){
                printf("Received connection from %s\n",inet_ntoa(sadr.sin_addr));
                LOG_MESSAGE( SeverityLevel::info ) << "Client connected: " + std::string( inet_ntoa( sadr.sin_addr ) );
                CreateThread(0,0,SocketHandler, (void*)csock , 0,0);
            }
            else
            {
                LOG_MESSAGE( SeverityLevel::error ) <<"Error accepting client " << WSAGetLastError();
            }
        }
    }
	DWORD SocketWrapper::SocketInitHandler(void *lp)
	{
        SocketWrapper *myObj = (SocketWrapper*)lp;
        return myObj->thread_clients();
	}


    std::vector<std::wstring> & split( const std::wstring & s, wchar_t delim, std::vector<std::wstring> & elems ) 
	{
		std::wstringstream ss(s);
		std::wstring item;
		while(std::getline(ss, item, delim)) 
		{
			elems.push_back(item);
		}
		return elems;
	}


    /**
 * Convert a Windows wide string to a UTF-8 (multi-byte) string.
 */


	DWORD SocketWrapper::SocketHandler( void * lp )
	{
		SOCKET *csock = (SOCKET*)lp;

		char buffer[1024];
        //wchar_t w_buffer[1024];
		int buffer_len = 512;
		int bytecount;

		//char ParseBuffer[2048];

		memset(buffer, 0, buffer_len);
		std::wstring temp;
        std::string temp_s;
		std::wstring cmd;

		const char START_TRANSMISSION = 0x02;
		const char END_TRANSMISSION = 0x03;
       
        // non blocking sockets, bitch!
        u_long iMode=1;
        ioctlsocket(*csock,FIONBIO,&iMode);

		for(;;)
		{
			//sending

            if(Responses.size()>0)
            {
                printf(" s¹ wiadomoœci do wys³ania!\n");
                for(unsigned int i=0;i<Responses.size();i++)
                {

                    if( Responses[i].socketID == (int) *csock && Responses[i].sent == false )
                    {
                        printf("Sock id %d zgadza siê!\n",*csock);
                        
                        printf("Sock id %d zgadza siê!\n",*csock);
						wchar_t CHAR_BEGIN = 0x02;
						wchar_t CHAR_END = 0x03;
                        std::wstring to_send = CHAR_BEGIN+Responses[i].message+CHAR_END;
                        const wchar_t* tmpBuffer = to_send.c_str();

                        size_t buffer_size;
                        wcstombs_s(&buffer_size, NULL, 0, tmpBuffer, _TRUNCATE);

                        // do the actual conversion
                        char *buffer = (char*) malloc(buffer_size);
                        wcstombs_s(&buffer_size, buffer, buffer_size, tmpBuffer, _TRUNCATE);

                        // send the data
                        int buffer_sent = 0;
                        while (buffer_sent < (int) buffer_size)
						{
                            int sent_size = send(*csock, buffer+buffer_sent, (int)buffer_size-buffer_sent, 0);
                            buffer_sent += sent_size;
                        }
                        Responses[i].sent = true;
                        //delete tmpBuffer;
                        Responses.erase(Responses.begin()+i);
                       
                    }
                }
			}

            //receiving
            //printf("malpka1\n");


			if((bytecount = recv(*csock, (char*) buffer, buffer_len, 0))==SOCKET_ERROR){
                int ierr= WSAGetLastError();
                 if (ierr==WSAEWOULDBLOCK) {  // currently no data available
                    Sleep(50);  // wait and try again
                    continue; 
                 }

				fprintf(stderr, "Error receiving data %d\n", ierr);
				fprintf(stderr, "Closing socket...");
				Sleep(50);
				//free(csock);
				Sleep(50);
				return 0;
			}
            // printf("malpka2\n");

			if(bytecount>0)
				;//printf("[Received] \"%s\"\n", buffer);
			else if(bytecount==0){
				printf("[client disconnected]\n");
				closesocket(*csock);
				return 0;
			}
            // printf("malpka3\n");
            //MultiByteToWideChar(CP_ACP, 0, buffer, bytecount, w_buffer, 65536);

			for(int i=0;i<bytecount;i++)
			{
				if(buffer[i]==START_TRANSMISSION)
				{
					temp=L"";
                    temp_s="";
				}
                else if(buffer[i]==END_TRANSMISSION)
				{
                    std::wstring_convert <std::codecvt_utf8<wchar_t>,wchar_t> convert;
                    std::wstring str = convert.from_bytes((const char*)temp_s.c_str());

                    SendCommandCallback( str, (int)*csock );
				}
                else
                {
					temp += buffer[i];
                    temp_s+=buffer[i];
				}
			}
			
			//strcat(buffer, " SERVER ECHO");
			memset(buffer, 0, buffer_len);

			/*if((bytecount = send(*csock, buffer, strlen(buffer), 0))==SOCKET_ERROR){
				fprintf(stderr, "Error sending data %d\n", WSAGetLastError());
				fprintf(stderr, "Closing socket...");
				free(csock);
				return 0;
			}
	
			printf("Sent bytes %d\n", bytecount);*/
		}
	}
}
