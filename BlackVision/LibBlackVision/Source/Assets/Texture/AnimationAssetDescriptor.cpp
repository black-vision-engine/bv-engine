#include "AnimationAssetDescriptor.h"
#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"
#include "Serialization/SerializationHelper.h"

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
    
    if( m_numFrames < std::numeric_limits<UInt32>::max() )
        sob.SetAttribute( "numFrames", std::to_string( m_numFrames ) );

    sob.SetAttribute( "filter", m_filter );
sob.ExitChild();
}

// ***********************
//
ISerializableConstPtr     AnimationAssetDesc::Create          ( const IDeserializer& dob )
{
    //return AnimationAssetDescConstPtr( new AnimationAssetDesc( dob.GetAttribute( "path" ), dob.GetAttribute( "filter" ) ) );
    SizeType numFrames = SerializationHelper::String2T( dob.GetAttribute( "numFrames" ), std::numeric_limits<UInt32>::max() );
    return AnimationAssetDescConstPtr( new AnimationAssetDesc( dob.GetAttribute( "path" ), numFrames, dob.GetAttribute( "filter" ) ) );
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
    , m_numFrames( 0 )
{}

// *******************************
//
AnimationAssetDesc::AnimationAssetDesc							( const std::string & path, SizeType numFrames, const std::string & filter )
    : m_path( path )
    , m_filter( filter )
    , m_numFrames( numFrames )
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
AnimationAssetDescConstPtr		AnimationAssetDesc::Create( const std::string & path, SizeType numFrames, const std::string & filter )
{
    return AnimationAssetDescConstPtr( new AnimationAssetDesc ( path, numFrames, filter ) );
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

// *******************************
//
SizeType             AnimationAssetDesc::GetNumFrames() const
{
    return m_numFrames;
}

} //bv
