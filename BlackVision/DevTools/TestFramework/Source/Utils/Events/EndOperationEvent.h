#pragma once


#include "Engine/Events/BaseEvent.h"



namespace bv
{



// ***********************
/// Event which can be used to notify that an operation is ended.
class EndOperationEvent : public BaseEvent
{
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;

public:
    explicit        EndOperationEvent()
    {}


    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;

};

DEFINE_PTR_TYPE( EndOperationEvent );

}	// bv

