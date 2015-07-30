#include "AnimationAssetDescriptor.h"

#include "IO/DirIO.h"


namespace bv {

const std::string AnimationAssetDesc::uid = "ANIMATION_ASSET_DESC";

// *******************************
//
AnimationAssetDesc::AnimationAssetDesc							( const std::vector< std::string > & frames )
{
	for( auto f : frames )
	{
		m_frames.push_back( TextureAssetDesc::Create( f, true ) );
	}
}

// *******************************
//
AnimationAssetDesc::~AnimationAssetDesc							()
{
}

// *******************************
//
const std::vector< TextureAssetDescConstPtr > & AnimationAssetDesc::GetFrames		() const
{
    return m_frames;
}

// *******************************
//
AnimationAssetDescConstPtr		AnimationAssetDesc::CreateFromDirFrames( const std::string & path, const std::string & filter )
{
    auto files = Dir::ListFiles( path, filter );

    if ( files.size() > 0 )
    {
        return std::make_shared< AnimationAssetDesc >( files );
    }

    return nullptr;
}

// *******************************
//
VoidConstPtr AnimationAssetDesc::QueryThis() const
{
	return shared_from_this();
}

// *******************************
//
const std::string &	AnimationAssetDesc::GetUID() const
{
	return AnimationAssetDesc::UID();
}

// *******************************
//
bool AnimationAssetDesc::IsCacheable() const
{
	return false;
}

// *******************************
//
const std::string &	AnimationAssetDesc::UID()
{
	return AnimationAssetDesc::uid;
}

} //bv
