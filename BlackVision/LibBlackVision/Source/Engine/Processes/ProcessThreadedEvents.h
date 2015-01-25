#pragma once

#include "Engine/Events/Interfaces/IEvent.h"
#include "Engine/Events/Queues/EventQueueConcurrent.h"

#include "Engine/Processes/ProcessThreaded.h"


namespace bv
{

class IEventManager;

// ******************************************************
class ProcessThreadedEventSender : public ProcessThreaded
{
private:

    bool                m_quit;

    IEventManager *     m_eventManager;

public:

	ProcessThreadedEventSender  ( IEventManager * eventManager );

    void             Stop() { m_quit = true; }

protected:

    virtual void Run            ();

};


// ******************************************************
class ProcessThreadedEventReceiver : public ProcessThreaded
{
private:

    bool                    m_quit;

	EventQueueConcurrent    m_eventCache;

public:

	ProcessThreadedEventReceiver    ( IEventManager * eventManager );

    void DelegateFun                ( IEventPtr evt );

    void             Stop() { m_quit = true; }

protected:

    virtual void Run                ();

};

} //bv
