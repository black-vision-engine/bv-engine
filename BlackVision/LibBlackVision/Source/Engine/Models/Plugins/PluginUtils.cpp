#include "PluginUtils.h"

#include "Engine/Models/Resources/Texture/TextureResourceDescriptor.h"
#include "Engine/Models/Resources/Texture/AnimationResourceDescriptor.h"
#include "Engine/Models/Resources/Font/FontResourceDescriptor.h"

namespace bv { namespace model {

// *******************************
//
bool    LoadTexture     ( IPluginPtr plugin, const std::string & textureFile )
{
	auto texDesc = TextureResourceDesc::Create( textureFile );

    return plugin->LoadResource( texDesc );
}

// *******************************
//
bool    LoadAnimation   ( IPluginPtr plugin, const std::string & animationPath, const std::string & filter )
{
	auto desc = AnimationResourceDesc::CreateFromDirFrames( animationPath, filter );

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
	auto desc = FontResourceDesc::Create( fontFile, fontSize, blurSize, outlineSize, generateBitmaps );

    return plugin->LoadResource( desc );
}

} //model
} //bv
