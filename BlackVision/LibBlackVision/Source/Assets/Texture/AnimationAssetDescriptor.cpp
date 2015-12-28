#include "AnimationAssetDescriptor.h"
#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"

#include <cassert>

namespace bv {

const std::string AnimationAssetDesc::uid = "ANIMATION_ASSET_DESC";


// ***********************
//
void                AnimationAssetDesc::Serialize       ( ISerializer& sob ) const
{
sob.EnterChild( "asset" );
    sob.SetAttribute( "type", UID() );
    sob.SetAttribute( "path", m_path );
    sob.SetAttribute( "filter", m_filter );
sob.ExitChild();
}

// ***********************
//
ISerializableConstPtr     AnimationAssetDesc::Create          ( const IDeserializer& dob )
{
    return AnimationAssetDescConstPtr( new AnimationAssetDesc( dob.GetAttribute( "path" ), dob.GetAttribute( "filter" ) ) );
}

//// ***********************
////
//void AnimationAssetDesc::Deserialize     ( const IDeserializer& /*sob*/ )
//{
//    assert( false );
//}

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





//const std::vector< TextureAssetDescConstPtr > & AnimationAssetDesc::GetFrames		() const
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
