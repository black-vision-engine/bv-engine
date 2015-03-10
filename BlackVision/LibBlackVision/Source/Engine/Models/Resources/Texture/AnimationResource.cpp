#include "AnimationResource.h"

namespace bv
{

const std::string AnimationResource::uid = "ANIMATION_RESOURCE";

// *******************************
// 
AnimationResourceConstPtr	AnimationResource::Create( const std::vector< TextureResourceConstPtr > & frames )
{
	return std::make_shared< AnimationResource >( frames );
}

// *******************************
// 
AnimationResource::AnimationResource( const std::vector< TextureResourceConstPtr > & frames )
	: m_frames( frames )
{}

// *******************************
// 
TextureResourceConstPtr	AnimationResource::GetFrame( SizeType i ) const
{
	return m_frames[ i ];
}

// *******************************
// 
SizeType AnimationResource::GetFramesNum() const
{
	return m_frames.size();
}

// *******************************
// 
const std::string &	AnimationResource::GetUID() const
{
	return AnimationResource::uid;
}

// *******************************
// 
const std::string &	AnimationResource::UID()
{
	return AnimationResource::uid;
}

// *******************************
// 
VoidConstPtr AnimationResource::QueryThis() const
{
	return shared_from_this();
}

} // bv