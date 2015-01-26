#pragma once

#include "SingleTextureResource.h"
#include "MipMapResource.h"
#include "Engine/Models/Resources/IResource.h"

namespace bv
{

class TextureResource;
DEFINE_CONST_PTR_TYPE( TextureResource )

// *******************************
// Implements texture resource with original image and all its mimmaps.
class TextureResource : public model::IResourceNEW
{
public:

	// *******************************
	// Returns original image without any transformations.
	SingleTextureResourceConstPtr	GetOriginal		() const;

	// *******************************
	// Returns class representation of mimmaps built from the original image.
	MipMapResourceConstPtr			GetMipMaps		() const;

	static TextureResourceConstPtr	Create			( const SingleTextureResourceConstPtr & originalTexture, const MipMapResourceConstPtr & mipMaps );

	explicit						TextureResource	( const SingleTextureResourceConstPtr & originalTexture, const MipMapResourceConstPtr & mipMaps );
	

private:

	SingleTextureResourceConstPtr	m_originalTexture;
	MipMapResourceConstPtr			m_mipMaps;
};

} // bv