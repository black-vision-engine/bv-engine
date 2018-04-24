#pragma once

#include "SingleTextureAsset.h"
#include "MipMapAsset.h"
#include "Assets/Asset.h"

#include "Assets/AssetManager.h"		// Only for LoadTypedAsset template specialization

namespace bv
{


/**@brief Implements texture asset with original image and all its mimmaps.
@details
Loads texture from hard drive.

-                       | UID                       | Class
----------------------- | ------------------------- | -----------
Descriptor              | TEXTURE_ASSET_DESC        | @ref bv::TextureAssetDesc
Asset                   | TEXTURE_ASSET             | @ref bv::TextureAsset

<b>Example serialized descriptor:</b>

@code{.json}
{
    "asset" :
    {
        "estimatedMemoryUsage" : "18009000",
        "filter" : "none",
        "height" : "2001",
        "loading_type" : "ONLY_ORIGINAL",
        "path" : "textures\\crystal.jpg",
        "type" : "TEXTURE_ASSET_DESC",
        "width" : "3000"
    }
}
@endcode

<b>Parameters</b>

Parameter name         	| Type                              | Required   | Description
----------------------- | --------------------------------- | ---------- | --------------------------
type                    | string                            | yes        | Asset descriptor UID.
estimatedMemoryUsage    | SizeType                          | no         | Estimated memory usage of asset after loading.
height                  | UInt32                            | no         | Height of image.
width                   | UInt32                            | no         | Width of image.
loading_type            | @ref bv::TextureAssetLoadingType  | yes        | Load only original texture or load or generate mipmaps.
filter                  | @ref bv::MipMapFilterType         | yes        | Filter used to create mipmaps.
path                    | string                            | yes        | Path to asset. 



@ingroup Assets*/
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