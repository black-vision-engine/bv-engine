#include "stdafx.h"

#include "VideoStreamAsset.h"

namespace bv
{

// ******************************
//
const std::string VideoStreamAsset::uid = "VIDEO_STREAM_ASSET";

// ******************************
//
VideoStreamAssetConstPtr VideoStreamAsset::Create( const std::string & streamPath, TextureFormat textureFormat, UInt32 width, UInt32 height, Float64 frameRate, VideoPixelFormat videoFormat )
{
	return std::make_shared< VideoStreamAsset >( streamPath, textureFormat, width, height, frameRate, videoFormat );
}

// ******************************
//
VideoStreamAsset::VideoStreamAsset( const std::string & streamPath, TextureFormat textureFormat, UInt32 width, UInt32 height, Float64 frameRate, VideoPixelFormat videoFormat )
	: m_streamPath( streamPath ),
    m_textureFormat( textureFormat ),
    m_width( width ),
    m_height( height ),
    m_frameRate( frameRate ),
    m_videoFormat( videoFormat )
{
}

// *******************************
// 
const std::string &	VideoStreamAsset::GetUID() const
{
	return VideoStreamAsset::uid;
}

// *******************************
// 
const std::string &	VideoStreamAsset::UID()
{
	return VideoStreamAsset::uid;
}

// ***********************
//
const std::string & VideoStreamAsset::GetStreamPath	() const
{
	return m_streamPath;
}

// ***********************
//
UInt32              VideoStreamAsset::GetWidth		() const
{
	return m_width;
}

// ***********************
//
UInt32              VideoStreamAsset::GetHeight		() const
{
	return m_height;
}

// ***********************
//
Float64             VideoStreamAsset::GetFrameRate	() const
{
	return m_frameRate;
}

// ***********************
//
TextureFormat       VideoStreamAsset::GetTextureFormat	() const
{
	return m_textureFormat;
}

// ***********************
//
VideoPixelFormat    VideoStreamAsset::GetVideoFormat	() const
{
	return m_videoFormat;
}

// *******************************
// 
VoidConstPtr        VideoStreamAsset::QueryThis() const
{
	return shared_from_this();
}

} //bv