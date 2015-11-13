#pragma once
#include <iostream>
#include "win_sock.h"
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <vector>

#include "../../Applications/BlackVision/Source/EndUserAPI/IEventServer.h"


//#include "SocketHandle.h"
//#include "tcp/SocketServer.h"


namespace bv
{
  
class RemoteCommandsListener;


class SocketWrapper : public IEventServer
{
private:
    static RemoteCommandsListener* BVCommandsConverter;
    static std::vector<ResponseMsg> Responses;

	int m_nLinkMode;
    int m_port;

private:
    bool thread_clients();
public:
        
	SocketWrapper();
	~SocketWrapper(void);


	bool                    InitServer          ();
	static DWORD WINAPI     SocketHandler       (void*);
	static DWORD WINAPI     SocketInitHandler   (void*);

    static void             AddMsg              ( ResponseMsg msg){Responses.push_back( msg );}     ///@deprecated @todo Wywaliæ jak ju¿ zniknie RemoteControlInterface

    bool        InitializeServer    ( RemoteCommandsListener* commandsConverter, int port ) override;
    void        SendResponse        ( ResponseMsg& message ) override;
};

}

