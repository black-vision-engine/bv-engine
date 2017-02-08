#pragma once

#include "Engine/Events/BaseEvent.h"
#include "Engine/Models/Cameras/CameraModel.h"


namespace bv
{


// *******************************
//
class CameraRemovedEvent : public BaseEvent
{
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;

public:
    explicit        CameraRemovedEvent()
    {}

    model::CameraModelPtr          Camera;

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;

};

DEFINE_PTR_TYPE( CameraRemovedEvent );


}   // bv

