#pragma once

#include "Assets/Texture/SingleTextureAssetDescriptor.h"
#include "Assets/Texture/SingleTextureAsset.h"

#include "Assets/Texture/TextureAssetDescriptor.h"
#include "Assets/Texture/TextureLoader.h"
#include "Assets/Texture/TextureAsset.h"

#include "Assets/Texture/AnimationAssetDescriptor.h"
#include "Assets/Texture/AnimationLoader.h"
#include "Assets/Texture/AnimationAsset.h"

#include "Assets/Font/2D/FontAssetDescriptor.h"
#include "Assets/Font/2D/Font2DLoader.h"
#include "Assets/Font/FontAsset.h"

#include "Assets/Font/3D/FontAsset3DDesc.h"
#include "Assets/Font/3D/FontAsset3D.h"
#include "Assets/Font/3D/Font3DLoader.h"

#include "Assets/AVStream/AVAssetDescriptor.h"
#include "Assets/AVStream/AVAsset.h"
#include "Assets//AVStream/AVAssetLoader.h"

#include "Assets/Mesh/MeshAssetDescriptor.h"
#include "Assets/Mesh/MeshAsset.h"
#include "Assets/Mesh/MeshAssetLoader.h"

#include "Assets/DataArray/DataArrayLoader.h"
#include "Assets/DataArray/DataArrayAssetDescriptor.h"
#include "Assets/DataArray/DataArrayAsset.h"

#include "Assets/SVG/SVGLoader.h"
#include "Assets/SVG/SVGAssetDescriptor.h"

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
} // bv