#pragma once

#include "SingleTextureAsset.h"

#include <vector>

namespace bv
{

class MipMapResource;
DEFINE_CONST_PTR_TYPE(MipMapResource)

// *******************************
// Implements resources of mipmaps levels.
class MipMapResource
{
public:

	SingleTextureResourceConstPtr	GetLevel		( SizeType level ) const;
	SizeType						GetLevelsNum	() const;

	static MipMapResourceConstPtr	Create			( const std::vector< SingleTextureResourceConstPtr > & mipMaps );

	explicit						MipMapResource	( const std::vector< SingleTextureResourceConstPtr > & mipMaps );

private:

	std::vector< SingleTextureResourceConstPtr > m_mipMaps;
};

} // bv