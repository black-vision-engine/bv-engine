#include "AnimationAsset.h"

namespace bv
{

const std::string AnimationAsset::uid = "ANIMATION_ASSET";

// *******************************
// 
AnimationAssetConstPtr	AnimationAsset::Create( const std::vector< TextureAssetConstPtr > & frames )
{
	return AnimationAssetConstPtr( new AnimationAsset( frames ) );
}

// *******************************
// 
AnimationAsset::AnimationAsset( const std::vector< TextureAssetConstPtr > & frames )
	: m_frames( frames )
{}

// *******************************
// 
TextureAssetConstPtr	AnimationAsset::GetFrame( SizeType i ) const
{
	return m_frames[ i ];
}

// *******************************
// 
SizeType AnimationAsset::GetFramesNum() const
{
	return m_frames.size();
}

// *******************************
// 
const std::string &	AnimationAsset::GetUID() const
{
	return AnimationAsset::uid;
}

// *******************************
// 
const std::string &	AnimationAsset::UID()
{
	return AnimationAsset::uid;
}

// *******************************
// 
VoidConstPtr AnimationAsset::QueryThis() const
{
	return shared_from_this();
}

} // bv