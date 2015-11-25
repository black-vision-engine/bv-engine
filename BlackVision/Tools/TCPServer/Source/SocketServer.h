#pragma once

#include "SocketClient.h"
#include "Threading/CriticalSection.h"

#include <map>
#include <memory>


#include "../../Applications/BlackVision/Source/EndUserAPI/IEventServer.h"

namespace bv
{



class SocketServer : public IEventServer
{
private:
    QueueEventCallback      m_sendCommandCallback;
    int                     m_port;
    
    static std::map<SOCKET, SocketClientPtr>    clientsMap;
    static CriticalSection                      clientsCriticalSection;

public:
    SocketServer();
    ~SocketServer();

    bool        InitializeServer        ( const QueueEventCallback& callback, int port ) override;
    void        SendResponse            ( ResponseMsg& message ) override;

    void        WaitForConnection       ();

private:
    void        RemoveUnusedClients     ();
};



} //bv
