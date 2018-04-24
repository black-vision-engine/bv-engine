#pragma once

#include "Engine/Events/BaseEvent.h"

namespace bv
{

/**@brief Parameter manipulation API.

@copydoc API_Parameters

@subsection ParamKeyEvent_Commands_AddKey "Adding or updating a key"

Adds a keyframe for Parameter. If keyframe exists at given time, it updates it's value.

This command can be used for all kind of Parameters (plugin's, effect's but also resource's). This corresponds with the following possible Target values:
- PluginParam
- GlobalEffectParam
- ResourceParam
- LightParam
- NodeLogicParam
- CameraParam

Attribute "ParamSubName" applies in two cases: for ResourceParam's and, in general, for Parameters, which are transformations. 
In the first case, it signifies the name of resource (see second example).
In the latter (see third example), it may be one of the following:
- center,
- rotation,
- scale,
- translation.

@code{.json}
{
    "EventID" : "43",
    "Event" : "ParamKeyEvent",
    "Command" : "AddKey",
    "SceneName" : "Shapes2D_Examples.scn",
    "NodeName" : "#0/#1/#0",
    "PluginName" : "rectangle",
    "ParamName" : "height",
    "ParamSubName": "",
    "Time" : "0.0",
    "ParamValue" : "2.2",
    "Target" : "PluginParam"
}
@endcode

@code{.json}
{
	"EventID": "65",
	"Event": "ParamKeyEvent",
	"Command": "AddKey",
	"SceneName": "Textures_Example.scn",
	"NodeName": "#0/#0",
	"PluginName": "texture",
	"ParamName": "filteringMode",
	"ParamSubName": "Tex0",
	"Time": "0.0",
	"ParamValue": "2",
	"Target": "ResourceParam"
}
@endcode

@code{.json}
{
	"EventID": "97",
	"Event": "ParamKeyEvent",
	"Command": "AddKey",
	"SceneName": "Shapes2D_Examples.scn",
	"NodeName": "#0/#1",
	"PluginName": "transform",
	"ParamName": "simple_transform",
	"ParamSubName": "translation",
	"Time": "0.0",
	"ParamValue": "0, -1, 0",
	"Target": "PluginParam"
}
@endcode

@subsection ParamKeyEvent_Commands_RemoveKey "Removing a key"
@subsection ParamKeyEvent_Commands_MoveKey "Moving a key"
@subsection ParamKeyEvent_Commands_SetInterpolatorType "Setting an interpolator type of whole curve"
@subsection ParamKeyEvent_Commands_SetAddedInterpolatorType "Setting next added interpolator type"
@subsection ParamKeyEvent_Commands_SetInterpolatorPreWrapMethod "Setting interpolator's pre-wrap method"
@subsection ParamKeyEvent_Commands_SetInterpolatorPostWrapMethod "Setting interpolator's post-wrap method"
@subsection ParamKeyEvent_Commands_AssignTimeline "Assigning timeline to a parameter"
@subsection ParamKeyEvent_Commands_SampleCurve "Evaluating parameter"
@subsection ParamKeyEvent_Commands_ListKeys "Listing all curve's keyframes"
@subsection ParamKeyEvent_Commands_ListParameters "Listing parameters"

*/
class ParamKeyEvent : public RemoteEvent
{
public:
    typedef enum
    {
        AddKey,
        RemoveKey,
        MoveKey,
        SetInterpolatorType,
        SetAddedInterpolatorType,
        SetInterpolatorPreWrapMethod,
        SetInterpolatorPostWrapMethod,
        AssignTimeline,
        SampleCurve,
        ListKeys,
        ListParameters,
        Fail            ///< Wrong command
    } Command;


private:
    static const EventType      m_sEventType;
    static std::string          m_sEventName;
public:

    ParameterAddress                ParamAddress;
    std::string                     Value;

    float                           Time;
    ParamKeyEvent::Command          ParamCommand;

public:
    virtual void                    Serialize( ISerializer& ser ) const;
    static IEventPtr                Create( IDeserializer& deser );
    virtual IEventPtr               Clone() const;


    virtual EventType               GetEventType() const;
    virtual const std::string &     GetName() const;

    static EventType                Type();
    static std::string&             Name();
};

DECLARE_ENUM_SERIALIZATION( ParamKeyEvent::Command )
DEFINE_PTR_TYPE( ParamKeyEvent )

}