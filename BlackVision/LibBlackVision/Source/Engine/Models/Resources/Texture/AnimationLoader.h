#pragma once

#include "Engine/Models/Resources/ResourceLoader.h"
#include "Engine/Models/Resources/Texture/AnimationResource.h"
#include "Engine/Models/Resources/Texture/AnimationResourceDescriptor.h"
#include "Engine/Models/Resources/Texture/TextureResourceDescriptor.h"

namespace bv { namespace model {

class AnimationLoader : public ResourceLoader
{
public:

	IResourceConstPtr		LoadResource	( const ResourceDescConstPtr & desc ) const override;

    virtual ~AnimationLoader(){}

private:

	TextureResourceConstPtr	LoadFrame		( const TextureResourceDescConstPtr & frameDesc ) const;

};


} // model
} // bv
