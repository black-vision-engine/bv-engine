#include "SingleTextureAssetDescriptor.h"

namespace bv
{

const std::string SingleTextureAssetDesc::uid = "SINGLE_TEXTURE_ASSET_DESC";

// ***********************
//
const std::string &	SingleTextureAssetDesc::GetUID() const
{
	return SingleTextureAssetDesc::UID();
}

// ***********************
const std::string &	SingleTextureAssetDesc::UID()
{
	return SingleTextureAssetDesc::uid;
}

// ***********************
//
VoidConstPtr SingleTextureAssetDesc::QueryThis() const
{
	return shared_from_this();
}

// ***********************
//
bool SingleTextureAssetDesc::IsCacheable	() const
{
	return m_isCacheable;
}

// ***********************
//
std::string	SingleTextureAssetDesc::GetKey	() const
{
	return m_imagePath;
}

// ***********************
//
SingleTextureAssetDescConstPtr SingleTextureAssetDesc::Create( const std::string & imagePath, UInt32 width, UInt32 height, TextureFormat format, bool isCacheable )
{
	return std::make_shared< SingleTextureAssetDesc >( imagePath, width, height, format, isCacheable );
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


} // bv

