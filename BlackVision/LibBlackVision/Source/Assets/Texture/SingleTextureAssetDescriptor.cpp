#include "stdafx.h"

#include "SingleTextureAssetDescriptor.h"
#include "TextureUtils.h"

#include <assert.h>

#include "Serialization/ISerializer.h"

#include "System/Path.h"
#include "ProjectManager.h"


namespace bv
{

const std::string SingleTextureAssetDesc::uid = "SINGLE_TEXTURE_ASSET_DESC";

// ***********************
//
void                SingleTextureAssetDesc::Serialize       ( ISerializer& ser ) const
{
ser.EnterChild( "asset" );
    
    ser.SetAttribute( "type", UID() );
    ser.SetAttribute( "path", GetImagePath() );

ser.ExitChild();
}

// ***********************
//
ISerializablePtr        SingleTextureAssetDesc::Create          ( IDeserializer& /*dob*/ )
{
    assert( !"implement me" );
    return nullptr;
}

// ***********************
//
const std::string &	    SingleTextureAssetDesc::GetUID() const
{
	return SingleTextureAssetDesc::UID();
}

// ***********************
const std::string &	    SingleTextureAssetDesc::UID()
{
	return SingleTextureAssetDesc::uid;
}

// ***********************
//
VoidConstPtr            SingleTextureAssetDesc::QueryThis() const
{
	return shared_from_this();
}

// ***********************
//
bool                    SingleTextureAssetDesc::IsCacheable	() const
{
	return m_isCacheable;
}

// ***********************
//
std::string	            SingleTextureAssetDesc::GetKey	() const
{
    if( m_key.empty() )
        m_key = ComputeKey();

    return m_key;
}

// ***********************
//
std::string         SingleTextureAssetDesc::ComputeKey() const
{
    return m_imagePath + std::to_string( Path::GetTimestamp( ProjectManager::GetInstance()->ToAbsPath( m_imagePath ) ) );
}

// ***********************
//
SingleTextureAssetDescConstPtr SingleTextureAssetDesc::Create( const std::string & imagePath, UInt32 width, UInt32 height, TextureFormat format, bool isCacheable )
{
	return SingleTextureAssetDescConstPtr( new SingleTextureAssetDesc( imagePath, width, height, format, isCacheable ) );
}

// ***********************
//
SingleTextureAssetDesc::SingleTextureAssetDesc( const std::string & imagePath, UInt32 width, UInt32 height, TextureFormat format, bool isCacheable )
	: m_imagePath( imagePath )
	, m_width( width )
	, m_height( height )
	, m_format( format )
	, m_isCacheable( isCacheable )
{}

// ***********************
//
const std::string & SingleTextureAssetDesc::GetImagePath() const
{
	return m_imagePath;
}

// ***********************
//
UInt32 SingleTextureAssetDesc::GetWidth() const
{
	return m_width;
}

// ***********************
//
UInt32 SingleTextureAssetDesc::GetHeight() const
{
	return m_height;
}

// ***********************
//
TextureFormat SingleTextureAssetDesc::GetFormat() const
{
	return m_format;
}

// ***********************
//
SizeType        SingleTextureAssetDesc::EstimateMemoryUsage () const
{
    UInt32 pixelSize = TextureUtils::ToBPP( m_format ) / 8;
    return m_width * m_height * pixelSize;
}



} // bv

