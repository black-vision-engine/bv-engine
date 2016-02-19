#include "stdafx.h"

#include "VideoStreamAssetDescriptor.h"
#include <cassert>			//@todo delete after implementing all functions

#include "Assets/Texture/TextureUtils.h"

// Fixme: Hack. Path in VideoStream descriptor is used to load video.
// There is no way to convert it to ProjectManager path.
#include "ProjectManager.h"

namespace bv
{

const std::string		VideoStreamAssetDesc::uid = "VIDEO_STREAM_ASSET_DESC";


// ***********************
//
void                VideoStreamAssetDesc::Serialize       ( ISerializer& ser ) const
{
ser.EnterChild( "asset" );
    ser.SetAttribute( "type", UID() );
	ser.SetAttribute( "path", m_streamPath );
	//ser.SetAttribute( "format", std::to_string( static_cast< int >( m_textureFormat ) ) );

	//if( m_width != 0 && m_height != 0 )
	//{
	//	ser.SetAttribute( "width", std::to_string( m_width ) );
	//	ser.SetAttribute( "height", std::to_string( m_height ) );
	//	ser.SetAttribute( "frameRate", std::to_string( m_frameRate ) );
	//	ser.SetAttribute( "videoFormat", std::to_string( static_cast< int >( m_videoFormat ) ) );
	//}

ser.ExitChild();
}

// ***********************
//
ISerializableConstPtr VideoStreamAssetDesc::Create          ( const IDeserializer& deser )
{
	auto path = deser.GetAttribute( "path" );
	
    //auto format = static_cast< TextureFormat >( stoul( deser.GetAttribute( "format" ) ) );
    auto format = TextureFormat::F_A8R8G8B8;

	return Create( path, format );
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

// *******************************
//
std::string				VideoStreamAssetDesc::GetKey      () const
{
    return m_streamPath;
}

// ***********************
//
SizeType                VideoStreamAssetDesc::EstimateMemoryUsage () const
{
    UInt32 pixelSize = TextureUtils::ToBPP( m_textureFormat ) / 8;

    return 4 * pixelSize * m_height * m_width;
}


// ***********************
//
VideoStreamAssetDescConstPtr VideoStreamAssetDesc::Create	( const std::string & streamPath, TextureFormat textureFormat )
{
	return std::make_shared< VideoStreamAssetDesc >( streamPath, textureFormat );
}

// ***********************
//
VideoStreamAssetDescConstPtr VideoStreamAssetDesc::Create	( const std::string & streamPath, TextureFormat textureFormat, UInt32 width, UInt32 height, Float64 frameRate, VideoPixelFormat videoFormat )
{
	return std::make_shared< VideoStreamAssetDesc >( streamPath, textureFormat, width, height, frameRate, videoFormat );
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
VideoStreamAssetDesc::VideoStreamAssetDesc					( const std::string & streamPath, TextureFormat textureFormat, UInt32 width, UInt32 height, Float64 frameRate, VideoPixelFormat videoFormat )
	: m_streamPath( streamPath )
	, m_width( width )
	, m_height( height )
	, m_frameRate( frameRate )
	, m_textureFormat( textureFormat )
	, m_videoFormat( videoFormat )
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

// ***********************
//
VideoPixelFormat		VideoStreamAssetDesc::GetVideoFormat	() const
{
	return m_videoFormat;
}

} // bv