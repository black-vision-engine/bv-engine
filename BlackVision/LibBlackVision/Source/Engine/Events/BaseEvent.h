#pragma once

#include "Engine/Events/Interfaces/IEvent.h"



namespace bv
{


class BaseEvent : public IEvent
{
private:
    TimeType        m_timeStamp;
public:
    explicit        BaseEvent       ( TimeType timeStamp = TimeType( 0.0 ) );

    TimeType        GetTimeStamp    () const;

    virtual void    Serialize       ( ISerializer& ser ) const;
    //static void     Create          ( IDeserializer& deser );

};

DEFINE_PTR_TYPE( BaseEvent );

class RemoteEvent : public BaseEvent
{
public:
    int             SocketID;       ///< Client which sent event.
    unsigned int    EventID;        ///< This field helps editor to find out, which response is to which event.
public:
    explicit        RemoteEvent     ()
        : EventID( 0 ) {}
};

DEFINE_PTR_TYPE( RemoteEvent );

} //bv
