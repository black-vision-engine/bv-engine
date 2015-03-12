#include "PluginUtils.h"

#include "Assets/Texture/TextureAssetDescriptor.h"
#include "Assets/Texture/AnimationAssetDescriptor.h"
#include "Assets/Font/FontAssetDescriptor.h"

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
bool    LoadAnimation   ( IPluginPtr plugin, const std::string & animationPath, const std::string & filter )
{
	auto desc = AnimationAssetDesc::CreateFromDirFrames( animationPath, filter );

    if( desc == nullptr )
    {
        return false;
    }

    return plugin->LoadResource( desc );
}

// *******************************
//
bool    LoadFont        ( IPluginPtr plugin, const std::string & fontFile, UInt32 fontSize, UInt32 blurSize, UInt32 outlineSize, bool generateBitmaps )
{
	auto desc = FontAssetDesc::Create( fontFile, fontSize, blurSize, outlineSize, generateBitmaps );

    return plugin->LoadResource( desc );
}

} //model
} //bv
