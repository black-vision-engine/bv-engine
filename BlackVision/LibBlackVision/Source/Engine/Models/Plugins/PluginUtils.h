#pragma once

#include "Engine/Models/Plugins/Plugin.h"


namespace bv { namespace model {

bool    LoadTexture     ( IPluginPtr plugin, const std::string & textureFile, bool isCacheable = true );
bool    LoadTexture     ( IPluginPtr plugin, const std::string & textureFile, MipMapFilterType mmFilter, bool isCacheable = true );
bool    LoadTexture     ( IPluginPtr plugin, const std::string & textureFile, const StringVector & mipmapsFiles, bool isCacheable = true );


bool    LoadAnimation   ( IPluginPtr plugin, const std::string & animationPath, const std::string & filter );

bool    LoadAVStream    ( IPluginPtr plugin, const std::string & streamPath, TextureFormat textureFormat );
bool    LoadAVStream    ( IPluginPtr plugin, const std::string & streamPath, TextureFormat textureFormat, UInt32 width, UInt32 height, Float64 frameRate, VideoPixelFormat videoFormat );

bool    LoadFont        ( IPluginPtr plugin, const std::string & fontFile, UInt32 fontSize, UInt32 blurSize, UInt32 glowBlurSize, UInt32 outlineSize, bool generateMipmaps );

bool    LoadMesh        ( IPluginPtr plugin, const std::string & meshPath, const std::string & meshName = "", bool recursive = true );

} //model
} //bv
