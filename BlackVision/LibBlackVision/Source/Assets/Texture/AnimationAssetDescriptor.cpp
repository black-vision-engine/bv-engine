#include "AnimationAssetDescriptor.h"

namespace bv {

const std::string AnimationAssetDesc::uid = "ANIMATION_ASSET_DESC";

// ***********************
//
void                AnimationAssetDesc::Serialize       ( SerializeObject & sob ) const
{
sob.SetName( "asset" );
    sob.SetValue( "type", "anim" );
    sob.SetValue( "path", m_path );
    sob.SetValue( "filter", m_filter );
sob.Pop();
}

// ***********************
//
ISerializableConstPtr     AnimationAssetDesc::Create          ( DeserializeObject & dob )
{
    return AnimationAssetDescConstPtr( new AnimationAssetDesc( dob.GetValue( "path" ), dob.GetValue( "filter" ) ) );
}

// *******************************
//
AnimationAssetDesc::AnimationAssetDesc							( const std::string & path, const std::string & filter )
    : m_path( path )
    , m_filter( filter )
{}

//// *******************************
////
//AnimationAssetDesc::AnimationAssetDesc	( const std::vector< TextureAssetDescConstPtr > & frames )
//    : m_frames( frames )
//    , m_path( "" )
//{
//}


// *******************************
//
AnimationAssetDesc::~AnimationAssetDesc							()
{
}

// *******************************
//
//const std::vector< TextureAssetDescConstPtr > & AnimationAssetDesc::GetFrames		() const
//{
//    return m_frames;
//}

//// *******************************
////
//AnimationAssetDescConstPtr			AnimationAssetDesc::Create ( const std::vector< TextureAssetDescConstPtr > & frames )
//{
//    return AnimationAssetDescConstPtr( new AnimationAssetDesc ( frames ) );
//}

// *******************************
//
std::string                     AnimationAssetDesc::GetKey      () const
{
    //return m_path + '/' + m_filter;
    return m_path; // assuming there's only one sequence in one directory
}

// *******************************
//
AnimationAssetDescConstPtr		AnimationAssetDesc::Create( const std::string & path, const std::string & filter )
{
    return AnimationAssetDescConstPtr( new AnimationAssetDesc ( path, filter ) );
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

// *******************************
//
std::string         AnimationAssetDesc::GetPath		() const
{
    return m_path;
}

// *******************************
//
std::string         AnimationAssetDesc::GetFilter   () const
{
    return m_filter;
}

} //bv
