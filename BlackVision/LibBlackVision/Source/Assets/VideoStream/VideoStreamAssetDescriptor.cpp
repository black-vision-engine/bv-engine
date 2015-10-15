#include "VideoStreamAssetDescriptor.h"
#include <cassert>			//@todo delete after implementing all functions

namespace bv
{

const std::string		VideoStreamAssetDesc::uid = "VIDEO_STREAM_ASSET_DESC";


// ***********************
//
void                VideoStreamAssetDesc::Serialize       ( ISerializer& /*sob*/ ) const
{
	assert( !"Implement me" );
}

// ***********************
//
void VideoStreamAssetDesc::Deserialize     ( ISerializer& /*sob*/ )
{

}

// ***********************
//
ISerializableConstPtr VideoStreamAssetDesc::Create          ( ISerializer& /*dob*/ )
{
	assert( !"Implement me" );
}


// ***********************
//
const std::string &		VideoStreamAssetDesc::GetUID		() const
{
	return VideoStreamAssetDesc::UID();
}

// ***********************
const std::string &		VideoStreamAssetDesc::UID			()
{
	return VideoStreamAssetDesc::uid;
}

// ***********************
//
VoidConstPtr			VideoStreamAssetDesc::QueryThis		() const
{
	return shared_from_this();
}

// ***********************
//
bool					VideoStreamAssetDesc::IsCacheable	() const
{
	return false;
}

// ***********************
//
VideoStreamAssetDescConstPtr VideoStreamAssetDesc::Create	( const std::string & streamPath )
{
	return std::make_shared< VideoStreamAssetDesc >( streamPath );
}

// ***********************
//
VideoStreamAssetDescConstPtr VideoStreamAssetDesc::Create	( const std::string & streamPath, UInt32 width, UInt32 height, UInt32 frameRate )
{
	return std::make_shared< VideoStreamAssetDesc >( streamPath, width, height, frameRate );
}

// ***********************
//
VideoStreamAssetDesc::VideoStreamAssetDesc					( const std::string & streamPath )
	: m_streamPath( streamPath )
	, m_width( 0 )
	, m_height( 0 )
	, m_frameRate( 0 )
{
}

// ***********************
//
VideoStreamAssetDesc::VideoStreamAssetDesc					( const std::string & streamPath, UInt32 width, UInt32 height, UInt32 frameRate )
	: m_streamPath( streamPath )
	, m_width( width )
	, m_height( height )
	, m_frameRate( frameRate )
{
}

// ***********************
//
const std::string &		VideoStreamAssetDesc::GetStreamPath	() const
{
	return m_streamPath;
}

// ***********************
//
UInt32					VideoStreamAssetDesc::GetWidth	() const
{
	return m_width;
}

// ***********************
//
UInt32					VideoStreamAssetDesc::GetHeight	() const
{
	return m_height;
}

} // bv

