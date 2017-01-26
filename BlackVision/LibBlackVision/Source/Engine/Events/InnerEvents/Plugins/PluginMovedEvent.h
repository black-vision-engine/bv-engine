#pragma once

#include "Engine/Events/BaseEvent.h"
#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Plugins/Plugin.h"


namespace bv
{


// *******************************
//
class PluginMovedEvent : public BaseEvent
{
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;

public:
    explicit        PluginMovedEvent()
    {}

    model::BasicNodePtr         SrcParentNode;
    model::BasicNodePtr         DstParentNode;
    model::BasePluginPtr        Plugin;

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;

};

DEFINE_PTR_TYPE( PluginMovedEvent );


}   // bv


