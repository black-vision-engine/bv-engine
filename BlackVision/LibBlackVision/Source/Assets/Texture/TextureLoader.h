#pragma once

#include "Assets/AssetLoader.h"
#include "Assets/Texture/SingleTextureAssetDescriptor.h"
#include "Assets/Texture/SingleTextureAsset.h"

#include "Engine/Types/Enums.h"
#include "Memory/MemoryChunk.h"

#include <string>

namespace bv {

class TextureLoader : public AssetLoader
{
public:

	AssetConstPtr						LoadAsset		( const AssetDescConstPtr & desc ) const override;

    virtual ~TextureLoader(){}

private:

	static MemoryChunkConstPtr		    LoadImage			( const std::string & path );
	static SingleTextureAssetConstPtr	LoadSingleTexture	( const SingleTextureAssetDescConstPtr & sinlgeTextureResDesc, bool loadFromCache = true );
};

} // bv
