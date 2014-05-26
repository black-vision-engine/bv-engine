#pragma once

#include "Engine/Models/Plugins/Plugin.h"


namespace bv { namespace model {

bool    LoadTexture     ( IPlugin * plugin, const std::string & textureFile );
bool    LoadAnimation   ( IPlugin * plugin, const std::string & animationPath, const std::string & filter );

} //model
} //bv
