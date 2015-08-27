#include "AnimationAssetDescriptor.h"

#include "IO/DirIO.h"


namespace bv {

const std::string AnimationAssetDesc::uid = "ANIMATION_ASSET_DESC";

// ***********************
//
void                AnimationAssetDesc::Serialize       ( SerializeObject & sob ) const
{
sob.SetName( "asset" );
    sob.SetValue( "type", "anim" );
    
    for( auto frame : GetFrames() )
        frame->Serialize( sob );
                
sob.Pop();
}

// ***********************
//
ISerializableConstPtr     AnimationAssetDesc::Create          ( DeserializeObject & dob )
{
    auto frames = dob.LoadProperties< const TextureAssetDesc >( "asset" );

    return AnimationAssetDescConstPtr( new AnimationAssetDesc ( frames ) );
}

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
AnimationAssetDesc::AnimationAssetDesc	( const std::vector< TextureAssetDescConstPtr > & frames )
    : m_frames( frames )
{
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

//// *******************************
////
//AnimationAssetDescConstPtr			AnimationAssetDesc::Create ( const std::vector< TextureAssetDescConstPtr > & frames )
//{
//    return AnimationAssetDescConstPtr( new AnimationAssetDesc ( frames ) );
//}

// *******************************
//
AnimationAssetDescConstPtr		AnimationAssetDesc::CreateFromDirFrames( const std::string & path, const std::string & filter )
{
    auto files = Dir::ListFiles( path, filter );

    if ( files.size() > 0 )
    {
        return AnimationAssetDescConstPtr( new AnimationAssetDesc ( files ) );
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
