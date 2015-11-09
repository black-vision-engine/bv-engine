#pragma once
#include <iostream>
#include "win_sock.h"
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include "JsonWrapper\ProtocolManager.h"
#include "../../Applications/BlackVision/Source/EndUserAPI/IEventServer.h"
#include "../../Applications/BlackVision/Source/EndUserAPI/RemoteCommandsConverter.h"

//#include "SocketHandle.h"
//#include "tcp/SocketServer.h"


namespace bv{
  
//class RemoteCommandsConverter;

class ResponseMsg{
public: 
    wstring msg;
    int sock_id;
    bool sent;
    ResponseMsg(){sent=false;}

};

class SocketWrapper : public IEventServer
{
private:
    static RemoteCommandsConverter* BVCommandsConverter;

	int m_nLinkMode;
    static ProtocolManager protocolManager;
    static vector<ResponseMsg> Responses;
    SocketWrapper *Socket;
    bool thread_clients();
public:
        
	SocketWrapper();
	~SocketWrapper(void);
	bool InitServer();
    static void ParseCmd(std::wstring cmd);
	static DWORD WINAPI SocketHandler(void*);
	static DWORD WINAPI SocketInitHandler(void*);
    static void AddMsg(ResponseMsg msg){Responses.push_back(msg);}

    bool        InitializeServer( RemoteCommandsConverter* commandsConverter ) override;
};

}

