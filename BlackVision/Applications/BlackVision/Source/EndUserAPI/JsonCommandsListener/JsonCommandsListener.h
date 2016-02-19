#pragma once

#include "../RemoteController.h"
#include "../IEventServer.h"
#include "Threading/CriticalSection.h"
#include "Engine/Events/Events.h"

#include <map>

namespace bv
{
class IDeserializer;

class JsonCommandsListener : public RemoteController
{
private:
    IEventServer*       m_eventServer;

    std::map<unsigned int, IEventPtr>   m_triggeredEvents;          ///< Events from this map are sent to EventsManager in requested frame.
    CriticalSection                     m_eventsMapLock;
public:
    JsonCommandsListener();
    ~JsonCommandsListener();

    void                QueueEvent          ( const std::string& eventString, int socketID ) override;
    bool                InitializeServer    ( int port ) override;
    void                DeinitializeServer  () override;
    void                SendResponse        ( const IEventPtr response ) override;

private:
    void                TryParseRegularEvent( IDeserializer& deser, int socketID );
    void                AddTriggeredEvent   ( unsigned int requestedFrame, RemoteEventPtr& eventPtr );
};

} //bv
