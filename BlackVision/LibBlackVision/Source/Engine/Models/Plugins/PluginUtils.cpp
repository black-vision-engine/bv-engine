#include "stdafx.h"

#include "PluginUtils.h"

#include "Assets/Assets.h"

#include "ProjectManager.h"



#include "Memory/MemoryLeaks.h"



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
bool    LoadAnimation   ( IPluginPtr plugin, const std::string & animationPath, const std::string & )
{
    auto desc = ProjectManager::GetInstance()->GetAssetDesc( "", "sequences", animationPath );

    if( desc == nullptr )
    {
        return false;
    }

    return plugin->LoadResource( desc );
}

// *******************************
//
bool    LoadAVStream ( IPluginPtr plugin, const std::string & streamPath, TextureFormat textureFormat )
{
    auto desc = AVAssetDesc::Create( streamPath, textureFormat );

    if( desc == nullptr )
    {
        return false;
    }

    return plugin->LoadResource( desc );
}

// *******************************
//
bool    LoadAVStream ( IPluginPtr plugin, const std::string & streamPath, TextureFormat textureFormat, UInt32 width, UInt32 height, Float64 frameRate, VideoPixelFormat videoFormat )
{
    auto desc = AVAssetDesc::Create( streamPath, textureFormat, width, height, frameRate, videoFormat );

    if( desc == nullptr )
    {
        return false;
    }

    return plugin->LoadResource( desc );
}

// *******************************
//
bool    LoadFont        ( IPluginPtr plugin, const std::string & fontFile, UInt32 fontSize, UInt32 blurSize, UInt32 glowBlurSize, UInt32 outlineSize, bool generateMipmaps )
{
    auto desc = FontAssetDesc::Create( fontFile, fontSize, blurSize, glowBlurSize, outlineSize, generateMipmaps, SUPPROTED_CHARS_FILE );

    return plugin->LoadResource( desc );
}

// *******************************
//
bool    LoadMesh        ( IPluginPtr plugin, const std::string & meshPath, const std::string & meshName, bool recursive )
{
    auto desc = MeshAssetDesc::Create( meshPath, meshName, recursive );

    return plugin->LoadResource( desc );
}

} //model
} //bv
