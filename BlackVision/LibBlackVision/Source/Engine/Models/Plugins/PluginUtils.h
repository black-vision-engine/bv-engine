#pragma once

#include "Engine/Models/Plugins/Plugin.h"


namespace bv { namespace model {

bool    LoadTexture     ( IPluginPtr plugin, const std::string & textureFile, bool isCacheable = true );
bool    LoadTexture     ( IPluginPtr plugin, const std::string & textureFile, MipMapFilterType mmFilter, bool isCacheable = true );
bool    LoadTexture     ( IPluginPtr plugin, const std::string & textureFile, const StringVector & mipmapsFiles, bool isCacheable = true );


bool    LoadAnimation   ( IPluginPtr plugin, const std::string & animationPath, const std::string & filter );

bool    LoadVideoStream ( IPluginPtr plugin, const std::string & streamPath, TextureFormat textureFormat );

bool    LoadFont        ( IPluginPtr plugin, const std::string & fontFile, UInt32 fontSize, UInt32 blurSize, UInt32 outlineSize, bool generateMipmaps );

} //model
} //bv
