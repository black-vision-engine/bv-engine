#pragma once

#include "SingleTextureAsset.h"

#include <vector>

namespace bv
{

// *******************************
// Implements assets of mipmaps levels.
class MipMapAsset
{
public:

	SingleTextureAssetConstPtr		GetLevel		( SizeType level ) const;
	SizeType						GetLevelsNum	() const;

	static MipMapAssetConstPtr		Create			( const std::vector< SingleTextureAssetConstPtr > & mipMaps );

private:

	explicit						MipMapAsset	( const std::vector< SingleTextureAssetConstPtr > & mipMaps );

	std::vector< SingleTextureAssetConstPtr > m_mipMaps;
};

} // bv