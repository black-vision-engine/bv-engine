#pragma once

#include "TextureResource.h"
#include "SingleTextureResourceDescriptor.h"
#include "TextureResourceDescriptor.h"

#include <map>

namespace bv
{

// *******************
// Implements high level cache for texture resources.
class TextureCache
{
public:

	// **************************
	// Adds entry to cache if doesn't exist.
	bool							Add		( const std::string & key, const TextureResourceConstPtr & textureRes );
	bool							Add		( const TextureResourceDescConstPtr & textureDesc, const TextureResourceConstPtr & textureRes );


	// **************************
	// Adds entry to cache if doesn't exist or update if exists.
	void							Update	( const std::string & key, const TextureResourceConstPtr & textureRes );

	bool							Exists	( const std::string & key ) const;
	bool							Exists	( const TextureResourceDescConstPtr & textureDesc ) const;

	TextureResourceConstPtr			Get		( const std::string & key ) const;
	TextureResourceConstPtr			Get		( const TextureResourceDescConstPtr & textureDesc ) const;

	// **************************
	// Returns reference to singleton of TextureCache class instance.
	static TextureCache &			GetInstance				();

	// **************************
	// Two finctions below generates texture cache keys for single textures.
	static std::string				GenKeyForSingleTexture	( const SingleTextureResourceDescConstPtr & singleTextureDesc );
	static std::string				GenKeyForGeneratedMipMap( const std::string & origPath, SizeType width, SizeType height, TextureFormat format, SizeType mmLevel, MipMapFilterType mmFiletType );
	static std::string				GenKeyForTextureResource( const TextureResourceDescConstPtr & textureDesc );

private:

	TextureResourceConstPtr			Find	( const std::string & key ) const;

	TextureCache();

	std::map< std::string, TextureResourceConstPtr > m_textures;

};

} // bv