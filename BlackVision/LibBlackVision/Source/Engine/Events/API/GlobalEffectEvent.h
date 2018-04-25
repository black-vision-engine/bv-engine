#pragma once


#include "Engine/Events/BaseEvent.h"
#include "Serialization/ConversionHelper.h"





namespace bv
{



/**@brief Effects manipulation API.

@section GlobalEffectEvent_Commands Commands

This event supports following commands:
- @ref GlobalEffectEvent_Commands_SetGlobalEffect "SetGlobalEffect"
- @ref GlobalEffectEvent_Commands_LoadGlobalEffectAsset "LoadGlobalEffectAsset"

@subsection GlobalEffectEvent_Commands_SetGlobalEffect Setting effect

Sets Node @ref Effects "Effect". Use this function to replace existing Effect.
If you want to remove Effect from Node, add Effect <b>default</b>.

@code{.json}
{
    "EventID" : "10961",
    "Event" : "GlobalEffectEvent",
    "SceneName" : "Shapes2D_Examples.scn",
    "NodeName" : "#0/#0",
    "Command" : "SetGlobalEffect",
    "GlobalEffectName" : "node mask",
    "TimelineName" : "Shapes2D_Examples.scn"
}
@endcode


@subsection GlobalEffectEvent_Commands_LoadGlobalEffectAsset Loading assets

Set asset for Effect.
@note There's no Effect using asset at this moment.

@code{.json}
{
	"Event" : "GlobalEffectEvent",
	"SceneName" : "ImageMask.scn",
	"NodeName" : "Root",
	"GlobalEffectName" : "image mask",
	"Command" : "LoadGlobalEffectAsset",
    "Request" : 
	{
		"assetIdx" : "0"
	},
    "AssetData" : 
	{
		"asset" :
		{
			"filter" : "bilinear",
			"loading_type" : "ORIGINAL_ONLY",
			"path" : "textures/butterfly1.png",
			"type" : "TEXTURE_ASSET_DESC"
		}
	}
}
@endcode

*/
class GlobalEffectEvent : public RemoteEvent
{
public:
    typedef enum
    {
        SetGlobalEffect,
        LoadGlobalEffectAsset,
        Fail            ///< Wrong command
    } Command;
private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;

public:
    GlobalEffectEvent::Command      EffectCommand;
    std::string                     NodePath;
    std::string                     SceneName;
    std::string                     EffectName;
    std::string                     TimelinePath;
    IDeserializer *                 AssetData;
    IDeserializer *                 Request;

public:
    explicit                        GlobalEffectEvent   () {}

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};


DECLARE_ENUM_SERIALIZATION( GlobalEffectEvent::Command )
DEFINE_PTR_TYPE( GlobalEffectEvent )



}	// bv
