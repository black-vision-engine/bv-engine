#include "Assets.h"

namespace bv
{

// ***********************
//
TextureAssetConstPtr	LoadTextureAsset( const std::string & imageFilePath, bool isCacheable )
{
	return std::static_pointer_cast< const TextureAsset >( AssetManager::GetInstance().LoadAsset( TextureAssetDesc::Create( imageFilePath, isCacheable ) ) );
}

// ***********************
//
TextureAssetConstPtr	LoadTextureAsset( const std::string & imageFilePath, MipMapFilterType mmFilter, bool isCacheable )
{
	return std::static_pointer_cast< const TextureAsset >( AssetManager::GetInstance().LoadAsset( TextureAssetDesc::Create( imageFilePath, mmFilter, isCacheable ) ) );
}

// ***********************
//
TextureAssetConstPtr	LoadTextureAsset( const std::string & imageFilePath, const StringVector & mipMapsPaths, bool isCacheable )
{
	return std::static_pointer_cast< const TextureAsset >( AssetManager::GetInstance().LoadAsset( TextureAssetDesc::Create( imageFilePath, mipMapsPaths, isCacheable ) ) );
}

// ***********************
//
FontAssetConstPtr	LoadFontAsset	(	const std::string & fontFileName,
										UInt32 fontSize,
										UInt32 blurSize,
										UInt32 outlineSize,
										bool generateMipmaps )
{
	return std::static_pointer_cast< const FontAsset >( AssetManager::GetInstance().LoadAsset( FontAssetDesc::Create( fontFileName,
										fontSize,
										blurSize,
										outlineSize,
										generateMipmaps ) ) );
}

// ***********************
//
AnimationAssetConstPtr LoadAnimationAsset( const std::string & path, const std::string & filter )
{
	return std::static_pointer_cast< const AnimationAsset >( AssetManager::GetInstance().LoadAsset( AnimationAssetDesc::Create( path, filter ) ) );
}

} // bv