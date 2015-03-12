#pragma once

#include "Assets/AssetLoader.h"
#include "Assets/Texture/SingleTextureAssetDescriptor.h"
#include "Assets/Texture/SingleTextureAsset.h"

#include "Engine/Types/Enums.h"
#include "Memory/MemoryChunk.h"

#include <string>

namespace bv { namespace model {

class TextureLoader : public ResourceLoader
{
public:

	ResourceConstPtr						LoadResource		( const ResourceDescConstPtr & desc ) const override;

    virtual ~TextureLoader(){}

private:

	static MemoryChunkConstPtr				LoadImage			( const std::string & path );
	static SingleTextureResourceConstPtr	LoadSingleTexture	( const SingleTextureResourceDescConstPtr & sinlgeTextureResDesc, bool loadFromCache = true );
};


} // model
} // bv
