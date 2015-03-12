#pragma once

#include "Assets/AssetLoader.h"
#include "Assets/Texture/AnimationAsset.h"
#include "Assets/Texture/AnimationAssetDescriptor.h"
#include "Assets/Texture/TextureAssetDescriptor.h"

namespace bv { namespace model {

class AnimationLoader : public AssetLoader
{
public:

	AssetConstPtr		LoadAsset	( const AssetDescConstPtr & desc ) const override;

    virtual ~AnimationLoader(){}

private:

	TextureAssetConstPtr	LoadFrame		( const TextureAssetDescConstPtr & frameDesc ) const;

};


} // model
} // bv
