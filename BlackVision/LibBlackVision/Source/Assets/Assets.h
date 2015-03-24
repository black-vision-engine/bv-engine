#pragma once

#include "Assets/Texture/TextureAssetDescriptor.h"
#include "Assets/Texture/TextureLoader.h"
#include "Assets/Texture/TextureAsset.h"

#include "Assets/Texture/AnimationAssetDescriptor.h"
#include "Assets/Texture/AnimationLoader.h"
#include "Assets/Texture/AnimationAsset.h"

#include "Assets/Font/FontAssetDescriptor.h"
#include "Assets/Font/FontLoader.h"
#include "Assets/Font/FontAsset.h"

#include "Assets/AssetManager.h"

namespace bv
{

// ***********************
//
TextureAssetConstPtr	LoadTextureAsset( const std::string & imageFilePath, bool isCacheable = true );

// ***********************
//
TextureAssetConstPtr	LoadTextureAsset( const std::string & imageFilePath, MipMapFilterType mmFilter, bool isCacheable = true );

// ***********************
//
TextureAssetConstPtr	LoadTextureAsset( const std::string & imageFilePath, const StringVector & mipMapsPaths, bool isCacheable = true );

// ***********************
//
FontAssetConstPtr		LoadFontAsset	(	const std::string & fontFileName,
											UInt32 fontSize,
											UInt32 blurSize,
											UInt32 outlineSize,
											bool generateMipmaps );

// ***********************
//
AnimationAssetConstPtr	LoadAnimationAsset( const std::string & path, const std::string & filter );

} // bv