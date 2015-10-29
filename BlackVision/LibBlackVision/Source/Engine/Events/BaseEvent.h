#pragma once

#include "Engine/Events/Interfaces/IEvent.h"



namespace bv
{


class BaseEvent : public IEvent
{
private:

    TimeType m_timeStamp;

public:

    explicit        BaseEvent       ( TimeType timeStamp = TimeType( 0.0 ) );

    TimeType        GetTimeStamp    () const;

    virtual void    Serialize       ( ISerializer& ser ) const;
    //static void     Create          ( IDeserializer& deser );

};

} //bv
