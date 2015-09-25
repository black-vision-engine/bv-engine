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

DEFINE_PTR_TYPE( AnimationAssetDesc )


} // bv