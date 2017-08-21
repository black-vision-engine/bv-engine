#pragma once

#include "CoreDEF.h"

namespace bv
{

class Asset;
class AssetDesc;
class AnimationAsset;
class AnimationAssetDesc;
class GeneratedSingleTextureAssetDesc;
class MipMapAsset;
class MipMapAssetDesc;
class SingleTextureAsset;
class SingleTextureAssetDesc;
class TextureAsset;
class TextureAssetDesc;
class FontAsset;
class FontAssetDesc;
class FontAsset3D;
class FontAsset3DDesc;
class AVAsset;
class AVAssetDesc;
class DataArrayAsset;
class DataArrayAssetDesc;
class DataArrayRowAssetDesc;

DEFINE_CONST_PTR_TYPE( FontAssetDesc )
DEFINE_CONST_PTR_TYPE( FontAsset )
DEFINE_CONST_PTR_TYPE( FontAssetDesc )
DEFINE_CONST_PTR_TYPE( FontAsset )
DEFINE_CONST_PTR_TYPE( TextureAssetDesc )
DEFINE_CONST_PTR_TYPE( TextureAsset )
DEFINE_CONST_PTR_TYPE( SingleTextureAssetDesc )
DEFINE_CONST_PTR_TYPE( SingleTextureAsset )
DEFINE_CONST_PTR_TYPE( MipMapAssetDesc )
DEFINE_CONST_PTR_TYPE( MipMapAsset )
DEFINE_CONST_PTR_TYPE( GeneratedSingleTextureAssetDesc )
DEFINE_CONST_PTR_TYPE( AnimationAssetDesc)
DEFINE_CONST_PTR_TYPE( AnimationAsset )
DEFINE_CONST_PTR_TYPE( AssetDesc )
DEFINE_CONST_PTR_TYPE( Asset )
DEFINE_CONST_PTR_TYPE( DataArrayAsset )
DEFINE_CONST_PTR_TYPE( DataArrayAssetDesc )
DEFINE_CONST_PTR_TYPE( DataArrayRowAssetDesc )

DEFINE_PTR_TYPE( AnimationAssetDesc )

typedef std::vector< AssetDescConstPtr >   AssetDescVec;
typedef std::vector< AssetConstPtr >       AssetVec;

typedef std::vector< TextureAssetDescConstPtr > TextureAssetDescVec;

} // bv