#pragma once

#include "Engine/Events/Events.h"

#include <unordered_map>

namespace bv
{

typedef IEventPtr (*EventCreatorDelegate)( IDeserializer& deser );

class RemoteCommandsListener
{
private:
    std::unordered_map<std::wstring, EventCreatorDelegate>      m_eventsConverter;
public:
    RemoteCommandsListener();
    virtual ~RemoteCommandsListener() {};

    virtual void                        QueueEvent          ( const std::wstring& eventString, int socketID ) = 0;
    virtual bool                        InitializeServer    ( int port ) = 0;
    virtual void                        SendResponse        ( const IEventPtr response ) = 0;

    static RemoteCommandsListener&      Get();
protected:
    IEventPtr                           CreateEvent         ( IDeserializer& deser );
    void                                RegisterEvent       ( const std::string& eventName, EventCreatorDelegate eventCreator );
};


} //bv
