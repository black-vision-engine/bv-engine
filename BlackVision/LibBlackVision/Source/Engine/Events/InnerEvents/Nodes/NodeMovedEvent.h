#pragma once

#include "Engine/Events/BaseEvent.h"
#include "Engine/Models/BasicNode.h"


namespace bv
{


// *******************************
//
class NodeMovedEvent : public BaseEvent
{
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;

public:
    explicit        NodeMovedEvent()
    {}

    model::BasicNodePtr         Node;
    model::BasicNodePtr         SrcParentNode;
    model::BasicNodePtr         DstParentNode;


    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;

};

DEFINE_PTR_TYPE( NodeMovedEvent );


}   // bv

