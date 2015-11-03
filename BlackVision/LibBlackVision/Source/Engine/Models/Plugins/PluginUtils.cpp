#include "PluginUtils.h"

#include "Assets/Assets.h"

namespace bv { namespace model {

// *******************************
//
bool    LoadTexture     ( IPluginPtr plugin, const std::string & textureFile, bool isCacheable )
{
	auto texDesc = TextureAssetDesc::Create( textureFile, isCacheable );

    return plugin->LoadResource( texDesc );
}

// *******************************
//
bool    LoadTexture     ( IPluginPtr plugin, const std::string & textureFile, MipMapFilterType mmFilter, bool isCacheable )
{
	auto texDesc = TextureAssetDesc::Create( textureFile, mmFilter, isCacheable );

    return plugin->LoadResource( texDesc );
}

// *******************************
//
bool    LoadTexture     ( IPluginPtr plugin, const std::string & textureFile, const StringVector & mipmapsFiles, bool isCacheable )
{
	auto texDesc = TextureAssetDesc::Create( textureFile, mipmapsFiles, isCacheable );

    return plugin->LoadResource( texDesc );
}

// *******************************
//
bool    LoadAnimation   ( IPluginPtr plugin, const std::string & animationPath, const std::string & filter )
{
	auto desc = AnimationAssetDesc::Create( animationPath, filter );

    if( desc == nullptr )
    {
        return false;
    }

    return plugin->LoadResource( desc );
}

// *******************************
//
bool    LoadVideoStream ( IPluginPtr plugin, const std::string & streamPath )
{
	auto desc = VideoStreamAssetDesc::Create( streamPath );

    if( desc == nullptr )
    {
        return false;
    }

    return plugin->LoadResource( desc );
}

// *******************************
//
bool    LoadFont        ( IPluginPtr plugin, const std::string & fontFile, UInt32 fontSize, UInt32 blurSize, UInt32 outlineSize, bool generateMipmaps )
{
	auto desc = FontAssetDesc::Create( fontFile, fontSize, blurSize, outlineSize, generateMipmaps, L"fonts/SupportedChars.txt" );

    return plugin->LoadResource( desc );
}

} //model
} //bv
