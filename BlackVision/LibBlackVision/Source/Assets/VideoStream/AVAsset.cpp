#include "stdafx.h"

#include "AVAsset.h"



#include "Memory/MemoryLeaks.h"



namespace bv
{

// ******************************
//
const std::string AVAsset::uid = "AV_ASSET";

// ******************************
//
AVAssetConstPtr AVAsset::Create( const std::string & streamPath, TextureFormat textureFormat, UInt32 width, UInt32 height, Float64 frameRate, VideoPixelFormat videoFormat )
{
	return std::make_shared< AVAsset >( streamPath, textureFormat, width, height, frameRate, videoFormat );
}

// ******************************
//
AVAsset::AVAsset( const std::string & streamPath, TextureFormat textureFormat, UInt32 width, UInt32 height, Float64 frameRate, VideoPixelFormat videoFormat )
	: m_streamPath( streamPath )
    , m_textureFormat( textureFormat )
    , m_width( width )
    , m_height( height )
    , m_frameRate( frameRate )
    , m_videoFormat( videoFormat )
{
}

// *******************************
// 
const std::string &	AVAsset::GetUID() const
{
	return AVAsset::uid;
}

// *******************************
// 
const std::string &	AVAsset::UID()
{
	return AVAsset::uid;
}

// ***********************
//
const std::string & AVAsset::GetStreamPath	() const
{
	return m_streamPath;
}

// ***********************
//
UInt32              AVAsset::GetWidth		() const
{
	return m_width;
}

// ***********************
//
UInt32              AVAsset::GetHeight		() const
{
	return m_height;
}

// ***********************
//
Float64             AVAsset::GetFrameRate	() const
{
	return m_frameRate;
}

// ***********************
//
TextureFormat       AVAsset::GetTextureFormat	() const
{
	return m_textureFormat;
}

// ***********************
//
VideoPixelFormat    AVAsset::GetVideoFormat	() const
{
	return m_videoFormat;
}

// ***********************
//
bool                AVAsset::IsVideoEnabled	() const
{
    return m_videoEnabled;
}

// ***********************
//
bool                AVAsset::IsAudioEnabled	() const
{
    return m_audioEnabled;
}

// *******************************
// 
VoidConstPtr        AVAsset::QueryThis() const
{
	return shared_from_this();
}

} //bv