#include "Log.h"

vector<LogMsg> Log::Logs;
bool Log::ConsoleOutput;
CRITICAL_SECTION Log::CriticalSection; 
Log::Log(void)
{
    ConsoleOutput = false;
   
}


Log::~Log(void)
{
	 DeleteCriticalSection(&CriticalSection);
}



#pragma comment(lib, "ws2_32.lib") 

#define   SOCKET_ERRNO   WSAGetLastError() 
#define ADDRESS "10.0.0.101" 
#define PORT 28777 


void Log::LogMessage(wstring type,wstring tag,wstring msg)
{
   
    
    LogMsg log(type,tag,msg);
   /* EnterCriticalSection(&CriticalSection);
    Logs.push_back(log);
    LeaveCriticalSection(&CriticalSection);*/
    if(ConsoleOutput)
    {
//        printf("[%s] [%s] - %s\n",type.c_str(),tag.c_str(),msg.c_str());
        //printf("Log: [%hs] [%hs] - %hs\n\n\n",type,tag,msg);
        //std::wcout<<"["<<type<<"]"<<" ["<<tag<<"] - "<<msg<<endl;
//        std::cout<<"["<<string(type.begin(),type.end())<<"]"<<" ["<<string(tag.begin(),tag.end())<<"] - "<<string(msg.begin(),msg.end())<<endl;
        string s_type = string(type.begin(),type.end());
        string s_tag  = string(tag.begin(),tag.end());
        string s_msg  = string(msg.begin(),msg.end());
        printf("-- [%s] [%s] - %s\n",s_type.c_str(),s_tag.c_str(),s_msg.c_str());

    }
}

SOCKET ConnectToPort() 
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
   addr.sin_addr.s_addr = inet_addr(ADDRESS); 
   addr.sin_port = htons (PORT); 

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

void Log::Connect()
{
    InitializeCriticalSectionAndSpinCount(&CriticalSection, 0x00000400);
    CreateThread(0,0,(LPTHREAD_START_ROUTINE)ConnectThread, 0 , 0,0);	
}
void Log::ConnectThread()
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

   for (;;) 
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
               for(;;)
               {
                   Sleep(50);
				  
                   if(!Logs.empty())
                   {
					   EnterCriticalSection(&CriticalSection);
                       
					   LogMsg log = Logs.back();
                       Logs.pop_back();
					   
					   LeaveCriticalSection(&CriticalSection);

                       int msg_size=20+(int)log.tag.size()+(int)log.type.size()+(int)log.msg.size();
                       wchar_t *tmpBuffer = new wchar_t[msg_size];
                       cbBuffer = swprintf(tmpBuffer,msg_size,L"\r\n+log|%ls|%ls|info|%ls\r\n\0",log.tag.c_str(),log.type.c_str(),log.msg.c_str() ); 


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