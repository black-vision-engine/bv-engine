#pragma once


#include "Engine/Events/BaseEvent.h"
#include "Serialization/ConversionHelper.h"






namespace bv
{


/**@brief Loading Assets.

@section LoadAssetEvent_Commands Commands

This event supports following commands:
- @ref LoadAssetEvent_Commands_LoadAsset "LoadAsset"

@section LoadAssetEvent_Commands_LoadAsset Loading Assets

Loads @ref Assets "Asset". Set <b>AsyncLoad</b> to true for asynchronous load.
<b>AssetData</b> contains @ref bv::AssetDescriptor "Asset Descriptor" with Asset dependent content.

@code{.json}
{
    "EventID" : "345",
    "Event" : "LoadAssetEvent",
    "SceneName" : "Textures_Example.scn",
    "NodeName" : "#0/#0",
    "PluginName" : "texture",
    "AsyncLoad" : "true",
    "AssetData" :
    {
        "asset" : 
        {
            "filter" : "bilinear",
            "loading_type" : "GENERATE_MIPMAPS",
            "path" : "textures/crystal.jpg",
            "type" : "TEXTURE_ASSET_DESC"
        }
    }
}
@endcode


*/
class LoadAssetEvent : public RemoteEvent
{
public:

    typedef enum
    {
        LoadAsset,
        Fail            ///< Wrong command
    } Command;

private:

    static const EventType          m_sEventType;
    static std::string              m_sEventName;

public:

    std::string                     NodeName;
    std::string                     SceneName;
    std::string                     PluginName;
    IDeserializer *                 AssetData;
    bool                            AsyncLoad;

public:
    explicit                        LoadAssetEvent      () { AssetData = nullptr; }
                                    ~LoadAssetEvent     () { delete AssetData; }

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};


DECLARE_ENUM_SERIALIZATION( LoadAssetEvent::Command )
DEFINE_PTR_TYPE( LoadAssetEvent )


}	// bv

