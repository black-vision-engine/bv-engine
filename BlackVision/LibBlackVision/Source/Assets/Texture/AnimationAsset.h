#pragma once

#include "TextureAsset.h"
#include "Assets/Asset.h"

#include "Assets/AssetManager.h"			// Only for LoadTypedAsset template specialization


namespace bv
{

/**Implements animation asset.
@details
Loads sequence of textures from directory as animation.

<b>Supported formats:</b> jpg, tga, png, bmp.

-                       | UID                       | Class
----------------------- | ------------------------- | -----------
Descriptor              | ANIMATION_ASSET_DESC      | @ref bv::AnimationAssetDesc
Asset                   | ANIMATION_ASSET           | @ref bv::AnimationAsset


<b>Example serialized descriptor:</b>

@code{.json}
{
    "asset" :
    {
        "filter" : "none",
        "height" : "2001",
        "path" : "sequences\\",
        "type" : "ANIMATION_ASSET_DESC",
        "numFrames" : "",
        "width" : "3000"
    }
}
@endcode

<b>Parameters</b>

Parameter name         	| Type                              | Required   | Description
----------------------- | --------------------------------- | ---------- | --------------------------
type                    | string                            | yes        | Asset descriptor UID.
height                  | UInt32                            | no         | Height of image.
width                   | UInt32                            | no         | Width of image.
filter                  | @ref bv::MipMapFilterType         | yes        | Filter used to create mipmaps.
path                    | string                            | yes        | Path to directory with images.
numFrames               | UInt32                            | no         | Number of images in sequence.

@ingroup Assets */
class AnimationAsset : public Asset, public std::enable_shared_from_this< AnimationAsset >
{
	static const std::string		uid;

protected:
	virtual VoidConstPtr			QueryThis		() const override;

public:

	// *******************************
	// Returns i'th frame
	TextureAssetConstPtr			GetFrame		( SizeType i ) const;

	// *******************************
	// Returns number of frames
	SizeType						GetFramesNum	() const;

	static AnimationAssetConstPtr	Create			( const std::vector< TextureAssetConstPtr > & frames );
	
	virtual const std::string &		GetUID			() const override;

	static const std::string &		UID				();

private:

	explicit						AnimationAsset	( const std::vector< TextureAssetConstPtr > & frames );

	std::vector< TextureAssetConstPtr >	m_frames;
};


} // bv