#pragma once

#include "../RemoteController.h"
#include "../IEventServer.h"
#include "LogTCP.h"

namespace bv
{
class IDeserializer;

class JsonCommandsListener : public RemoteController
{
private:
    IEventServer*       m_eventServer;
    LogTCP*             m_remoteLog;
public:
    JsonCommandsListener();
    ~JsonCommandsListener();

    void                QueueEvent          ( const std::wstring& eventString, int socketID ) override;
    bool                InitializeServer    ( int port ) override;
    bool                InitializeRemoteLog ( const std::string& address, unsigned short port, SeverityLevel minLevel = SeverityLevel::debug, int modules = 0xFFFFFFFF ) override;
    void                SendResponse        ( const IEventPtr response ) override;
};

} //bv
