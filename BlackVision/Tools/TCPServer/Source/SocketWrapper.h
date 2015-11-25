#pragma once
#include <iostream>
#include "win_sock.h"
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <vector>


#include "DataTypes/QueueConcurrent.h"

#include "../../Applications/BlackVision/Source/EndUserAPI/IEventServer.h"


//#include "SocketHandle.h"
//#include "tcp/SocketServer.h"


namespace bv
{

struct InitData
{
    unsigned int    SeverityLevel;
    unsigned int    LogModules;
};

class SocketWrapper : public IEventServer
{
private:
    static QueueEventCallback           SendCommandCallback;
    static std::vector<ResponseMsg>     Responses;

    std::vector<QueueConcurrent<ResponseMsg> >        m_responseQueues;

    int m_port;

private:
    bool thread_clients();
public:
        
	SocketWrapper();
	~SocketWrapper(void);


	bool                    InitServer          ();
	static DWORD WINAPI     SocketHandler       (void*);
	static DWORD WINAPI     SocketInitHandler   (void*);

    static InitData         InitCommunication   ( SOCKET socketID );
    static bool             Authorization       ( SOCKET socketID );

    bool        InitializeServer        ( const QueueEventCallback& callback, int port ) override;
    void        SendResponse            ( ResponseMsg& message ) override;
};

}

