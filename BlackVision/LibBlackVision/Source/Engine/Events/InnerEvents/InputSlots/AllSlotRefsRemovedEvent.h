#pragma once


#include "SlotEventBase.h"


namespace bv
{


// *******************************
/// All references to slot have been removed. Input slot source provider can optimize itself and stop producing input.
class AllSlotRefsRemovedEvent : public SlotEventBase
{
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;

public:
    explicit        AllSlotRefsRemovedEvent( SlotIndex idx, const std::string & name )
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

DEFINE_PTR_TYPE( AllSlotRefsRemovedEvent );


}   // bv


