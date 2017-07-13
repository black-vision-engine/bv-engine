#pragma once

#include "Assets/AssetDescriptor.h"

#include <string>


namespace bv
{


// ***********************
// Asynchronous asset loading request info.
struct LoadAssetRequest
{
    std::string             SceneName;
    std::string             NodePath;
    std::string             PluginName;
    AssetDescConstPtr       Descriptor;
};


}	// bv
