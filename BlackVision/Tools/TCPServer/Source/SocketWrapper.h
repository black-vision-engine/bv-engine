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
  
class RemoteController;


class SocketWrapper : public IEventServer
{
private:
    static RemoteController* BVCommandsConverter;
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

    static void             AddMsg              ( ResponseMsg msg){Responses.push_back( msg );}     ///@deprecated @todo Wywali� jak ju� zniknie RemoteControlInterface

    bool        InitializeServer    ( RemoteController* commandsConverter, int port ) override;
    void        SendResponse        ( ResponseMsg& message ) override;
};

}

