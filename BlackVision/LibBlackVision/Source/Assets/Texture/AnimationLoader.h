#pragma once

#include "Assets/AssetLoader.h"
#include "Assets/Texture/AnimationResource.h"
#include "Assets/Texture/AnimationAssetDescriptor.h"
#include "Assets/Texture/TextureAssetDescriptor.h"

namespace bv { namespace model {

class AnimationLoader : public ResourceLoader
{
public:

	ResourceConstPtr		LoadResource	( const ResourceDescConstPtr & desc ) const override;

    virtual ~AnimationLoader(){}

private:

	TextureResourceConstPtr	LoadFrame		( const TextureResourceDescConstPtr & frameDesc ) const;

};


} // model
} // bv
