#pragma warning(disable :4996)
#include <codecvt>
#include<locale>
#include<boost/lexical_cast.hpp>
#include "SocketWrapper.h"
#include <math.h>


#include "Engine/Events/Interfaces/IEventManager.h"
#include "Engine/Events/Events.h"

#include "Log.h"


#include <iostream>
#pragma comment(lib, "Ws2_32.lib")

namespace bv{
	

	//DWORD WINAPI SocketHandler(void*);
    vector<ResponseMsg> SocketWrapper::Responses;
	SocketWrapper::SocketWrapper(void)
	{
	    Socket = this;
	}


	SocketWrapper::~SocketWrapper(void)
	{
	}


	bool SocketWrapper::InitServer()
	{

		CreateThread(0,0,(LPTHREAD_START_ROUTINE)SocketInitHandler, this , 0,0);	
		return true;
	}


    bool SocketWrapper::thread_clients(void)
	{
//The port you want the server to listen on
		int host_port= 11101;

		
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
                Log::A("Connection","Client connected: "+string(inet_ntoa(sadr.sin_addr)));
				CreateThread(0,0,SocketHandler, (void*)csock , 0,0);
			}
			else{
				fprintf(stderr, "Error accepting %d\n",WSAGetLastError());
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

	void SocketWrapper::ParseCmd(std::wstring cmd)
	{
       //CmdBase b = ProtocolManager::ParseCmd(cmd);
       
        /*
		std::wcout<<"Parsed: "<<cmd.c_str()<<std::endl;
        // kebab
        if(cmd.find(L"x:")!=std::string::npos)
        {
            cmd = cmd.substr(2,cmd.length()-2);
            std::wcout<<"Parsed2: "<<cmd.c_str()<<std::endl;

            TransformSetEventPtr  GTransformSetEvent = TransformSetEventPtr( new TransformSetEvent() );;
            double tx = boost::lexical_cast<double>(cmd);
            glm::vec3 kebab( tx, 0.f, 0.f );
            glm::vec3 kebab2( tx, -tx, 1.f );
    
            //gowno
            GTransformSetEvent->SetTranslation( kebab );
            //GTransformSetEvent->SetScale( kebab2 );
    
            GetDefaultEventManager().QueueEvent( GTransformSetEvent );
            
        }else if(cmd.find(L"t:")!=std::string::npos)
        {
             cmd = cmd.substr(2,cmd.length()-2);
            std::wcout<<"Parsed2: "<<cmd.c_str()<<std::endl;

            std::vector<std::wstring> V;
			V = split(cmd,'|',V);
            std::wcout<<"after split"<<std::endl;
            if(V.size()!=6)return;
            double tx = boost::lexical_cast<double>(V[0]);
            std::wcout<<"after 1"<<std::endl;
            double ty = boost::lexical_cast<double>(V[1]);
            std::wcout<<"after 2"<<std::endl;
            double tz = boost::lexical_cast<double>(V[2]);
            std::wcout<<"after 3"<<std::endl;
            double sx = boost::lexical_cast<double>(V[3]);
            std::wcout<<"after 4"<<std::endl;
            double sy = boost::lexical_cast<double>(V[4]);
            std::wcout<<"after 5"<<std::endl;
            double sz = boost::lexical_cast<double>(V[5]);
            std::wcout<<"after 6"<<std::endl;
            std::wcout<<"after cast"<<std::endl;
            TransformSetEventPtr  GTransformSetEvent = TransformSetEventPtr( new TransformSetEvent() );



            glm::vec3 kebab( tx, ty, tz );
            glm::vec3 kebab2( sx, sy, sz );
            std::wcout<<"after kebab"<<std::endl;
            
            GTransformSetEvent->SetTranslation( kebab );
            GTransformSetEvent->SetScale( kebab2 );
            std::wcout<<"after scale"<<std::endl;
            
            GetDefaultEventManager().QueueEvent( GTransformSetEvent );


        }else{
             std::wcout<<"fuck u : ( "<<std::endl;
        }
        */
	}

    /**
 * Convert a Windows wide string to a UTF-8 (multi-byte) string.
 */


	DWORD SocketWrapper::SocketHandler( void * lp )
	{
		SOCKET *csock = (SOCKET*)lp;

		char buffer[1024];
        //wchar_t w_buffer[1024];
		int buffer_len = 10;
		int bytecount;

		//char ParseBuffer[2048];

		memset(buffer, 0, buffer_len);
		std::wstring temp;
        std::string temp_s;
		std::wstring cmd;

		const char START_TRANSMISSION = 0x02;
		const char END_TRANSMISSION = 0x03;

        ProtocolManager pm;
       
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

                    if( Responses[i].sock_id == (int) *csock && Responses[i].sent == false )
                    {
                        printf("Sock id %d zgadza siê!\n",*csock);
                        
                        printf("Sock id %d zgadza siê!\n",*csock);
						wchar_t CHAR_BEGIN = 0x02;
						wchar_t CHAR_END = 0x03;
                        wstring to_send = CHAR_BEGIN+Responses[i].msg+CHAR_END;
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
				}else if(buffer[i]==END_TRANSMISSION)
				{
					//json parse
					//ParseCmd(temp);
                    
                    std::wstring_convert <std::codecvt_utf8<wchar_t>,wchar_t> convert;
                    wstring str = convert.from_bytes((const char*)temp_s.c_str());


                    pm.ParseCmd(str);
                   
                    for(unsigned int i=0;i<pm.cmdStack.size();i++)
                    {
                        wcout<<"command :"<<pm.cmdStack[i]->CmdName<<"parsed!"<<endl;
                        if(pm.cmdStack[i]->Type==CmdType::VIDEOCARD_INFO)
                        {
                             CmdVideoCardInfo *cmd = (CmdVideoCardInfo*)(pm.cmdStack[i]);
                             VideoCardEventPtr  ev = VideoCardEventPtr( new VideoCardEvent() );

                             ev->SetData(cmd->query);
							 
 
                             GetDefaultEventManager().ConcurrentQueueEvent( ev );
                             wcout<<"event  :"<<cmd->query<<" added!"<<endl;
                        }else if(pm.cmdStack[i]->Type==CmdType::SET_PARAM)
                        {
                            CmdSetParam *cmd = (CmdSetParam*)(pm.cmdStack[i]);
                            SetParamEventPtr ev = SetParamEventPtr(new SetParamEvent() );
                            ev->NodeName = cmd->path;
                            ev->PluginName = cmd->plugin;
                            ev->ParamName = cmd->param;
                            ev->Value = cmd->value;
                            ev->time = cmd->time;
							ev->x = cmd->x;
                            ev->y = cmd->y;
                            ev->z = cmd->z;

                            GetDefaultEventManager().ConcurrentQueueEvent( ev );
                        }else if(pm.cmdStack[i]->Type==CmdType::GET_INFO)
                        {
                             CmdGetInfo *cmd = (CmdGetInfo*)(pm.cmdStack[i]);
                             InfoEventPtr  ev = InfoEventPtr( new InfoEvent() );

                             ev->request = cmd->request;
							 ev->NodeName = cmd->path;
							 ev->SetAddStrData(cmd->param);
                             ev->sock_id = (int)*csock;
 
                             GetDefaultEventManager().ConcurrentQueueEvent( ev );
                        }else if(pm.cmdStack[i]->Type==CmdType::SCENE_STRUCTURE)
                        {
                             CmdStruct *cmd = (CmdStruct*)(pm.cmdStack[i]);
                             SceneStructureEventPtr  ev = SceneStructureEventPtr( new SceneStructureEvent() );

                             ev->command = cmd->Command;
							 ev->NodeName = cmd->NodeParam1;
							 ev->NodeName2 = cmd->NodeParam2;
                             ev->request = cmd->Request;
							 
                             ev->sock_id = (int)*csock;
 
                             GetDefaultEventManager().ConcurrentQueueEvent( ev );
                        }else if(pm.cmdStack[i]->Type==CmdType::TIMELINE)
                        {
                            CmdTimeline *cmd = (CmdTimeline*)(pm.cmdStack[i]);
                            TimeLineCmdPtr ev = TimeLineCmdPtr(new TimeLineCmd() );
                            ev->cmd = cmd->command;
                            ev->value = cmd->value;
                            ev->TimelineName = cmd->timeline_name;
                            
                            GetDefaultEventManager().ConcurrentQueueEvent( ev );
                        }
                        else if(pm.cmdStack[i]->Type==CmdType::TIMER)
                        {
                            CmdTimer *cmd = (CmdTimer*)(pm.cmdStack[i]);
                            TimerCmdPtr ev = TimerCmdPtr(new TimerCmd() );
                            ev->cmd = cmd->command;
                            
                            ev->NodeName = cmd->NodeName;
                            ev->H = cmd->H;
                            ev->M = cmd->M;
                            ev->S = cmd->S;
                            ev->MS = cmd->MS;
                            
                            GetDefaultEventManager().ConcurrentQueueEvent( ev );
                        }else if(pm.cmdStack[i]->Type==CmdType::WIDGET)
                        {
                            CmdWidget *cmd = (CmdWidget*)(pm.cmdStack[i]);
                            WidgetCmdPtr ev = WidgetCmdPtr(new WidgetCmd() );
							
                            
                            ev->NodeName = cmd->NodePath;
							ev->WidgetName = cmd->WidgetName;
							ev->Action = cmd->Action;
							ev->Param = cmd->Param;
							ev->Time = cmd->Time;
							ev->Value = cmd->Value;

                            GetDefaultEventManager().ConcurrentQueueEvent( ev );
                        }

                    }


                    pm.cmdStack.clear();


            

				}else{
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
