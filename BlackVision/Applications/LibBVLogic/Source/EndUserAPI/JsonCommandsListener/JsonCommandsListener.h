#pragma once

#include <map>
#include <fstream>
#include <mutex>
#include <atomic>

#include "EndUserAPI/RemoteController.h"
#include "Util/IEventServer.h"
#include "Threading/CriticalSection.h"
#include "Engine/Events/Events.h"


namespace bv
{
class IDeserializer;

class JsonCommandsListener : public RemoteController
{
private:
    IEventServer*       m_eventServer;

    std::map< unsigned int, IEventPtr > m_triggeredEvents;          ///< Events from this map are sent to EventsManager in requested frame.
    CriticalSection                     m_eventsMapLock;

    // Debug layer
    bool                        m_debugLayer;
    std::string                 m_resultDirectory;
    std::ofstream               m_resultFile;
    std::atomic< UInt64 >       m_numEvents;
    std::atomic< UInt64 >       m_numResponses;
    std::mutex                  m_debugFileMutex;

public:

    explicit    JsonCommandsListener        ();
    virtual     ~JsonCommandsListener       ();

    void                QueueEvent          ( const std::string& eventString, int socketID ) override;
    bool                InitializeServer    ( int port ) override;
    void                DeinitializeServer  () override;
    void                SendResponse        ( const IEventPtr response ) override;

    void                InitializeDebugLayer( const std::string & resultPath ) override;
    void                DeinitDebugLayer    ();

private:

    void                DebugLayerProcessResponse   ( ResponseMsg & response );
    void                DebugLayerProcessEvent      ( const std::string & eventString );

    std::string         MakeDebugResultFilePath     ();
	std::string         GetFormattedTime			();

public:

    UInt64              GetDebugEventsCounter       () const override { return m_numEvents; }
    UInt64              GetResponseEventsCounter    () const override { return m_numResponses; }

};

} //bv
