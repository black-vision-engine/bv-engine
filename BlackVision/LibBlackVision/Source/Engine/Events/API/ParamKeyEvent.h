#pragma once

#include "Engine/Events/BaseEvent.h"

namespace bv
{

/**@brief Parameter manipulation API.

@section ParamKeyEvent_Commands Commands

This event supports following commands:
- @ref ParamKeyEvent_Commands_AddKey "AddKey"
- @ref ParamKeyEvent_Commands_RemoveKey "RemoveKey"
- @ref ParamKeyEvent_Commands_MoveKey "MoveKey"
- @ref ParamKeyEvent_Commands_SetInterpolatorType "SetInterpolatorType"
- @ref ParamKeyEvent_Commands_SetAddedInterpolatorType "SettAddedInterpolatorType"
- @ref ParamKeyEvent_Commands_SetInterpolatorPreWrapMethod "SetInterpolatorPreWrapMethod"
- @ref ParamKeyEvent_Commands_SetInterpolatorPostWrapMethod "SetInterpolatorPostWrapMethod"
- @ref ParamKeyEvent_Commands_AssignTimeline "AssignTimeline"
- @ref ParamKeyEvent_Commands_SampleCurve "SampleCurve"
- @ref ParamKeyEvent_Commands_ListKeys "ListKeys"
- @ref ParamKeyEvent_Commands_ListParameters "ListParameters"

@subsection ParamKeyEvent_Commands_AddKey Adding or updating a key

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

@code{.json}
{
	"EventID": "6010",
	"Event": "ParamKeyEvent",
	"Command": "RemoveKey",
	"SceneName": "TimelineKeyframes_Example.scn",
	"NodeName": "#0/#0",
	"PluginName": "transform",
	"ParamName": "simple_transform",
	"ParamSubName": "translation",
	"Time": "3",
	"Target": "PluginParam"
}
@endcode

@subsection ParamKeyEvent_Commands_MoveKey "Moving a key"

Note that new time is sent as ParamValue attribute.

@code{.json}
{
	"EventID": "6010",
	"Event": "ParamKeyEvent",
	"Command": "MoveKey",
	"SceneName": "TimelineKeyframes_Example.scn",
	"NodeName": "#0/#0",
	"PluginName": "transform",
	"ParamName": "simple_transform",
	"ParamSubName": "translation",
	"Time": "3",
    "ParamValue": "5",
    "Target": "PluginParam"
}
@endcode

@subsection ParamKeyEvent_Commands_SetInterpolatorType "Setting an interpolator type of whole curve"

Possible interpolator types:
- bezier,
- cosine,
- linear,
- point,
- cubic_in
- cubic_out,
- elastic_in,
- elastic_out,
- elastic_in_bounce,
- elastic_out_bounce,
- quadric_inout,
- cubic_inout.

@code{.json}
{
	"EventID": "6294",
	"Event": "ParamKeyEvent",
	"Command": "SetInterpolator",
	"SceneName": "TimelineKeyframes_Example.scn",
	"NodeName": "#0/#0",
	"PluginName": "transform",
	"ParamName": "simple_transform",
	"ParamValue": "elastic_in_bounce"
}
@endcode

@subsection ParamKeyEvent_Commands_SetAddedInterpolatorType "Setting next added interpolator type"

For the list of types see @ref ParamKeyEvent_Commands_SetInterpolatorType.

@code{.json}
{
	"EventID": "6294",
	"Event": "ParamKeyEvent",
	"Command": "SetAddedInterpolatorType",
	"SceneName": "TimelineKeyframes_Example.scn",
	"NodeName": "#0/#0",
	"PluginName": "transform",
	"ParamName": "simple_transform",
	"ParamValue": "elastic_in_bounce"
}
@endcode

@subsection ParamKeyEvent_Commands_SetInterpolatorPreWrapMethod "Setting interpolator's pre-wrap method"

Possible pre-wrap methods:
- clamp
- pingPong
- repeat

@code{.json}
"EventID": "6294",
"Event": "ParamKeyEvent",
"Command": "SetInterpolatorPreWrapMethod",
"SceneName": "TimelineKeyframes_Example.scn",
"NodeName": "#0/#0",
"PluginName": "transform",
"ParamName": "simple_transform",
"ParamValue": "mirror"
@endcode

@subsection ParamKeyEvent_Commands_SetInterpolatorPostWrapMethod "Setting interpolator's post-wrap method"

For the list of methods see @ref ParamKeyEvent_Commands_SetInterpolatorPreWrapMethod.

@code{.json}
"EventID": "6294",
"Event": "ParamKeyEvent",
"Command": "SetInterpolatorPostWrapMethod",
"SceneName": "TimelineKeyframes_Example.scn",
"NodeName": "#0/#0",
"PluginName": "transform",
"ParamName": "simple_transform",
"ParamValue": "mirror"
@endcode

@subsection ParamKeyEvent_Commands_AssignTimeline "Assigning timeline to a parameter"

@code{.json}
{
	"EventID": "40788",
	"Event": "ParamKeyEvent",
	"Command": "AssignTimeline",
	"SceneName": "TimelineKeyframes_Example.scn",
	"NodeName": "#0/#0",
	"PluginName": "transform",
	"ParamName": "simple_transform",
	"Target": "PluginParam",
	"ParamValue": "TimelineKeyframes_Example.scn%default"
}
@endcode

@subsection ParamKeyEvent_Commands_SampleCurve "Evaluating parameter"

ParamValue attribute should be of form "start,end,steps" where start and end define sampling interval and steps is number of samples.

The result is returned in an array "samples" (see example response below).

@code{.json}
{
	"EventID": "40788",
	"Event": "ParamKeyEvent",
	"Command": "SampleCurve",
	"SceneName": "TimelineKeyframes_Example.scn",
	"NodeName": "#0/#0",
	"PluginName": "transform",
	"ParamName": "simple_transform",
	"Target": "PluginParam",
	"ParamValue": "0,3,10"
}
@endcode

Response:

@code{.json}
{
	"samples": [{
			"t": "0.000000",
			"val": ""
		}, {
			"t": "0.300000",
			"val": ""
		}, {
			"t": "0.600000",
			"val": ""
		}, {
			"t": "0.900000",
			"val": ""
		}, {
			"t": "1.200000",
			"val": ""
		}, {
			"t": "1.500000",
			"val": ""
		}, {
			"t": "1.800000",
			"val": ""
		}, {
			"t": "2.100000",
			"val": ""
		}, {
			"t": "2.400000",
			"val": ""
		}, {
			"t": "2.700000",
			"val": ""
		}
	]
}
@endcode


@subsection ParamKeyEvent_Commands_ListKeys "Listing all curve's keyframes"

@code{.json}
{
	"EventID": "40788",
	"Event": "ParamKeyEvent",
	"Command": "ListKeys",
	"SceneName": "TimelineKeyframes_Example.scn",
	"NodeName": "#0/#0",
	"PluginName": "transform",
	"ParamName": "simple_transform",
	"ParamSubName": "translation",
    "Target": "PluginParam"
}
@endcode

@subsection ParamKeyEvent_Commands_ListParameters "Listing parameters"

@code{.json}
{
	"EventID": "40788",
	"Event": "ParamKeyEvent",
	"Command": "ListParameters",
	"SceneName": "TimelineKeyframes_Example.scn",
	"NodeName": "#0/#0",
	"PluginName": "transform",
    "Target": "PluginParam"
}
@endcode


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