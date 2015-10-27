#pragma once

#include "SingleTextureAsset.h"
#include "MipMapAsset.h"
#include "Assets/Asset.h"

#include "Assets/AssetManager.h"		// Only for LoadTypedAsset template specialization

namespace bv
{

// *******************************
// Implements texture asset with original image and all its mimmaps.
class TextureAsset : public Asset, public std::enable_shared_from_this< TextureAsset >
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

	bool						HasMipMaps		() const;

	static TextureAssetConstPtr	Create			( const SingleTextureAssetConstPtr & originalTexture, const MipMapAssetConstPtr & mipMaps );
	
	virtual const std::string &	GetUID			() const override;

	static const std::string &	UID				();

private:

	explicit					TextureAsset	( const SingleTextureAssetConstPtr & originalTexture, const MipMapAssetConstPtr & mipMaps );

	SingleTextureAssetConstPtr	m_originalTexture;
	MipMapAssetConstPtr			m_mipMaps;
};



} // bv