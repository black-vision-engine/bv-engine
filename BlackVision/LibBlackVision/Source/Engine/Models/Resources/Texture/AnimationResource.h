#pragma once

#include "TextureResource.h"
#include "Engine/Models/Resources/IResource.h"

namespace bv
{

class AnimationResource;
DEFINE_CONST_PTR_TYPE( AnimationResource )

// *******************************
// Implements animation resource.
class AnimationResource : public model::IResource, public std::enable_shared_from_this< AnimationResource >
{
	static const std::string		uid;

protected:
	virtual VoidConstPtr			QueryThis		() const override;

public:

	// *******************************
	// Returns i'th frame
	TextureResourceConstPtr			GetFrame		( SizeType i ) const;

	// *******************************
	// Returns number of frames
	SizeType						GetFramesNum	() const;

	static AnimationResourceConstPtr Create			( const std::vector< TextureResourceConstPtr > & frames );

	explicit						AnimationResource( const std::vector< TextureResourceConstPtr > & frames );
	
	virtual const std::string &		GetUID			() const override;

	static const std::string &		UID				();

private:

	std::vector< TextureResourceConstPtr >	m_frames;
};

} // bv