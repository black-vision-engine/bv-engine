#pragma once

#include <string>


namespace bv
{

// *************************************

namespace SerializationHelper
{

// Key names used to serialization.

    static const std::string EVENT_TYPE_STRING          = "Event";
    static const std::string TIMELINE_NAME_STRING       = "TimelineName";      // TimeLineEvent and NodeStructureEvent
    static const std::string COMMAND_STRING             = "Command";
    static const std::string REQUEST_STRING             = "Request";           // ProjectStructureEvent InfoEvent

    static const std::string RESPONSE_COMMAND_STRING    = "cmd";

    static const std::string SCENE_NAME_STRING          = "SceneName";
    static const std::string NODE_NAME_STRING           = "NodeName";
    static const std::string PLUGIN_NAME_STRING         = "PluginName";
    static const std::string PARAM_NAME_STRING          = "ParamName";
    static const std::string PARAM_SUB_NAME_STRING      = "ParamSubName";
    static const std::string PARAM_TARGET_TYPE_STRING   = "Target";
    static const std::string PARAM_INDEX                = "Index";          // Light and cameras

    static const char* EMPTY_STRING = "";

}   // SerializationHelper




}	// bv