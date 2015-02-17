#pragma once

#include "Engine/Models/Resources/ResourceLoader.h"
#include "Engine/Models/Resources/Texture/SingleTextureResourceDescriptor.h"
#include "Engine/Models/Resources/Texture/SingleTextureResource.h"

#include "Engine/Models/Resources/Resource.h"
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
    
	
	ResourceHandle *        LoadResource    ( IResource * res )  const; // DEPRECATED

    explicit                TextureLoader	( bool loadFormMemory = true );  // DEPRECATED

    virtual ~TextureLoader(){}

private:

	static MemoryChunkConstPtr				LoadImage			( const std::string & path );
	static SingleTextureResourceConstPtr	LoadSingleTexture	( const SingleTextureResourceDescConstPtr & sinlgeTextureResDesc, bool loadFromCache = true );
	static TextureResourceConstPtr			LoadFromCache		( const TextureResourceDescConstPtr & textureResDesc );
};

class TextureExtraData : public ResourceExtraData
{
public:

    SizeType				m_width;
    SizeType				m_height;
    SizeType				m_bitsPerPixel;

    TextureFormat           m_format;
    TextureType             m_type;

public:

    SizeType                GetWidth            () const;
    SizeType                GetHeight           () const;
    SizeType                GetBitsPerPixel     () const;

    TextureFormat           GetFormat           () const;
    TextureType             GetType             () const;

                            TextureExtraData    ();
    explicit                TextureExtraData    ( SizeType w, SizeType h, SizeType bitsPerPixel, TextureFormat format, TextureType type );

};

} // model
} // bv
