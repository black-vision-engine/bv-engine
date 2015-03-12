#pragma once

#include "SingleTextureAsset.h"
#include "MipMapAsset.h"
#include "Assets/Asset.h"

namespace bv
{

class TextureAsset;
DEFINE_CONST_PTR_TYPE( TextureAsset )

// *******************************
// Implements texture resource with original image and all its mimmaps.
class TextureAsset : public model::Asset, public std::enable_shared_from_this< TextureAsset >
{
	static const std::string		uid;

protected:
	virtual VoidConstPtr			QueryThis		() const override;

public:

	// *******************************
	// Returns original image without any transformations.
	SingleTextureAssetConstPtr	GetOriginal		() const;

	// *******************************
	// Returns class representation of mimmaps built from the original image.
	MipMapAssetConstPtr			GetMipMaps		() const;

	static TextureAssetConstPtr	Create			( const SingleTextureAssetConstPtr & originalTexture, const MipMapAssetConstPtr & mipMaps );

	explicit						TextureAsset	( const SingleTextureAssetConstPtr & originalTexture, const MipMapAssetConstPtr & mipMaps );
	
	virtual const std::string &		GetUID			() const override;

	static const std::string &		UID				();

private:

	SingleTextureAssetConstPtr	m_originalTexture;
	MipMapAssetConstPtr			m_mipMaps;
};

} // bv