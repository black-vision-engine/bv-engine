#pragma once

#include "Engine/Events/BaseEvent.h"
#include "Serialization/ConversionHelper.h"




namespace bv
{



/**@brief Returns engine state informations.

@section InfoEvent_Commands Commands

- @ref InfoEvent_Commands_TreeStructure "TreeStructure"
- @ref InfoEvent_Commands_TreeStructure "MinimalTreeStructure"
- @ref InfoEvent_Commands_NodeInfo "NodeInfo"
- @ref InfoEvent_Commands_PluginInfo "PluginInfo"
- EffectInfo
- ParamInfo
- @ref InfoEvent_Commands_LogicInfo "LogicInfo"
- @ref InfoEvent_Commands_TreeStructure "MinimalSceneInfo"
- @ref InfoEvent_Commands_LightsInfo "LightsInfo"
- @ref InfoEvent_CamerasInfo_ExampleResponse "CamerasInfo"
- ListParamDescriptors
- GetParamDescriptor
- ListSceneAssets
- ListProjectNames
- ListScenes
- ListPresets
- ListAssetsPaths
- @ref InfoEvent_Commands_ListCategoriesNames "ListCategoriesNames"
- ListProjects
- ListAllScenes
- ListAllFolders
- ListResourcesInFolders
- ListAllResources
- GetAssetDescriptor
- GetAssetThumbnail
- GetSceneThumbnail
- GetPresetThumbnail
- GetPMItemStats
- Timelines
- CheckTimelineTime
- ListTimelineKeyframes
- ListTimelinesParams
- Performance
- Videocards

@subsection InfoEvent_Commands_TreeStructure TreeStructure

- <b>MinimalTreeStructure</b> queries main informations about all loaded @ref Scenes "Scenes" in engine.
- <b>TreeStructure</b> command queries full serialized tree structure contatining all information about @ref Scenes "Scenes".
- <b>MinimalSceneInfo</b> command queries the same info as MinimalTreeStructure, but only for single scene.

Query <b>MinimalTreeStructure</b>:
@code{.json}
{
    "Event" : "InfoEvent",
    "EventID" : "3024",
    "Command" : "MinimalTreeStructure"
}
@endcode

@ref InfoEvent_MinimalTreeStructure_ExampleResponse "Example Response"

Query <b>TreeStructure</b>:
@code{.json}
{
    "Event" : "InfoEvent",
    "EventID" : "3024",
    "Command" : "TreeStructure"
}
@endcode

@ref InfoEvent_TreeStructure_ExampleResponse "Example Response"

Query <b>MinimalSceneInfo</b>:
@code{.json}
{
    "Event" : "ProjectEvent",
    "EventID" : "2",
    "Command" : "MinimalSceneInfo",
    "Request" :
    {
        "sceneName" : "Shapes2D_Examples.scn"
    }
}
@endcode

@ref InforEvent_MinimalSceneStructure_ExampleResponse "Example Response"

@subsection InfoEvent_Commands_NodeInfo NodeInfo

Gets single Node full information.

@code{.json}
{
    "EventID" : "10713",
    "Event" : "InfoEvent",
    "Command" : "NodeInfo",
    "Request" : 
    {
        "SceneName" : "Shapes2D_Examples.scn",
        "NodePath" : "#0"
    }
}
@endcode

@ref InfoEvent_NodeInfo_ExampleResponse "Example Response"

@subsection InfoEvent_Commands_CamerasInfo CamerasInfo

Gets information about cameras in Scene. Returns array of all cameras with serialized
parameters and index of current Camera.

@code{.json}
{
    "EventID" : "10713",
    "Event" : "InfoEvent",
    "Command" : "CamerasInfo",
    "Request" : 
    {
        "SceneName" : "Shapes2D_Examples.scn"
    }
}
@endcode

@ref InfoEvent_CamerasInfo_ExampleResponse "Example Response"

@subsection InfoEvent_Commands_LightsInfo LightsInfo

Gets information about all Lights in @ref Scenes "Scene".

@code{.json}
{
    "EventID" : "10713",
    "Event" : "InfoEvent",
    "Command" : "LightsInfo",
    "Request" : 
    {
        "SceneName" : "Shapes2D_Examples.scn"
    }
}
@endcode

@ref InfoEvent_LightsInfo_ExampleResponse "Example Response"

@subsection InfoEvent_Commands_LogicInfo LogicInfo

Gets information about logic.

@code{.json}
{
    "EventID" : "10713",
    "Event" : "InfoEvent",
    "Command" : "LogicInfo",
    "Request" : 
    {
        "SceneName" : "Shapes2D_Examples.scn",
        "NodeName" : "#0/#0"
    }
}
@endcode

@ref InfoEvent_LogicInfo_ExampleResponse "Example Response"

@subsection InfoEvent_Commands_PluginInfo PluginInfo

Gets information about single Plugin.

@code{.json}
{
	"Event" : "InfoEvent",
	"EventID" : "1",
	"Command" : "PluginInfo",
	"Request" : 
	{
		"SceneName" : "Shapes2D_Examples.scn",
		"NodePath" : "#0/#0/#0",
		"PluginName" : "circle"
	}
}
@endcode

@ref InfoEvent_PluginInfo_ExampleResponse "Example Response"

@subsection InfoEvent_Commands_EffectInfo EffectInfo

Gets information about @ref Effects "Effect".

@code{.json}
{
	"Event" : "InfoEvent",
	"EventID" : "1",
	"Command" : "EffectInfo",
	"Request" : 
	{
		"SceneName" : "BluredShapes.scn",
		"NodePath" : "#0/#0"
	}
}
@endcode

@ref InfoEvent_EffectInfo_ExampleResponse "Example Response"

@subsection InfoEvent_Commands_ListCategoriesNames ListCategoriesNames

Lists assets categories.

@code{.json}
{
    "EventID" : "213",
    "Event" : "InfoEvent",
    "Command" : "ListCategoriesNames",
    "Request" : {}
}
@endcode

<b>Example response:</b>

@code{.json}
{
   "EventID" : "213",
   "Success" : "true",
   "cmd" : "ListCategoriesNames",
   "list" : [ "audio", "fonts", "meshes", "sequences", "svgs", "textures", "video" ]
}
@endcode

@subsection InfoEvent_Commands_ListSceneAssets ListSceneAssets

Lists all assets used in loaded @ref scenes "Scenes" of selected category.
Note that <b>CategoryName</b> is Asset descriptor UID not category name like in event @ref InfoEvent_Commands_ListCategoriesNames "ListCategoriesNames".
@todo Fix this inconsistency in future versions.

@code{.json}
{
    "EventID" : "213",
    "Event" : "InfoEvent",
    "Command" : "ListSceneAssets",
    "Request" :
    {
        "CategoryName" : "TEXTURE_ASSET_DESC"
    }
}
@endcode

@ref InfoEvent_ListSceneAssets_ExampleResponse "Example Response"

@subsection InfoEvent_Commands_GetAssetDescriptor GetAssetDescriptor

Gets @ref AssetDesc "AssetDescriptor" template for choosen @ref Assets "Asset".
Categories names can be listed with @ref InfoEvent_Commands_ListCategoriesNames "ListCategoriesNames" command.

@code{.json}
{
    "EventID" : "213",
    "Event" : "InfoEvent",
    "Command" : "GetAssetDescriptor",
    "Request" :
    {
        "categoryName" : "textures",
        "projectName" : "",
        "path" : "crystal.jpg"
    }
}
@endcode

@ref InfoEvent_GetAssetDescriptor_ExampleResponse "Example Response"



*/
class InfoEvent : public RemoteEvent
{
public:

    typedef enum
    {
        TreeStructure,
        MinimalTreeStructure,
        NodeInfo,
        PluginInfo,
        EffectInfo,
        ParamInfo,
        LogicInfo,
        MinimalSceneInfo,
        LightsInfo,
        CamerasInfo,
        ListParamDescriptors,
        GetParamDescriptor,

        ListSceneAssets,
        ListProjectNames,
        ListScenes,
        ListPresets,
        ListAssetsPaths,
        ListCategoriesNames,
        ListProjects,
        ListAllScenes,
        ListAllFolders,
        ListResourcesInFolders,
        ListAllResources,
        GetAssetDescriptor,
        GetAssetThumbnail,
        GetSceneThumbnail,
        GetPresetThumbnail,
        GetPMItemStats,

        Timelines,
        CheckTimelineTime,
        ListTimelineKeyframes,
        ListTimelinesParams,

        Performance,
        Videocards,
        Fail            ///< Wrong command
    } Command;

private:

    static const EventType      m_sEventType;
    static std::string          m_sEventName;

public:

    InfoEvent::Command          InfoCommand;
    IDeserializer *             Request;

public:
    explicit                        InfoEvent   ()  { Request = nullptr; }
    ~InfoEvent   () { delete Request; }

    virtual void                    Serialize           ( ISerializer& ser ) const;
    static IEventPtr                Create              ( IDeserializer& deser );
    virtual IEventPtr               Clone               () const;

    static EventType                Type                ();
    static std::string&             Name                ();
    virtual const std::string &     GetName             () const;
    virtual EventType               GetEventType        () const;
};


DECLARE_ENUM_SERIALIZATION( InfoEvent::Command )
DEFINE_PTR_TYPE( InfoEvent )


}	// bv

/**@page InfoEvent_GetAssetDescriptor_ExampleResponse InfoEvent GetAssetDescriptor command example response:

@code{.json}
{
   "AssetData" : {
      "asset" : {
         "estimatedMemoryUsage" : "18009000",
         "filter" : "none",
         "height" : "2001",
         "loading_type" : "ONLY_ORIGINAL",
         "path" : "textures\\crystal.jpg",
         "type" : "TEXTURE_ASSET_DESC",
         "width" : "3000"
      }
   },
   "EventID" : "213",
   "Success" : "true",
   "categoryName" : "textures",
   "cmd" : "GetAssetDescriptor",
   "path" : "crystal.jpg",
   "projectName" : ""
}
@endcode

*/

/**@page InfoEvent_ListSceneAssets_ExampleResponse InfoEvent ListSceneAssets command example response:

Example created with scene <b>Textures_Example</b> loaded.

@code{.json}
{
   "EventID" : "213",
   "Success" : "true",
   "assets" : [
      {
         "filter" : "bilinear",
         "loading_type" : "GENERATE_MIPMAPS",
         "path" : "textures/ice-surface.jpg",
         "type" : "TEXTURE_ASSET_DESC"
      },
      {
         "filter" : "bilinear",
         "loading_type" : "GENERATE_MIPMAPS",
         "path" : "textures/snowflakes.jpg",
         "type" : "TEXTURE_ASSET_DESC"
      }
   ],
   "cmd" : "ListSceneAssets"
}
@endcode

*/


/**@page InfoEvent_EffectInfo_ExampleResponse InfoEvent EffectInfo command example response:

@code{.json}
{
   "EventID" : "1",
   "NodePath" : "#0/#0",
   "SceneName" : "BluredShapes.scn",
   "Success" : "true",
   "cmd" : "EffectInfo",
   "effect" : {
      "numRequiredAssets" : "0",
      "params" : [
         {
            "interpolator" : {
               "curve_type" : "linear",
               "interpolations" : [],
               "keys" : [
                  {
                     "time" : "0.000000",
                     "val" : "20.799999"
                  }
               ],
               "postMethod" : "clamp",
               "preMethod" : "clamp"
            },
            "name" : "blurSize",
            "timeline" : "BluredShapes.scn",
            "type" : "float"
         },
         {
            "interpolator" : {
               "curve_type" : "point",
               "interpolations" : [],
               "keys" : [
                  {
                     "time" : "0.000000",
                     "val" : "2"
                  }
               ],
               "postMethod" : "clamp",
               "preMethod" : "clamp"
            },
            "name" : "blurKernelType",
            "timeline" : "BluredShapes.scn",
            "type" : "int"
         },
         {
            "interpolator" : {
               "curve_type" : "point",
               "interpolations" : [],
               "keys" : [
                  {
                     "time" : "0.000000",
                     "val" : "true"
                  }
               ],
               "postMethod" : "clamp",
               "preMethod" : "clamp"
            },
            "name" : "normalize",
            "timeline" : "BluredShapes.scn",
            "type" : "bool"
         },
         {
            "interpolator" : {
               "curve_type" : "point",
               "interpolations" : [],
               "keys" : [
                  {
                     "time" : "0.000000",
                     "val" : "true"
                  }
               ],
               "postMethod" : "clamp",
               "preMethod" : "clamp"
            },
            "name" : "effectIsEnabled",
            "timeline" : "BluredShapes.scn",
            "type" : "bool"
         }
      ],
      "type" : "blur"
   }
}
@endcode

*/


/**@page InfoEvent_PluginInfo_ExampleResponse InfoEvent PluginInfo command example response:

@code{.json}
{
   "EventID" : "1",
   "NodePath" : "#0/#0/#0",
   "SceneName" : "Shapes2D_Examples.scn",
   "Success" : "true",
   "cmd" : "PluginInfo",
   "plugin" : {
      "name" : "circle",
      "params" : [
         {
            "interpolator" : {
               "curve_type" : "linear",
               "interpolations" : [],
               "keys" : [
                  {
                     "time" : "0.000000",
                     "val" : "1.000000"
                  }
               ],
               "postMethod" : "clamp",
               "preMethod" : "clamp"
            },
            "name" : "outer radius",
            "timeline" : "Shapes2D_Examples.scn%default",
            "type" : "float"
         },
         {
            "interpolator" : {
               "curve_type" : "linear",
               "interpolations" : [],
               "keys" : [
                  {
                     "time" : "0.000000",
                     "val" : "0.500000"
                  }
               ],
               "postMethod" : "clamp",
               "preMethod" : "clamp"
            },
            "name" : "inner radius",
            "timeline" : "Shapes2D_Examples.scn%default",
            "type" : "float"
         },
         {
            "interpolator" : {
               "curve_type" : "linear",
               "interpolations" : [],
               "keys" : [
                  {
                     "time" : "0.000000",
                     "val" : "360.000000"
                  }
               ],
               "postMethod" : "clamp",
               "preMethod" : "clamp"
            },
            "name" : "open angle",
            "timeline" : "Shapes2D_Examples.scn%default",
            "type" : "float"
         },
         {
            "interpolator" : {
               "curve_type" : "point",
               "interpolations" : [],
               "keys" : [
                  {
                     "time" : "0.000000",
                     "val" : "78"
                  }
               ],
               "postMethod" : "clamp",
               "preMethod" : "clamp"
            },
            "name" : "tesselation",
            "timeline" : "Shapes2D_Examples.scn%default",
            "type" : "int"
         },
         {
            "interpolator" : {
               "curve_type" : "point",
               "interpolations" : [],
               "keys" : [
                  {
                     "time" : "0.000000",
                     "val" : "0"
                  }
               ],
               "postMethod" : "clamp",
               "preMethod" : "clamp"
            },
            "name" : "open angle mode",
            "timeline" : "Shapes2D_Examples.scn%default",
            "type" : "enum"
         }
      ],
      "timeline" : "Shapes2D_Examples.scn%default",
      "uid" : "DEFAULT_CIRCLE"
   }
}
@endcode


*/


/**@page InfoEvent_LogicInfo_ExampleResponse InfoEvent LogicInfo command example response:

@code{.json}
{
   "EventID" : "10713",
   "NodePath" : "#0/#0",
   "SceneName" : "Shapes2D_Examples.scn",
   "Success" : "true",
   "cmd" : "LogicInfo",
   "logic" : {
      "params" : [
         {
            "interpolator" : {
               "curve_type" : "linear",
               "interpolations" : [],
               "keys" : [
                  {
                     "time" : "0.000000",
                     "val" : "5.000000"
                  }
               ],
               "postMethod" : "clamp",
               "preMethod" : "clamp"
            },
            "name" : "MaxHeight",
            "timeline" : "Shapes2D_Examples.scn%default",
            "type" : "float"
         },
         {
            "interpolator" : {
               "curve_type" : "linear",
               "interpolations" : [],
               "keys" : [
                  {
                     "time" : "0.000000",
                     "val" : "5.000000"
                  }
               ],
               "postMethod" : "clamp",
               "preMethod" : "clamp"
            },
            "name" : "MaxWidth",
            "timeline" : "Shapes2D_Examples.scn%default",
            "type" : "float"
         },
         {
            "interpolator" : {
               "curve_type" : "linear",
               "interpolations" : [],
               "keys" : [
                  {
                     "time" : "0.000000",
                     "val" : "5.000000"
                  }
               ],
               "postMethod" : "clamp",
               "preMethod" : "clamp"
            },
            "name" : "MaxDepth",
            "timeline" : "Shapes2D_Examples.scn%default",
            "type" : "float"
         },
         {
            "interpolator" : {
               "curve_type" : "point",
               "interpolations" : [],
               "keys" : [
                  {
                     "time" : "0.000000",
                     "val" : "false"
                  }
               ],
               "postMethod" : "clamp",
               "preMethod" : "clamp"
            },
            "name" : "IsProportional",
            "timeline" : "Shapes2D_Examples.scn%default",
            "type" : "bool"
         }
      ],
      "type" : "MaxSize"
   }
}

@endcode

*/


/**@page InfoEvent_LightsInfo_ExampleResponse InfoEvent LightsInfo command example response:

@code{.json}
{
   "EventID" : "10713",
   "SceneName" : "Shapes2D_Examples.scn",
   "Success" : "true",
   "cmd" : "LightsInfo",
   "lights" : [
      {
         "params" : [
            {
               "interpolator" : {
                  "curve_type" : "linear",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "1.000000, 1.000000, 1.000000"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "color",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "vec3"
            },
            {
               "interpolator" : {
                  "curve_type" : "linear",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "0.000000, 0.000000, 0.000000"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "direction",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "vec3"
            },
            {
               "interpolator" : {
                  "curve_type" : "linear",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "0.000000, 0.000000, 0.000000"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "position",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "vec3"
            },
            {
               "interpolator" : {
                  "curve_type" : "linear",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "1.000000, 0.000000, 0.020000"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "attenuation",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "vec3"
            },
            {
               "interpolator" : {
                  "curve_type" : "linear",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "100.000000"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "cutOff",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "float"
            },
            {
               "interpolator" : {
                  "curve_type" : "point",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "10"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "exponent",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "int"
            }
         ],
         "type" : "spot"
      },
      {
         "params" : [
            {
               "interpolator" : {
                  "curve_type" : "linear",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "1.000000, 1.000000, 1.000000"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "color",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "vec3"
            },
            {
               "interpolator" : {
                  "curve_type" : "linear",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "0.000000, 0.000000, 0.000000"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "direction",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "vec3"
            },
            {
               "interpolator" : {
                  "curve_type" : "linear",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "0.000000, 0.000000, 0.000000"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "position",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "vec3"
            },
            {
               "interpolator" : {
                  "curve_type" : "linear",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "1.000000, 0.000000, 0.020000"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "attenuation",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "vec3"
            },
            {
               "interpolator" : {
                  "curve_type" : "linear",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "100.000000"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "cutOff",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "float"
            },
            {
               "interpolator" : {
                  "curve_type" : "point",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "10"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "exponent",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "int"
            }
         ],
         "type" : "spot"
      }
   ]
}
@endcode
*/


/**@page InfoEvent_CamerasInfo_ExampleResponse InfoEvent CamerasInfo command example response:

@code{.json}
{
   "CurrentCamera" : "0",
   "EventID" : "10713",
   "Success" : "true",
   "cameras" : [
      {
         "params" : [
            {
               "interpolator" : {
                  "curve_type" : "point",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "true"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "IsPerspective",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "bool"
            },
            {
               "interpolator" : {
                  "curve_type" : "linear",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "45.000000"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "FOV",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "float"
            },
            {
               "interpolator" : {
                  "curve_type" : "linear",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "0.100000"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "NearClippingPlane",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "float"
            },
            {
               "interpolator" : {
                  "curve_type" : "linear",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "100.000000"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "FarClippingPlane",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "float"
            },
            {
               "interpolator" : {
                  "curve_type" : "linear",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "2.000000"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "ViewportSize",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "float"
            },
            {
               "interpolator" : {
                  "curve_type" : "linear",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "0.000000, 0.000000, 5.000000"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "Position",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "vec3"
            },
            {
               "interpolator" : {
                  "curve_type" : "linear",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "0.000000, 0.000000, -1.000000"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "Direction",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "vec3"
            },
            {
               "interpolator" : {
                  "curve_type" : "linear",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "0.000000, 1.000000, 0.000000"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "UpVector",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "vec3"
            }
         ]
      },
      {
         "params" : [
            {
               "interpolator" : {
                  "curve_type" : "point",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "true"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "IsPerspective",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "bool"
            },
            {
               "interpolator" : {
                  "curve_type" : "linear",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "45.000000"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "FOV",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "float"
            },
            {
               "interpolator" : {
                  "curve_type" : "linear",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "0.100000"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "NearClippingPlane",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "float"
            },
            {
               "interpolator" : {
                  "curve_type" : "linear",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "100.000000"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "FarClippingPlane",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "float"
            },
            {
               "interpolator" : {
                  "curve_type" : "linear",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "2.000000"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "ViewportSize",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "float"
            },
            {
               "interpolator" : {
                  "curve_type" : "linear",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "0.000000, 0.000000, 5.000000"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "Position",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "vec3"
            },
            {
               "interpolator" : {
                  "curve_type" : "linear",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "0.000000, 0.000000, -1.000000"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "Direction",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "vec3"
            },
            {
               "interpolator" : {
                  "curve_type" : "linear",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "0.000000, 1.000000, 0.000000"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "UpVector",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "vec3"
            }
         ]
      },
      {
         "params" : [
            {
               "interpolator" : {
                  "curve_type" : "point",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "true"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "IsPerspective",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "bool"
            },
            {
               "interpolator" : {
                  "curve_type" : "linear",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "45.000000"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "FOV",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "float"
            },
            {
               "interpolator" : {
                  "curve_type" : "linear",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "0.100000"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "NearClippingPlane",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "float"
            },
            {
               "interpolator" : {
                  "curve_type" : "linear",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "100.000000"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "FarClippingPlane",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "float"
            },
            {
               "interpolator" : {
                  "curve_type" : "linear",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "2.000000"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "ViewportSize",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "float"
            },
            {
               "interpolator" : {
                  "curve_type" : "linear",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "0.000000, 0.000000, 5.000000"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "Position",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "vec3"
            },
            {
               "interpolator" : {
                  "curve_type" : "linear",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "0.000000, 0.000000, -1.000000"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "Direction",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "vec3"
            },
            {
               "interpolator" : {
                  "curve_type" : "linear",
                  "interpolations" : [],
                  "keys" : [
                     {
                        "time" : "0.000000",
                        "val" : "0.000000, 1.000000, 0.000000"
                     }
                  ],
                  "postMethod" : "clamp",
                  "preMethod" : "clamp"
               },
               "name" : "UpVector",
               "timeline" : "Shapes2D_Examples.scn%default",
               "type" : "vec3"
            }
         ]
      }
   ],
   "cmd" : "CamerasInfo"
}

@endcode
*/


/**@page InfoEvent_NodeInfo_ExampleResponse InfoEvent NodeInfo command example response:

@code{.json}
{
   "EventID" : "2147483647",
   "NodePath" : "#0",
   "SceneName" : "Shapes2D_Examples.scn",
   "Success" : "true",
   "cmd" : "NodeInfo",
   "node" : {
      "id" : "32",
      "logic" : {
         "params" : [],
         "type" : "NodeVisibilityAnimation"
      },
      "name" : "root",
      "plugins" : [
         {
            "name" : "transform",
            "params" : [
               {
                  "composite_transform" : [
                     {
                        "interpolators" : [
                           {
                              "curve_type" : "linear",
                              "interpolations" : [],
                              "keys" : [
                                 {
                                    "time" : "0.000000",
                                    "val" : "0.000000"
                                 }
                              ],
                              "postMethod" : "clamp",
                              "preMethod" : "clamp"
                           },
                           {
                              "curve_type" : "linear",
                              "interpolations" : [],
                              "keys" : [
                                 {
                                    "time" : "0.000000",
                                    "val" : "0.000000"
                                 }
                              ],
                              "postMethod" : "clamp",
                              "preMethod" : "clamp"
                           },
                           {
                              "curve_type" : "linear",
                              "interpolations" : [],
                              "keys" : [
                                 {
                                    "time" : "0.000000",
                                    "val" : "0.000000"
                                 }
                              ],
                              "postMethod" : "clamp",
                              "preMethod" : "clamp"
                           }
                        ],
                        "kind" : "center"
                     },
                     {
                        "interpolators" : [
                           {
                              "curve_type" : "linear",
                              "interpolations" : [],
                              "keys" : [
                                 {
                                    "time" : "0.000000",
                                    "val" : "0.000000"
                                 }
                              ],
                              "postMethod" : "clamp",
                              "preMethod" : "clamp"
                           },
                           {
                              "curve_type" : "linear",
                              "interpolations" : [],
                              "keys" : [
                                 {
                                    "time" : "0.000000",
                                    "val" : "0.000000"
                                 }
                              ],
                              "postMethod" : "clamp",
                              "preMethod" : "clamp"
                           },
                           {
                              "curve_type" : "linear",
                              "interpolations" : [],
                              "keys" : [
                                 {
                                    "time" : "0.000000",
                                    "val" : "0.000000"
                                 }
                              ],
                              "postMethod" : "clamp",
                              "preMethod" : "clamp"
                           }
                        ],
                        "kind" : "translation"
                     },
                     {
                        "interpolators" : [
                           {
                              "curve_type" : "linear",
                              "interpolations" : [],
                              "keys" : [
                                 {
                                    "time" : "0.000000",
                                    "val" : "0.000000"
                                 }
                              ],
                              "postMethod" : "clamp",
                              "preMethod" : "clamp"
                           },
                           {
                              "curve_type" : "linear",
                              "interpolations" : [],
                              "keys" : [
                                 {
                                    "time" : "0.000000",
                                    "val" : "0.000000"
                                 }
                              ],
                              "postMethod" : "clamp",
                              "preMethod" : "clamp"
                           },
                           {
                              "curve_type" : "linear",
                              "interpolations" : [],
                              "keys" : [
                                 {
                                    "time" : "0.000000",
                                    "val" : "0.000000"
                                 }
                              ],
                              "postMethod" : "clamp",
                              "preMethod" : "clamp"
                           }
                        ],
                        "iseuler" : "true",
                        "kind" : "rotation"
                     },
                     {
                        "interpolators" : [
                           {
                              "curve_type" : "linear",
                              "interpolations" : [],
                              "keys" : [
                                 {
                                    "time" : "0.000000",
                                    "val" : "1.000000"
                                 }
                              ],
                              "postMethod" : "clamp",
                              "preMethod" : "clamp"
                           },
                           {
                              "curve_type" : "linear",
                              "interpolations" : [],
                              "keys" : [
                                 {
                                    "time" : "0.000000",
                                    "val" : "1.000000"
                                 }
                              ],
                              "postMethod" : "clamp",
                              "preMethod" : "clamp"
                           },
                           {
                              "curve_type" : "linear",
                              "interpolations" : [],
                              "keys" : [
                                 {
                                    "time" : "0.000000",
                                    "val" : "1.000000"
                                 }
                              ],
                              "postMethod" : "clamp",
                              "preMethod" : "clamp"
                           }
                        ],
                        "kind" : "scale"
                     }
                  ],
                  "name" : "simple_transform",
                  "timeline" : "Shapes2D_Examples.scn%default",
                  "type" : "transform"
               }
            ],
            "timeline" : "Shapes2D_Examples.scn%default",
            "uid" : "DEFAULT_TRANSFORM"
         }
      ],
      "visible" : "true"
   }
}
@endcode

*/



/**@page InfoEvent_MinimalTreeStructure_ExampleResponse InfoEvent MinimalTreeStructure command example response:

@code{.json}
{
   "EventID" : "2147483647",
   "Success" : "true",
   "cmd" : "TreeStructure",
   "scenes" : [
      {
         "preset" : "false",
         "scene" : {
            "RenderChannelIdx" : "0",
            "name" : "Shapes2D_Examples.scn",
            "node" : {
               "id" : "38",
               "logic" : {
                  "type" : "NodeVisibilityAnimation"
               },
               "name" : "root",
               "nodes" : [
                  {
                     "id" : "40",
                     "name" : "circles",
                     "nodes" : [
                        {
                           "id" : "42",
                           "name" : "circle1",
                           "nodes" : [],
                           "plugins" : [
                              {
                                 "name" : "transform",
                                 "uid" : "DEFAULT_TRANSFORM"
                              },
                              {
                                 "name" : "circle",
                                 "uid" : "DEFAULT_CIRCLE"
                              },
                              {
                                 "name" : "solid color",
                                 "uid" : "DEFAULT_COLOR"
                              }
                           ]
                        },
                        {
                           "id" : "46",
                           "name" : "circle2",
                           "nodes" : [],
                           "plugins" : [
                              {
                                 "name" : "transform",
                                 "uid" : "DEFAULT_TRANSFORM"
                              },
                              {
                                 "name" : "circle",
                                 "uid" : "DEFAULT_CIRCLE"
                              },
                              {
                                 "name" : "solid color",
                                 "uid" : "DEFAULT_COLOR"
                              }
                           ]
                        },
                        {
                           "id" : "50",
                           "name" : "circle3",
                           "nodes" : [],
                           "plugins" : [
                              {
                                 "name" : "transform",
                                 "uid" : "DEFAULT_TRANSFORM"
                              },
                              {
                                 "name" : "solid color",
                                 "uid" : "DEFAULT_COLOR"
                              },
                              {
                                 "name" : "circle",
                                 "uid" : "DEFAULT_CIRCLE"
                              }
                           ]
                        }
                     ],
                     "plugins" : [
                        {
                           "name" : "transform",
                           "uid" : "DEFAULT_TRANSFORM"
                        }
                     ]
                  },
                  {
                     "id" : "54",
                     "name" : "rectangles",
                     "nodes" : [
                        {
                           "id" : "65",
                           "name" : "rectangle1",
                           "nodes" : [],
                           "plugins" : [
                              {
                                 "name" : "transform",
                                 "uid" : "DEFAULT_TRANSFORM"
                              },
                              {
                                 "name" : "rectangle",
                                 "uid" : "DEFAULT_RECTANGLE"
                              },
                              {
                                 "name" : "linear_gradient",
                                 "uid" : "DEFAULT_LINEAR_GRADIENT"
                              }
                           ]
                        }
                     ],
                     "plugins" : [
                        {
                           "name" : "transform",
                           "uid" : "DEFAULT_TRANSFORM"
                        }
                     ]
                  }
               ],
               "plugins" : [
                  {
                     "name" : "transform",
                     "uid" : "DEFAULT_TRANSFORM"
                  }
               ]
            },
            "version" : {
               "Build" : "177",
               "Major" : "0",
               "Minor" : "51",
               "Patch" : "0",
               "Platform" : "Windows x64 Release",
               "SerializerVersion" : "1"
            }
         }
      }
   ]
}
@endcode
*/

/**@page InfoEvent_TreeStructure_ExampleResponseInfoEvent TreeStructure command example response:
@code{.json}
{
   "EventID" : "3024",
   "Success" : "true",
   "cmd" : "TreeStructure",
   "scenes" : [
      {
         "preset" : "false",
         "scene" : {
            "CurrentCamera" : "0",
            "RenderChannelIdx" : "0",
            "assets" : [],
            "cameras" : [
               {
                  "params" : [
                     {
                        "interpolator" : {
                           "curve_type" : "point",
                           "interpolations" : [],
                           "keys" : [
                              {
                                 "time" : "0.000000",
                                 "val" : "true"
                              }
                           ],
                           "postMethod" : "clamp",
                           "preMethod" : "clamp"
                        },
                        "name" : "IsPerspective",
                        "timeline" : "Shapes2D_Examples.scn%default",
                        "type" : "bool"
                     },
                     {
                        "interpolator" : {
                           "curve_type" : "linear",
                           "interpolations" : [],
                           "keys" : [
                              {
                                 "time" : "0.000000",
                                 "val" : "45.000000"
                              }
                           ],
                           "postMethod" : "clamp",
                           "preMethod" : "clamp"
                        },
                        "name" : "FOV",
                        "timeline" : "Shapes2D_Examples.scn%default",
                        "type" : "float"
                     },
                     {
                        "interpolator" : {
                           "curve_type" : "linear",
                           "interpolations" : [],
                           "keys" : [
                              {
                                 "time" : "0.000000",
                                 "val" : "0.100000"
                              }
                           ],
                           "postMethod" : "clamp",
                           "preMethod" : "clamp"
                        },
                        "name" : "NearClippingPlane",
                        "timeline" : "Shapes2D_Examples.scn%default",
                        "type" : "float"
                     },
                     {
                        "interpolator" : {
                           "curve_type" : "linear",
                           "interpolations" : [],
                           "keys" : [
                              {
                                 "time" : "0.000000",
                                 "val" : "100.000000"
                              }
                           ],
                           "postMethod" : "clamp",
                           "preMethod" : "clamp"
                        },
                        "name" : "FarClippingPlane",
                        "timeline" : "Shapes2D_Examples.scn%default",
                        "type" : "float"
                     },
                     {
                        "interpolator" : {
                           "curve_type" : "linear",
                           "interpolations" : [],
                           "keys" : [
                              {
                                 "time" : "0.000000",
                                 "val" : "2.000000"
                              }
                           ],
                           "postMethod" : "clamp",
                           "preMethod" : "clamp"
                        },
                        "name" : "ViewportSize",
                        "timeline" : "Shapes2D_Examples.scn%default",
                        "type" : "float"
                     },
                     {
                        "interpolator" : {
                           "curve_type" : "linear",
                           "interpolations" : [],
                           "keys" : [
                              {
                                 "time" : "0.000000",
                                 "val" : "0.000000, 0.000000, 5.000000"
                              }
                           ],
                           "postMethod" : "clamp",
                           "preMethod" : "clamp"
                        },
                        "name" : "Position",
                        "timeline" : "Shapes2D_Examples.scn%default",
                        "type" : "vec3"
                     },
                     {
                        "interpolator" : {
                           "curve_type" : "linear",
                           "interpolations" : [],
                           "keys" : [
                              {
                                 "time" : "0.000000",
                                 "val" : "0.000000, 0.000000, -1.000000"
                              }
                           ],
                           "postMethod" : "clamp",
                           "preMethod" : "clamp"
                        },
                        "name" : "Direction",
                        "timeline" : "Shapes2D_Examples.scn%default",
                        "type" : "vec3"
                     },
                     {
                        "interpolator" : {
                           "curve_type" : "linear",
                           "interpolations" : [],
                           "keys" : [
                              {
                                 "time" : "0.000000",
                                 "val" : "0.000000, 1.000000, 0.000000"
                              }
                           ],
                           "postMethod" : "clamp",
                           "preMethod" : "clamp"
                        },
                        "name" : "UpVector",
                        "timeline" : "Shapes2D_Examples.scn%default",
                        "type" : "vec3"
                     }
                  ]
               }
            ],
            "endUserParams" : {
               "CameraParamsDescriptors" : [],
               "EffectParamsDescriptors" : [],
               "LightParamsDescriptors" : [],
               "LogicParamsDescriptors" : [],
               "PluginParamsDescriptors" : []
            },
            "gridlines" : {
               "horizontal" : [],
               "vertical" : []
            },
            "lights" : [],
            "name" : "Shapes2D_Examples.scn",
            "node" : {
               "id" : "2",
               "logic" : {
                  "params" : [],
                  "type" : "NodeVisibilityAnimation"
               },
               "name" : "root",
               "nodes" : [
                  {
                     "id" : "4",
                     "name" : "circles",
                     "nodes" : [
                        {
                           "id" : "6",
                           "name" : "circle1",
                           "nodes" : [],
                           "plugins" : [
                              {
                                 "name" : "transform",
                                 "params" : [
                                    {
                                       "composite_transform" : [
                                          {
                                             "interpolators" : [
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                }
                                             ],
                                             "kind" : "center"
                                          },
                                          {
                                             "interpolators" : [
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "1.640000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                }
                                             ],
                                             "kind" : "translation"
                                          },
                                          {
                                             "interpolators" : [
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                }
                                             ],
                                             "iseuler" : "true",
                                             "kind" : "rotation"
                                          },
                                          {
                                             "interpolators" : [
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "1.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "1.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "1.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                }
                                             ],
                                             "kind" : "scale"
                                          }
                                       ],
                                       "name" : "simple_transform",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "transform"
                                    }
                                 ],
                                 "timeline" : "Shapes2D_Examples.scn%default",
                                 "uid" : "DEFAULT_TRANSFORM"
                              },
                              {
                                 "name" : "circle",
                                 "params" : [
                                    {
                                       "interpolator" : {
                                          "curve_type" : "linear",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "1.000000"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "outer radius",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "float"
                                    },
                                    {
                                       "interpolator" : {
                                          "curve_type" : "linear",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "0.500000"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "inner radius",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "float"
                                    },
                                    {
                                       "interpolator" : {
                                          "curve_type" : "linear",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "360.000000"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "open angle",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "float"
                                    },
                                    {
                                       "interpolator" : {
                                          "curve_type" : "point",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "78"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "tesselation",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "int"
                                    },
                                    {
                                       "interpolator" : {
                                          "curve_type" : "point",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "0"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "open angle mode",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "enum"
                                    }
                                 ],
                                 "timeline" : "Shapes2D_Examples.scn%default",
                                 "uid" : "DEFAULT_CIRCLE"
                              },
                              {
                                 "name" : "solid color",
                                 "params" : [
                                    {
                                       "interpolator" : {
                                          "curve_type" : "point",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "true"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "blend enable",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "bool"
                                    },
                                    {
                                       "interpolator" : {
                                          "curve_type" : "point",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "0"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "background blend mode",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "enum"
                                    },
                                    {
                                       "interpolator" : {
                                          "curve_type" : "linear",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "0.705000, 1.000000, 0.000000, 1.000000"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "color",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "vec4"
                                    }
                                 ],
                                 "timeline" : "Shapes2D_Examples.scn%default",
                                 "uid" : "DEFAULT_COLOR"
                              }
                           ],
                           "visible" : "true"
                        },
                        {
                           "id" : "10",
                           "name" : "circle2",
                           "nodes" : [],
                           "plugins" : [
                              {
                                 "name" : "transform",
                                 "params" : [
                                    {
                                       "composite_transform" : [
                                          {
                                             "interpolators" : [
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                }
                                             ],
                                             "kind" : "center"
                                          },
                                          {
                                             "interpolators" : [
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "-0.250000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.730000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                }
                                             ],
                                             "kind" : "translation"
                                          },
                                          {
                                             "interpolators" : [
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                }
                                             ],
                                             "iseuler" : "true",
                                             "kind" : "rotation"
                                          },
                                          {
                                             "interpolators" : [
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "1.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "1.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "1.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                }
                                             ],
                                             "kind" : "scale"
                                          }
                                       ],
                                       "name" : "simple_transform",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "transform"
                                    }
                                 ],
                                 "timeline" : "Shapes2D_Examples.scn%default",
                                 "uid" : "DEFAULT_TRANSFORM"
                              },
                              {
                                 "name" : "circle",
                                 "params" : [
                                    {
                                       "interpolator" : {
                                          "curve_type" : "linear",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "0.600000"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "outer radius",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "float"
                                    },
                                    {
                                       "interpolator" : {
                                          "curve_type" : "linear",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "0.000000"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "inner radius",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "float"
                                    },
                                    {
                                       "interpolator" : {
                                          "curve_type" : "linear",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "360.000000"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "open angle",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "float"
                                    },
                                    {
                                       "interpolator" : {
                                          "curve_type" : "point",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "20"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "tesselation",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "int"
                                    },
                                    {
                                       "interpolator" : {
                                          "curve_type" : "point",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "0"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "open angle mode",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "enum"
                                    }
                                 ],
                                 "timeline" : "Shapes2D_Examples.scn%default",
                                 "uid" : "DEFAULT_CIRCLE"
                              },
                              {
                                 "name" : "solid color",
                                 "params" : [
                                    {
                                       "interpolator" : {
                                          "curve_type" : "point",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "true"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "blend enable",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "bool"
                                    },
                                    {
                                       "interpolator" : {
                                          "curve_type" : "point",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "0"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "background blend mode",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "enum"
                                    },
                                    {
                                       "interpolator" : {
                                          "curve_type" : "linear",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "0.314000, 0.560000, 1.000000, 1.000000"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "color",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "vec4"
                                    }
                                 ],
                                 "timeline" : "Shapes2D_Examples.scn%default",
                                 "uid" : "DEFAULT_COLOR"
                              }
                           ],
                           "visible" : "true"
                        },
                        {
                           "id" : "14",
                           "name" : "circle3",
                           "nodes" : [],
                           "plugins" : [
                              {
                                 "name" : "transform",
                                 "params" : [
                                    {
                                       "composite_transform" : [
                                          {
                                             "interpolators" : [
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                }
                                             ],
                                             "kind" : "center"
                                          },
                                          {
                                             "interpolators" : [
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "-1.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                }
                                             ],
                                             "kind" : "translation"
                                          },
                                          {
                                             "interpolators" : [
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                }
                                             ],
                                             "iseuler" : "true",
                                             "kind" : "rotation"
                                          },
                                          {
                                             "interpolators" : [
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "1.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "1.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "1.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                }
                                             ],
                                             "kind" : "scale"
                                          }
                                       ],
                                       "name" : "simple_transform",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "transform"
                                    }
                                 ],
                                 "timeline" : "Shapes2D_Examples.scn%default",
                                 "uid" : "DEFAULT_TRANSFORM"
                              },
                              {
                                 "name" : "solid color",
                                 "params" : [
                                    {
                                       "interpolator" : {
                                          "curve_type" : "point",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "true"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "blend enable",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "bool"
                                    },
                                    {
                                       "interpolator" : {
                                          "curve_type" : "point",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "0"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "background blend mode",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "enum"
                                    },
                                    {
                                       "interpolator" : {
                                          "curve_type" : "linear",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "0.000000, 1.000000, 0.000000, 1.000000"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "color",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "vec4"
                                    }
                                 ],
                                 "timeline" : "Shapes2D_Examples.scn%default",
                                 "uid" : "DEFAULT_COLOR"
                              },
                              {
                                 "name" : "circle",
                                 "params" : [
                                    {
                                       "interpolator" : {
                                          "curve_type" : "linear",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "1.500000"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "outer radius",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "float"
                                    },
                                    {
                                       "interpolator" : {
                                          "curve_type" : "linear",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "0.000000"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "inner radius",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "float"
                                    },
                                    {
                                       "interpolator" : {
                                          "curve_type" : "linear",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "360.000000"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "open angle",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "float"
                                    },
                                    {
                                       "interpolator" : {
                                          "curve_type" : "point",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "80"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "tesselation",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "int"
                                    },
                                    {
                                       "interpolator" : {
                                          "curve_type" : "point",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "0"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "open angle mode",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "enum"
                                    }
                                 ],
                                 "timeline" : "Shapes2D_Examples.scn%default",
                                 "uid" : "DEFAULT_CIRCLE"
                              }
                           ],
                           "visible" : "true"
                        }
                     ],
                     "plugins" : [
                        {
                           "name" : "transform",
                           "params" : [
                              {
                                 "composite_transform" : [
                                    {
                                       "interpolators" : [
                                          {
                                             "curve_type" : "linear",
                                             "interpolations" : [],
                                             "keys" : [
                                                {
                                                   "time" : "0.000000",
                                                   "val" : "0.000000"
                                                }
                                             ],
                                             "postMethod" : "clamp",
                                             "preMethod" : "clamp"
                                          },
                                          {
                                             "curve_type" : "linear",
                                             "interpolations" : [],
                                             "keys" : [
                                                {
                                                   "time" : "0.000000",
                                                   "val" : "0.000000"
                                                }
                                             ],
                                             "postMethod" : "clamp",
                                             "preMethod" : "clamp"
                                          },
                                          {
                                             "curve_type" : "linear",
                                             "interpolations" : [],
                                             "keys" : [
                                                {
                                                   "time" : "0.000000",
                                                   "val" : "0.000000"
                                                }
                                             ],
                                             "postMethod" : "clamp",
                                             "preMethod" : "clamp"
                                          }
                                       ],
                                       "kind" : "center"
                                    },
                                    {
                                       "interpolators" : [
                                          {
                                             "curve_type" : "linear",
                                             "interpolations" : [],
                                             "keys" : [
                                                {
                                                   "time" : "0.000000",
                                                   "val" : "0.000000"
                                                }
                                             ],
                                             "postMethod" : "clamp",
                                             "preMethod" : "clamp"
                                          },
                                          {
                                             "curve_type" : "linear",
                                             "interpolations" : [],
                                             "keys" : [
                                                {
                                                   "time" : "0.000000",
                                                   "val" : "0.000000"
                                                }
                                             ],
                                             "postMethod" : "clamp",
                                             "preMethod" : "clamp"
                                          },
                                          {
                                             "curve_type" : "linear",
                                             "interpolations" : [],
                                             "keys" : [
                                                {
                                                   "time" : "0.000000",
                                                   "val" : "0.000000"
                                                }
                                             ],
                                             "postMethod" : "clamp",
                                             "preMethod" : "clamp"
                                          }
                                       ],
                                       "kind" : "translation"
                                    },
                                    {
                                       "interpolators" : [
                                          {
                                             "curve_type" : "linear",
                                             "interpolations" : [],
                                             "keys" : [
                                                {
                                                   "time" : "0.000000",
                                                   "val" : "0.000000"
                                                }
                                             ],
                                             "postMethod" : "clamp",
                                             "preMethod" : "clamp"
                                          },
                                          {
                                             "curve_type" : "linear",
                                             "interpolations" : [],
                                             "keys" : [
                                                {
                                                   "time" : "0.000000",
                                                   "val" : "0.000000"
                                                }
                                             ],
                                             "postMethod" : "clamp",
                                             "preMethod" : "clamp"
                                          },
                                          {
                                             "curve_type" : "linear",
                                             "interpolations" : [],
                                             "keys" : [
                                                {
                                                   "time" : "0.000000",
                                                   "val" : "0.000000"
                                                }
                                             ],
                                             "postMethod" : "clamp",
                                             "preMethod" : "clamp"
                                          }
                                       ],
                                       "iseuler" : "true",
                                       "kind" : "rotation"
                                    },
                                    {
                                       "interpolators" : [
                                          {
                                             "curve_type" : "linear",
                                             "interpolations" : [],
                                             "keys" : [
                                                {
                                                   "time" : "0.000000",
                                                   "val" : "1.000000"
                                                }
                                             ],
                                             "postMethod" : "clamp",
                                             "preMethod" : "clamp"
                                          },
                                          {
                                             "curve_type" : "linear",
                                             "interpolations" : [],
                                             "keys" : [
                                                {
                                                   "time" : "0.000000",
                                                   "val" : "1.000000"
                                                }
                                             ],
                                             "postMethod" : "clamp",
                                             "preMethod" : "clamp"
                                          },
                                          {
                                             "curve_type" : "linear",
                                             "interpolations" : [],
                                             "keys" : [
                                                {
                                                   "time" : "0.000000",
                                                   "val" : "1.000000"
                                                }
                                             ],
                                             "postMethod" : "clamp",
                                             "preMethod" : "clamp"
                                          }
                                       ],
                                       "kind" : "scale"
                                    }
                                 ],
                                 "name" : "simple_transform",
                                 "timeline" : "Shapes2D_Examples.scn%default",
                                 "type" : "transform"
                              }
                           ],
                           "timeline" : "Shapes2D_Examples.scn%default",
                           "uid" : "DEFAULT_TRANSFORM"
                        }
                     ],
                     "visible" : "true"
                  },
                  {
                     "id" : "18",
                     "name" : "rectangles",
                     "nodes" : [
                        {
                           "id" : "20",
                           "name" : "rectangle1",
                           "nodes" : [],
                           "plugins" : [
                              {
                                 "name" : "transform",
                                 "params" : [
                                    {
                                       "composite_transform" : [
                                          {
                                             "interpolators" : [
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                }
                                             ],
                                             "kind" : "center"
                                          },
                                          {
                                             "interpolators" : [
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "-2.080000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "1.200000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                }
                                             ],
                                             "kind" : "translation"
                                          },
                                          {
                                             "interpolators" : [
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                }
                                             ],
                                             "iseuler" : "true",
                                             "kind" : "rotation"
                                          },
                                          {
                                             "interpolators" : [
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "1.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "1.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "1.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                }
                                             ],
                                             "kind" : "scale"
                                          }
                                       ],
                                       "name" : "simple_transform",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "transform"
                                    }
                                 ],
                                 "timeline" : "Shapes2D_Examples.scn%default",
                                 "uid" : "DEFAULT_TRANSFORM"
                              },
                              {
                                 "name" : "rectangle",
                                 "params" : [
                                    {
                                       "interpolator" : {
                                          "curve_type" : "linear",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "2.200000"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "width",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "float"
                                    },
                                    {
                                       "interpolator" : {
                                          "curve_type" : "linear",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "0.700000"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "height",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "float"
                                    },
                                    {
                                       "interpolator" : {
                                          "curve_type" : "point",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "2"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "weight center x",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "enum"
                                    },
                                    {
                                       "interpolator" : {
                                          "curve_type" : "point",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "2"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "weight center y",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "enum"
                                    }
                                 ],
                                 "timeline" : "Shapes2D_Examples.scn%default",
                                 "uid" : "DEFAULT_RECTANGLE"
                              },
                              {
                                 "name" : "linear_gradient",
                                 "params" : [
                                    {
                                       "interpolator" : {
                                          "curve_type" : "point",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "true"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "blend enable",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "bool"
                                    },
                                    {
                                       "interpolator" : {
                                          "curve_type" : "point",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "0"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "background blend mode",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "enum"
                                    },
                                    {
                                       "interpolator" : {
                                          "curve_type" : "linear",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "1.000000"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "alpha",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "float"
                                    },
                                    {
                                       "interpolator" : {
                                          "curve_type" : "linear",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "0.000000, 0.000000"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "point1",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "vec2"
                                    },
                                    {
                                       "interpolator" : {
                                          "curve_type" : "linear",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "1.000000, 0.000000"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "point2",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "vec2"
                                    },
                                    {
                                       "interpolator" : {
                                          "curve_type" : "linear",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "0.934000, 0.000000, 0.000000, 1.000000"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "color1",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "vec4"
                                    },
                                    {
                                       "interpolator" : {
                                          "curve_type" : "linear",
                                          "interpolations" : [],
                                          "keys" : [
                                             {
                                                "time" : "0.000000",
                                                "val" : "0.000000, 0.000000, 0.516000, 1.000000"
                                             }
                                          ],
                                          "postMethod" : "clamp",
                                          "preMethod" : "clamp"
                                       },
                                       "name" : "color2",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "vec4"
                                    },
                                    {
                                       "composite_transform" : [
                                          {
                                             "interpolators" : [
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.500000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.500000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                }
                                             ],
                                             "kind" : "center"
                                          },
                                          {
                                             "interpolators" : [
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                }
                                             ],
                                             "kind" : "translation"
                                          },
                                          {
                                             "interpolators" : [
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "0.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                }
                                             ],
                                             "iseuler" : "true",
                                             "kind" : "rotation"
                                          },
                                          {
                                             "interpolators" : [
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "1.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "1.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                },
                                                {
                                                   "curve_type" : "linear",
                                                   "interpolations" : [],
                                                   "keys" : [
                                                      {
                                                         "time" : "0.000000",
                                                         "val" : "1.000000"
                                                      }
                                                   ],
                                                   "postMethod" : "clamp",
                                                   "preMethod" : "clamp"
                                                }
                                             ],
                                             "kind" : "scale"
                                          }
                                       ],
                                       "name" : "txMat",
                                       "timeline" : "Shapes2D_Examples.scn%default",
                                       "type" : "transform"
                                    }
                                 ],
                                 "timeline" : "Shapes2D_Examples.scn%default",
                                 "uid" : "DEFAULT_LINEAR_GRADIENT"
                              }
                           ],
                           "visible" : "true"
                        }
                     ],
                     "plugins" : [
                        {
                           "name" : "transform",
                           "params" : [
                              {
                                 "composite_transform" : [
                                    {
                                       "interpolators" : [
                                          {
                                             "curve_type" : "linear",
                                             "interpolations" : [],
                                             "keys" : [
                                                {
                                                   "time" : "0.000000",
                                                   "val" : "0.000000"
                                                }
                                             ],
                                             "postMethod" : "clamp",
                                             "preMethod" : "clamp"
                                          },
                                          {
                                             "curve_type" : "linear",
                                             "interpolations" : [],
                                             "keys" : [
                                                {
                                                   "time" : "0.000000",
                                                   "val" : "0.000000"
                                                }
                                             ],
                                             "postMethod" : "clamp",
                                             "preMethod" : "clamp"
                                          },
                                          {
                                             "curve_type" : "linear",
                                             "interpolations" : [],
                                             "keys" : [
                                                {
                                                   "time" : "0.000000",
                                                   "val" : "0.000000"
                                                }
                                             ],
                                             "postMethod" : "clamp",
                                             "preMethod" : "clamp"
                                          }
                                       ],
                                       "kind" : "center"
                                    },
                                    {
                                       "interpolators" : [
                                          {
                                             "curve_type" : "linear",
                                             "interpolations" : [],
                                             "keys" : [
                                                {
                                                   "time" : "0.000000",
                                                   "val" : "0.000000"
                                                }
                                             ],
                                             "postMethod" : "clamp",
                                             "preMethod" : "clamp"
                                          },
                                          {
                                             "curve_type" : "linear",
                                             "interpolations" : [],
                                             "keys" : [
                                                {
                                                   "time" : "0.000000",
                                                   "val" : "0.000000"
                                                }
                                             ],
                                             "postMethod" : "clamp",
                                             "preMethod" : "clamp"
                                          },
                                          {
                                             "curve_type" : "linear",
                                             "interpolations" : [],
                                             "keys" : [
                                                {
                                                   "time" : "0.000000",
                                                   "val" : "0.000000"
                                                }
                                             ],
                                             "postMethod" : "clamp",
                                             "preMethod" : "clamp"
                                          }
                                       ],
                                       "kind" : "translation"
                                    },
                                    {
                                       "interpolators" : [
                                          {
                                             "curve_type" : "linear",
                                             "interpolations" : [],
                                             "keys" : [
                                                {
                                                   "time" : "0.000000",
                                                   "val" : "0.000000"
                                                }
                                             ],
                                             "postMethod" : "clamp",
                                             "preMethod" : "clamp"
                                          },
                                          {
                                             "curve_type" : "linear",
                                             "interpolations" : [],
                                             "keys" : [
                                                {
                                                   "time" : "0.000000",
                                                   "val" : "0.000000"
                                                }
                                             ],
                                             "postMethod" : "clamp",
                                             "preMethod" : "clamp"
                                          },
                                          {
                                             "curve_type" : "linear",
                                             "interpolations" : [],
                                             "keys" : [
                                                {
                                                   "time" : "0.000000",
                                                   "val" : "0.000000"
                                                }
                                             ],
                                             "postMethod" : "clamp",
                                             "preMethod" : "clamp"
                                          }
                                       ],
                                       "iseuler" : "true",
                                       "kind" : "rotation"
                                    },
                                    {
                                       "interpolators" : [
                                          {
                                             "curve_type" : "linear",
                                             "interpolations" : [],
                                             "keys" : [
                                                {
                                                   "time" : "0.000000",
                                                   "val" : "1.000000"
                                                }
                                             ],
                                             "postMethod" : "clamp",
                                             "preMethod" : "clamp"
                                          },
                                          {
                                             "curve_type" : "linear",
                                             "interpolations" : [],
                                             "keys" : [
                                                {
                                                   "time" : "0.000000",
                                                   "val" : "1.000000"
                                                }
                                             ],
                                             "postMethod" : "clamp",
                                             "preMethod" : "clamp"
                                          },
                                          {
                                             "curve_type" : "linear",
                                             "interpolations" : [],
                                             "keys" : [
                                                {
                                                   "time" : "0.000000",
                                                   "val" : "1.000000"
                                                }
                                             ],
                                             "postMethod" : "clamp",
                                             "preMethod" : "clamp"
                                          }
                                       ],
                                       "kind" : "scale"
                                    }
                                 ],
                                 "name" : "simple_transform",
                                 "timeline" : "Shapes2D_Examples.scn%default",
                                 "type" : "transform"
                              }
                           ],
                           "timeline" : "Shapes2D_Examples.scn%default",
                           "uid" : "DEFAULT_TRANSFORM"
                        }
                     ],
                     "visible" : "true"
                  }
               ],
               "plugins" : [
                  {
                     "name" : "transform",
                     "params" : [
                        {
                           "composite_transform" : [
                              {
                                 "interpolators" : [
                                    {
                                       "curve_type" : "linear",
                                       "interpolations" : [],
                                       "keys" : [
                                          {
                                             "time" : "0.000000",
                                             "val" : "0.000000"
                                          }
                                       ],
                                       "postMethod" : "clamp",
                                       "preMethod" : "clamp"
                                    },
                                    {
                                       "curve_type" : "linear",
                                       "interpolations" : [],
                                       "keys" : [
                                          {
                                             "time" : "0.000000",
                                             "val" : "0.000000"
                                          }
                                       ],
                                       "postMethod" : "clamp",
                                       "preMethod" : "clamp"
                                    },
                                    {
                                       "curve_type" : "linear",
                                       "interpolations" : [],
                                       "keys" : [
                                          {
                                             "time" : "0.000000",
                                             "val" : "0.000000"
                                          }
                                       ],
                                       "postMethod" : "clamp",
                                       "preMethod" : "clamp"
                                    }
                                 ],
                                 "kind" : "center"
                              },
                              {
                                 "interpolators" : [
                                    {
                                       "curve_type" : "linear",
                                       "interpolations" : [],
                                       "keys" : [
                                          {
                                             "time" : "0.000000",
                                             "val" : "0.000000"
                                          }
                                       ],
                                       "postMethod" : "clamp",
                                       "preMethod" : "clamp"
                                    },
                                    {
                                       "curve_type" : "linear",
                                       "interpolations" : [],
                                       "keys" : [
                                          {
                                             "time" : "0.000000",
                                             "val" : "0.000000"
                                          }
                                       ],
                                       "postMethod" : "clamp",
                                       "preMethod" : "clamp"
                                    },
                                    {
                                       "curve_type" : "linear",
                                       "interpolations" : [],
                                       "keys" : [
                                          {
                                             "time" : "0.000000",
                                             "val" : "0.000000"
                                          }
                                       ],
                                       "postMethod" : "clamp",
                                       "preMethod" : "clamp"
                                    }
                                 ],
                                 "kind" : "translation"
                              },
                              {
                                 "interpolators" : [
                                    {
                                       "curve_type" : "linear",
                                       "interpolations" : [],
                                       "keys" : [
                                          {
                                             "time" : "0.000000",
                                             "val" : "0.000000"
                                          }
                                       ],
                                       "postMethod" : "clamp",
                                       "preMethod" : "clamp"
                                    },
                                    {
                                       "curve_type" : "linear",
                                       "interpolations" : [],
                                       "keys" : [
                                          {
                                             "time" : "0.000000",
                                             "val" : "0.000000"
                                          }
                                       ],
                                       "postMethod" : "clamp",
                                       "preMethod" : "clamp"
                                    },
                                    {
                                       "curve_type" : "linear",
                                       "interpolations" : [],
                                       "keys" : [
                                          {
                                             "time" : "0.000000",
                                             "val" : "0.000000"
                                          }
                                       ],
                                       "postMethod" : "clamp",
                                       "preMethod" : "clamp"
                                    }
                                 ],
                                 "iseuler" : "true",
                                 "kind" : "rotation"
                              },
                              {
                                 "interpolators" : [
                                    {
                                       "curve_type" : "linear",
                                       "interpolations" : [],
                                       "keys" : [
                                          {
                                             "time" : "0.000000",
                                             "val" : "1.000000"
                                          }
                                       ],
                                       "postMethod" : "clamp",
                                       "preMethod" : "clamp"
                                    },
                                    {
                                       "curve_type" : "linear",
                                       "interpolations" : [],
                                       "keys" : [
                                          {
                                             "time" : "0.000000",
                                             "val" : "1.000000"
                                          }
                                       ],
                                       "postMethod" : "clamp",
                                       "preMethod" : "clamp"
                                    },
                                    {
                                       "curve_type" : "linear",
                                       "interpolations" : [],
                                       "keys" : [
                                          {
                                             "time" : "0.000000",
                                             "val" : "1.000000"
                                          }
                                       ],
                                       "postMethod" : "clamp",
                                       "preMethod" : "clamp"
                                    }
                                 ],
                                 "kind" : "scale"
                              }
                           ],
                           "name" : "simple_transform",
                           "timeline" : "Shapes2D_Examples.scn%default",
                           "type" : "transform"
                        }
                     ],
                     "timeline" : "Shapes2D_Examples.scn%default",
                     "uid" : "DEFAULT_TRANSFORM"
                  }
               ],
               "visible" : "true"
            },
            "sceneVariables" : {
               "variablesDict" : []
            },
            "timelines" : [
               {
                  "duration" : "340282346638528859811704183484516925440.000000",
                  "events" : [],
                  "local_time" : "0.000000",
                  "loop" : "clamp",
                  "name" : "default",
                  "play" : "false",
                  "type" : "default"
               }
            ],
            "version" : {
               "Build" : "177",
               "Major" : "0",
               "Minor" : "51",
               "Patch" : "0",
               "Platform" : "Windows x64 Release",
               "SerializerVersion" : "1"
            }
         }
      }
   ]
}

@endcode
*/


/**@page InforEvent_MinimalSceneStructure_ExampleResponse MinimalSceneInfo command example response:

@code{.json}
{
   "EventID" : "2147483647",
   "Success" : "true",
   "cmd" : "MinimalSceneInfo",
   "preset" : "false",
   "scene" : {
      "RenderChannelIdx" : "0",
      "name" : "Shapes2D_Examples.scn",
      "node" : {
         "id" : "38",
         "logic" : {
            "type" : "NodeVisibilityAnimation"
         },
         "name" : "root",
         "nodes" : [
            {
               "id" : "40",
               "name" : "circles",
               "nodes" : [
                  {
                     "id" : "42",
                     "name" : "circle1",
                     "nodes" : [],
                     "plugins" : [
                        {
                           "name" : "transform",
                           "uid" : "DEFAULT_TRANSFORM"
                        },
                        {
                           "name" : "circle",
                           "uid" : "DEFAULT_CIRCLE"
                        },
                        {
                           "name" : "solid color",
                           "uid" : "DEFAULT_COLOR"
                        }
                     ]
                  },
                  {
                     "id" : "46",
                     "name" : "circle2",
                     "nodes" : [],
                     "plugins" : [
                        {
                           "name" : "transform",
                           "uid" : "DEFAULT_TRANSFORM"
                        },
                        {
                           "name" : "circle",
                           "uid" : "DEFAULT_CIRCLE"
                        },
                        {
                           "name" : "solid color",
                           "uid" : "DEFAULT_COLOR"
                        }
                     ]
                  },
                  {
                     "id" : "50",
                     "name" : "circle3",
                     "nodes" : [],
                     "plugins" : [
                        {
                           "name" : "transform",
                           "uid" : "DEFAULT_TRANSFORM"
                        },
                        {
                           "name" : "solid color",
                           "uid" : "DEFAULT_COLOR"
                        },
                        {
                           "name" : "circle",
                           "uid" : "DEFAULT_CIRCLE"
                        }
                     ]
                  }
               ],
               "plugins" : [
                  {
                     "name" : "transform",
                     "uid" : "DEFAULT_TRANSFORM"
                  }
               ]
            },
            {
               "id" : "54",
               "name" : "rectangles",
               "nodes" : [
                  {
                     "id" : "65",
                     "name" : "rectangle1",
                     "nodes" : [],
                     "plugins" : [
                        {
                           "name" : "transform",
                           "uid" : "DEFAULT_TRANSFORM"
                        }
                     ]
                  }
               ],
               "plugins" : [
                  {
                     "name" : "transform",
                     "uid" : "DEFAULT_TRANSFORM"
                  }
               ]
            }
         ],
         "plugins" : [
            {
               "name" : "transform",
               "uid" : "DEFAULT_TRANSFORM"
            }
         ]
      },
      "version" : {
         "Build" : "177",
         "Major" : "0",
         "Minor" : "51",
         "Patch" : "0",
         "Platform" : "Windows x64 Release",
         "SerializerVersion" : "1"
      }
   }
}
@endcode

*/
