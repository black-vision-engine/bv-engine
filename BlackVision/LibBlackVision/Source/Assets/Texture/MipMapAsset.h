#pragma once

#include "SingleTextureAsset.h"

#include <vector>

namespace bv
{

class MipMapAsset;
DEFINE_CONST_PTR_TYPE(MipMapAsset)

// *******************************
// Implements assets of mipmaps levels.
class MipMapAsset
{
public:

	SingleTextureAssetConstPtr		GetLevel		( SizeType level ) const;
	SizeType						GetLevelsNum	() const;

	static MipMapAssetConstPtr		Create			( const std::vector< SingleTextureAssetConstPtr > & mipMaps );

	explicit						MipMapAsset	( const std::vector< SingleTextureAssetConstPtr > & mipMaps );

private:

	std::vector< SingleTextureAssetConstPtr > m_mipMaps;
};

} // bv