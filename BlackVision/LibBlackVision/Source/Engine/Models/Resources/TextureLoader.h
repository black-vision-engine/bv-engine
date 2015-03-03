#pragma once

#include "Engine/Models/Resources/ResourceLoader.h"
#include "Engine/Models/Resources/Texture/SingleTextureResourceDescriptor.h"
#include "Engine/Models/Resources/Texture/SingleTextureResource.h"

#include "Engine/Types/Enums.h"
#include "Core/MemoryChunk.h"

#include <string>

namespace bv { namespace model {

class TextureLoader : public ResourceLoader
{
private:

    bool                    m_loadFromMemory;  // DEPRECATED

public:

	IResourceNEWConstPtr	LoadResource	( const ResourceDescConstPtr & desc ) const override;

    virtual ~TextureLoader(){}

private:

	static MemoryChunkConstPtr				LoadImage			( const std::string & path );
	static SingleTextureResourceConstPtr	LoadSingleTexture	( const SingleTextureResourceDescConstPtr & sinlgeTextureResDesc, bool loadFromCache = true );
};


} // model
} // bv
