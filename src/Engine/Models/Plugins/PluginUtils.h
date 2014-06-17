#pragma once

#include "Engine/Models/Plugins/Plugin.h"


namespace bv { namespace model {

bool    LoadTexture     ( IPluginPtr plugin, const std::string & textureFile );
bool    LoadAnimation   ( IPluginPtr plugin, const std::string & animationPath, const std::string & filter );
bool    LoadFont        ( IPluginPtr plugin, const std::string & fontFile );

} //model
} //bv
