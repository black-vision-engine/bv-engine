#pragma once

#include "../RemoteCommandsListener.h"
#include "../IEventServer.h"

namespace bv
{
class IDeserializer;

class JsonCommandsListener : public RemoteCommandsListener
{
private:
    IEventServer*       m_eventServer;
public:
    JsonCommandsListener();
    ~JsonCommandsListener();

    void                QueueEvent          ( const std::wstring& eventString, int socketID ) override;
    bool                InitializeServer    ( int port ) override;
    void                SendResponse        ( const IEventPtr response ) override;
};

} //bv
