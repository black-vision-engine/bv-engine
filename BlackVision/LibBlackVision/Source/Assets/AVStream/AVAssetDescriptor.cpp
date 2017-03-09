#include "stdafx.h"

#include "AVAssetDescriptor.h"
#include <cassert>			//@todo delete after implementing all functions

#include "Assets/Texture/TextureUtils.h"

// Fixme: Hack. Path in VideoStream descriptor is used to load video.
// There is no way to convert it to ProjectManager path.
#include "ProjectManager.h"




#include "Memory/MemoryLeaks.h"



namespace bv {

const std::string		AVAssetDesc::uid = "AV_ASSET_DESC";


// ***********************
//
void                    AVAssetDesc::Serialize       ( ISerializer& ser ) const
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
ISerializableConstPtr AVAssetDesc::Create          ( const IDeserializer& deser )
{
	auto path = deser.GetAttribute( "path" );
	
    //auto format = static_cast< TextureFormat >( stoul( deser.GetAttribute( "format" ) ) );
    auto format = TextureFormat::F_A8R8G8B8;

	return Create( path, format );
}

// ***********************
//
const std::string &		AVAssetDesc::GetUID		() const
{
	return AVAssetDesc::UID();
}

// ***********************
const std::string &		AVAssetDesc::UID			()
{
	return AVAssetDesc::uid;
}

// ***********************
//
VoidConstPtr			AVAssetDesc::QueryThis		() const
{
	return shared_from_this();
}

// ***********************
//
bool					AVAssetDesc::IsCacheable	() const
{
	return false;
}

// *******************************
//
std::string				AVAssetDesc::GetKey      () const
{
    return m_streamPath + std::to_string( Path::GetTimestamp( ProjectManager::GetInstance()->ToAbsPath( m_streamPath ) ) );
}

// ***********************
//
SizeType                AVAssetDesc::EstimateMemoryUsage () const
{
    UInt32 pixelSize = TextureUtils::ToBPP( m_textureFormat ) / 8;

    return 4 * pixelSize * m_height * m_width;
}

// ***********************
//
AVAssetDescConstPtr AVAssetDesc::Create	( const std::string & streamPath, TextureFormat textureFormat )
{
	return std::make_shared< AVAssetDesc >( streamPath, textureFormat );
}

// ***********************
//
AVAssetDescConstPtr AVAssetDesc::Create	( const std::string & streamPath, TextureFormat textureFormat, UInt32 width, UInt32 height, Float64 frameRate, VideoPixelFormat videoFormat )
{
	return std::make_shared< AVAssetDesc >( streamPath, textureFormat, width, height, frameRate, videoFormat );
}

// ***********************
//
AVAssetDesc::AVAssetDesc					( const std::string & streamPath, TextureFormat textureFormat )
	: m_streamPath( streamPath )
	, m_width( 0 )
	, m_height( 0 )
	, m_frameRate( 0 )
	, m_textureFormat( textureFormat )
{
}

// ***********************
//
AVAssetDesc::AVAssetDesc					( const std::string & streamPath, TextureFormat textureFormat, UInt32 width, UInt32 height, Float64 frameRate, VideoPixelFormat videoFormat )
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
const std::string &		AVAssetDesc::GetStreamPath	() const
{
	return m_streamPath;
}

// ***********************
//
UInt32					AVAssetDesc::GetWidth		() const
{
	return m_width;
}

// ***********************
//
UInt32					AVAssetDesc::GetHeight		() const
{
	return m_height;
}

// ***********************
//
Float64					AVAssetDesc::GetFrameRate	() const
{
	return m_frameRate;
}

// ***********************
//
TextureFormat			AVAssetDesc::GetTextureFormat	() const
{
	return m_textureFormat;
}

// ***********************
//
VideoPixelFormat		AVAssetDesc::GetVideoFormat	() const
{
	return m_videoFormat;
}

} // bv