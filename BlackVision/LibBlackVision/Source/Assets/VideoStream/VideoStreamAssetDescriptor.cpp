#include "VideoStreamAssetDescriptor.h"

namespace bv
{

const std::string		VideoStreamAssetDesc::uid = "VIDEO_STREAM_ASSET_DESC";

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
VideoStreamAssetDescConstPtr VideoStreamAssetDesc::Create	( const std::string & streamPath, TextureFormat textureFormat )
{
	return std::make_shared< VideoStreamAssetDesc >( streamPath, textureFormat );
}

// ***********************
//
VideoStreamAssetDescConstPtr VideoStreamAssetDesc::Create	( const std::string & streamPath, TextureFormat textureFormat, UInt32 width, UInt32 height, Float64 frameRate )
{
	return std::make_shared< VideoStreamAssetDesc >( streamPath, textureFormat, width, height, frameRate );
}

// ***********************
//
VideoStreamAssetDesc::VideoStreamAssetDesc					( const std::string & streamPath, TextureFormat textureFormat )
	: m_streamPath( streamPath )
	, m_width( 0 )
	, m_height( 0 )
	, m_frameRate( 0 )
	, m_textureFormat( textureFormat )
{
}

// ***********************
//
VideoStreamAssetDesc::VideoStreamAssetDesc					( const std::string & streamPath, TextureFormat textureFormat, UInt32 width, UInt32 height, Float64 frameRate )
	: m_streamPath( streamPath )
	, m_width( width )
	, m_height( height )
	, m_frameRate( frameRate )
	, m_textureFormat( textureFormat )
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
UInt32					VideoStreamAssetDesc::GetWidth		() const
{
	return m_width;
}

// ***********************
//
UInt32					VideoStreamAssetDesc::GetHeight		() const
{
	return m_height;
}

// ***********************
//
Float64					VideoStreamAssetDesc::GetFrameRate	() const
{
	return m_frameRate;
}

// ***********************
//
TextureFormat			VideoStreamAssetDesc::GetTextureFormat	() const
{
	return m_textureFormat;
}

} // bv

