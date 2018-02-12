#pragma once


#include "SlotEventBase.h"


namespace bv
{


// *******************************
/// Slot
class FirstSlotRefEvent : public SlotEventBase
{
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;

public:
    explicit        FirstSlotRefEvent( SlotIndex idx, const std::string & name )
        : SlotEventBase( idx, name )
    {}



    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;

};

DEFINE_PTR_TYPE( FirstSlotRefEvent );


}   // bv


