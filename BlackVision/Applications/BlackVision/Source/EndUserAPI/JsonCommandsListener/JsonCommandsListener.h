#pragma once

#include "../RemoteController.h"
#include "../IEventServer.h"
#include "LogTCP.h"
#include "Threading/CriticalSection.h"

#include <map>

namespace bv
{
class IDeserializer;

class JsonCommandsListener : public RemoteController
{
private:
    IEventServer*       m_eventServer;
    LogTCP*             m_remoteLog;

    std::map<unsigned int, IEventPtr>   m_triggeredEvents;          ///< Events from this map are sent to EventsManager in requested frame.
    CriticalSection                     m_eventsMapLock;
public:
    JsonCommandsListener();
    ~JsonCommandsListener();

    void                QueueEvent          ( const std::wstring& eventString, int socketID ) override;
    bool                InitializeServer    ( int port ) override;
    bool                InitializeRemoteLog ( const std::string& address, unsigned short port, SeverityLevel minLevel = SeverityLevel::debug, int modules = 0xFFFFFFFF ) override;
    void                SendResponse        ( const IEventPtr response ) override;

private:
    void                TryParseEventsGroup ( IDeserializer& deser, int socketID );
    void                TryParseRegularEvent( IDeserializer& deser, int socketID );

    void                AddTriggeredEvent   ( unsigned int requestedFrame, BaseEventPtr& eventPtr );
};

} //bv
