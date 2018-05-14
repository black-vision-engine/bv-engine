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

    ThumbnailConstPtr   LoadThumbnail       ( const AssetDescConstPtr & desc ) const override;

    virtual ~AnimationLoader(){}

private:

    void                SendProgress        ( AnimationAssetDescConstPtr desc, SizeType frame, SizeType numFrames ) const;

private:

	TextureAssetConstPtr	LoadFrame		( const TextureAssetDescConstPtr & frameDesc ) const;

};

} // bv
