#include "AnimationResourceDescriptor.h"

#include "System/DirIO.h"


namespace bv { namespace model {

const std::string AnimationResourceDesc::uid = "ANIMATION_RESOURCE_DESC";

// *******************************
//
AnimationResourceDesc::AnimationResourceDesc							( const std::vector< std::string > & frames )
{
	for( auto f : frames )
	{
		m_frames.push_back( TextureResourceDesc::Create( f, false ) );
	}
}

// *******************************
//
AnimationResourceDesc::~AnimationResourceDesc							()
{
}

// *******************************
//
const std::vector< TextureResourceDescConstPtr > & AnimationResourceDesc::GetFrames		() const
{
    return m_frames;
}

// *******************************
//
AnimationResourceDescConstPtr		AnimationResourceDesc::CreateFromDirFrames( const std::string & path, const std::string & filter )
{
    auto files = Dir::ListFiles( path, filter );

    if ( files.size() > 0 )
    {
        return std::make_shared< AnimationResourceDesc >( files );
    }

    return nullptr;
}

// *******************************
//
VoidConstPtr AnimationResourceDesc::QueryThis() const
{
	return shared_from_this();
}

// *******************************
//
const std::string &	AnimationResourceDesc::GetUID() const
{
	return AnimationResourceDesc::UID();
}

// *******************************
//
bool AnimationResourceDesc::IsCacheable() const
{
	return false;
}

// *******************************
//
const std::string &	AnimationResourceDesc::UID()
{
	return AnimationResourceDesc::uid;
}

} //model
} //bv
