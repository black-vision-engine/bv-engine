#pragma once

#include "Assets/AssetLoader.h"
#include "Assets/Texture/TextureAsset.h"
#include "Assets/Texture/TextureAssetDescriptor.h"
#include "Assets/Texture/SingleTextureAsset.h"
#include "Assets/Texture/SingleTextureAssetDescriptor.h"
#include "Serialization/ISerializer.h"

#include "Engine/Types/Enums.h"
#include "Memory/MemoryChunk.h"

#include <string>

namespace bv {

class TextureLoader : public AssetLoader
{
public:

	AssetConstPtr						LoadAsset			( const AssetDescConstPtr & desc ) const override;
	AssetDescConstPtr					CreateDescriptor	( ISerializer& deserializeObject ) const override;

    virtual								~TextureLoader		(){}
};




} // bv
