#pragma once

#include "TextureResource.h"

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

	// **************************
	// Adds entry to cache if doesn't exist or update if exists.
	void							Update	( const std::string & key, const TextureResourceConstPtr & textureRes );

	bool							Exists	( const std::string & key ) const;
	TextureResourceConstPtr			Get		( const std::string & key ) const;

	// **************************
	// Returns reference to singleton of TextureCache class instance.
	static TextureCache &			GetInstance				();

	// **************************
	// Two finctions below generates texture cache keys for single textures.
	static std::string				GenKeyForSingleTexture	( const std::string & path, SizeType width, SizeType height, TextureFormat format );
	static std::string				GenKeyForGeneratedMipMap( const std::string & origPath, SizeType width, SizeType height, TextureFormat format, SizeType mmLevel, MipMapFilterType mmFiletType );

private:

	TextureResourceConstPtr			Find	( const std::string & key ) const;

	TextureCache();

	std::map< std::string, TextureResourceConstPtr > m_textures;

};

} // bv