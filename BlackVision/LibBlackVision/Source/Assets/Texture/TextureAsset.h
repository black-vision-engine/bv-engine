#pragma once

#include "SingleTextureAsset.h"
#include "MipMapAsset.h"
#include "Assets/Asset.h"

namespace bv
{

class TextureResource;
DEFINE_CONST_PTR_TYPE( TextureResource )

// *******************************
// Implements texture resource with original image and all its mimmaps.
class TextureResource : public model::Resource, public std::enable_shared_from_this< TextureResource >
{
	static const std::string		uid;

protected:
	virtual VoidConstPtr			QueryThis		() const override;

public:

	// *******************************
	// Returns original image without any transformations.
	SingleTextureResourceConstPtr	GetOriginal		() const;

	// *******************************
	// Returns class representation of mimmaps built from the original image.
	MipMapResourceConstPtr			GetMipMaps		() const;

	static TextureResourceConstPtr	Create			( const SingleTextureResourceConstPtr & originalTexture, const MipMapResourceConstPtr & mipMaps );

	explicit						TextureResource	( const SingleTextureResourceConstPtr & originalTexture, const MipMapResourceConstPtr & mipMaps );
	
	virtual const std::string &		GetUID			() const override;

	static const std::string &		UID				();

private:

	SingleTextureResourceConstPtr	m_originalTexture;
	MipMapResourceConstPtr			m_mipMaps;
};

} // bv