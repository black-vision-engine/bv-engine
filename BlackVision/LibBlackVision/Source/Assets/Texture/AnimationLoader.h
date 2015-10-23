#pragma once

#include "Assets/AssetLoader.h"
#include "Assets/Texture/AnimationAsset.h"
#include "Assets/Texture/AnimationAssetDescriptor.h"
#include "Assets/Texture/TextureAssetDescriptor.h"
#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"

namespace bv {

class AnimationLoader : public AssetLoader
{
public:

	AssetConstPtr		LoadAsset			( const AssetDescConstPtr & desc ) const override;
    AssetDescConstPtr	CreateDescriptor	( const IDeserializer& deserializeObject ) const override;

    virtual ~AnimationLoader(){}

private:

	TextureAssetConstPtr	LoadFrame		( const TextureAssetDescConstPtr & frameDesc ) const;

};

} // bv
