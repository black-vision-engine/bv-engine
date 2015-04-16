#pragma once

#include "TextureAsset.h"
#include "SingleTextureAssetDescriptor.h"
#include "TextureAssetDescriptor.h"

#include <map>

namespace bv
{

// *******************
// Implements high level cache for texture assets.
class TextureCache
{
public:

	// **************************
	// Adds entry to cache if doesn't exist.
	bool							Add		( const std::string & key, const TextureAssetConstPtr & textureRes );
	bool							Add		( const TextureAssetDescConstPtr & textureDesc, const TextureAssetConstPtr & textureRes );


	// **************************
	// Adds entry to cache if doesn't exist or update if exists.
	void							Update	( const std::string & key, const TextureAssetConstPtr & textureRes );

	bool							Exists	( const std::string & key ) const;
	bool							Exists	( const TextureAssetDescConstPtr & textureDesc ) const;

	TextureAssetConstPtr			Get		( const std::string & key ) const;
	TextureAssetConstPtr			Get		( const TextureAssetDescConstPtr & textureDesc ) const;

	// **************************
	// Returns reference to singleton of TextureCache class instance.
	static TextureCache &			GetInstance				();

	// **************************
	// Tree finctions below generates texture cache keys for single textures.
	static std::string				GenKeyForSingleTexture	( const SingleTextureAssetDescConstPtr & singleTextureDesc );
	static std::string				GenKeyForGeneratedMipMap( const std::string & origPath, SizeType width, SizeType height, TextureFormat format, SizeType mmLevel, MipMapFilterType mmFiletType );
	static std::string				GenKeyForTextureAsset	( const TextureAssetDescConstPtr & textureDesc );

private:

	TextureAssetConstPtr			Find				( const std::string & key ) const;
	void							AddToRawDataCache	( const TextureAssetConstPtr & textureRes ) const;
	MemoryChunkConstPtr				GetFormRawDataCache	( const SingleTextureAssetDescConstPtr & desc ) const;


	TextureCache();

	std::map< std::string, TextureAssetConstPtr > m_textures;

};

} // bv