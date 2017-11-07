#include "stdafx.h"

#include "AnimationAssetDescriptor.h"
#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"
#include "Serialization/SerializationHelper.h"
#include "Serialization/BV/BVSerializeContext.h"

#include <cassert>



#include "Memory/MemoryLeaks.h"



namespace bv {

const std::string AnimationAssetDesc::uid = "ANIMATION_ASSET_DESC";


// ***********************
//
void                AnimationAssetDesc::Serialize       ( ISerializer& ser ) const
{
    auto context = static_cast<BVSerializeContext*>( ser.GetSerializeContext() );

ser.EnterChild( "asset" );
    ser.SetAttribute( "type", UID() );
    ser.SetAttribute( "path", m_path );
    
    if( context->extendedAssetData )
    {
        ser.SetAttribute( "numFrames", Convert::T2String( m_numFrames ) );
        ser.SetAttribute( "width", Convert::T2String( m_width ) );
        ser.SetAttribute( "height", Convert::T2String( m_height ) );
    }

    ser.SetAttribute( "filter", m_filter );
ser.ExitChild();
}

// ***********************
//
ISerializableConstPtr     AnimationAssetDesc::Create          ( const IDeserializer& dob )
{
    //return AnimationAssetDescConstPtr( new AnimationAssetDesc( dob.GetAttribute( "path" ), dob.GetAttribute( "filter" ) ) );
    SizeType numFrames = Convert::String2T( dob.GetAttribute( "numFrames" ), std::numeric_limits<UInt32>::max() );

    UInt32 width = Convert::String2T( dob.GetAttribute( "width" ), std::numeric_limits<UInt32>::max() );
    UInt32 height = Convert::String2T( dob.GetAttribute( "height" ), std::numeric_limits<UInt32>::max() );

    return AnimationAssetDescConstPtr( new AnimationAssetDesc( dob.GetAttribute( "path" ), numFrames, width, height, dob.GetAttribute( "filter" ) ) );
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
AnimationAssetDesc::AnimationAssetDesc							( const std::string & path, SizeType numFrames, UInt32 width, UInt32 height, const std::string & filter )
    : m_path( path )
    , m_filter( filter )
    , m_numFrames( numFrames )
    , m_width( width )
    , m_height( height )
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
AnimationAssetDescConstPtr		AnimationAssetDesc::Create( const std::string & path, SizeType numFrames, UInt32 width, UInt32 height, const std::string & filter )
{
    return AnimationAssetDescConstPtr( new AnimationAssetDesc ( path, numFrames, width, height, filter ) );
}

// ***********************
//
AnimationAssetDescConstPtr		AnimationAssetDesc::Create ( const std::string & path, SizeType numFrames, const std::string & filter )
{
    return AnimationAssetDescConstPtr( new AnimationAssetDesc ( path, numFrames, std::numeric_limits<UInt32>::max(), std::numeric_limits<UInt32>::max(), filter ) );
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

// ***********************
//
SizeType            AnimationAssetDesc::EstimateMemoryUsage () const
{
    // I assume that textures have 4 bytes per pixel.
    return 4 * m_width * m_height * m_numFrames;
}


} //bv
