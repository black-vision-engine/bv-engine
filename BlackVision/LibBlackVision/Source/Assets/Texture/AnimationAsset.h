#pragma once

#include "TextureAsset.h"
#include "Assets/Asset.h"

namespace bv
{

class AnimationAsset;
DEFINE_CONST_PTR_TYPE( AnimationAsset )

// *******************************
// Implements animation asset.
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

	explicit						AnimationAsset	( const std::vector< TextureAssetConstPtr > & frames );
	
	virtual const std::string &		GetUID			() const override;

	static const std::string &		UID				();

private:

	std::vector< TextureAssetConstPtr >	m_frames;
};

} // bv