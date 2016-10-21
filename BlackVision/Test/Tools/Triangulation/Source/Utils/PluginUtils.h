#pragma once

#include <vector>

#include "Engine/Models/Plugins/Interfaces/IPluginDescriptor.h"
#include "Engine/Models/Plugins/Interfaces/IPlugin.h"
#include "Engine/Models/Plugins/Manager/PluginsManager.h"


namespace pluginUtils
{

std::vector< bv::model::IPluginDescriptor * >          CreatePlugins    ();



}   // pluginUtils
