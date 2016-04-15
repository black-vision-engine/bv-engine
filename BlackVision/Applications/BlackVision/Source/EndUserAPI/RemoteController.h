#pragma once

#include "Engine/Events/EventFactory.h"
#include "Engine/Events/Interfaces/IEvent.h"
#include "Tools/Logger/LoggerEnums.h"

#include <unordered_map>

namespace bv
{

class RemoteController
{
private:

    std::unordered_map< std::string, EventCreatorDelegate >      m_eventsConverter;

public:

    RemoteController() {};
    virtual ~RemoteController() {};

    virtual void                        QueueEvent          ( const std::string& eventString, int socketID ) = 0;
    virtual bool                        InitializeServer    ( int port ) = 0;
    virtual void                        DeinitializeServer  () = 0;
    virtual void                        SendResponse        ( const IEventPtr response ) = 0;

protected:

    IEventPtr                           DeserializeEvent    ( IDeserializer& deser );
    void                                RegisterEvent       ( const std::string& eventName, EventCreatorDelegate eventCreator );
};


} //bv
