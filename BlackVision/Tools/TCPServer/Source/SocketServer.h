#pragma once

#include <map>

#include "SocketConnection.h"
#include "Threading/CriticalSection.h"

#include "Util/IEventServer.h"


namespace bv {

class SocketServer : public IEventServer
{
private:

    QueueEventCallback      m_sendCommandCallback;
    int                     m_port;
    
    static std::map< SOCKET, SocketConnectionPtr >  clientsMap;
    static CriticalSection                          clientsCriticalSection;

public:

                SocketServer            ();
                ~SocketServer           ();

    bool        InitializeServer        ( const QueueEventCallback & callback, int port ) override;
    void        DeinitializeServer      () override;
    void        SendResponse            ( ResponseMsg & message ) override;

    void        WaitForConnection       ();

private:

    void        RemoveUnusedClients     ();

};

} //bv
