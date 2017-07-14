#pragma once

#include "Engine/Events/BaseEvent.h"

#include "LoadAssetRequest.h"
#include "Assets/Assets.h"


namespace bv
{


// *******************************
//
class AssetAsyncLoadFinishedEvent : public BaseEvent
{
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;

public:
    explicit        AssetAsyncLoadFinishedEvent         ( AssetConstPtr asset, LoadAssetRequest && request )
        :   Asset( asset ),
            Request( std::move( request ) )
    {}

    LoadAssetRequest            Request;
    AssetConstPtr               Asset;


public:

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;

};

DEFINE_PTR_TYPE( AssetAsyncLoadFinishedEvent );


}	// bv


