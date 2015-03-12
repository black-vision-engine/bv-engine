#include "SingleTextureResourceDescriptor.h"

namespace bv
{

const std::string SingleTextureResourceDesc::uid = "SINGLE_TEXTURE_RESOURCE_DESC";

// ***********************
//
const std::string &	SingleTextureResourceDesc::GetUID() const
{
	return SingleTextureResourceDesc::UID();
}

// ***********************
const std::string &	SingleTextureResourceDesc::UID()
{
	return SingleTextureResourceDesc::uid;
}

// ***********************
//
VoidConstPtr SingleTextureResourceDesc::QueryThis() const
{
	return shared_from_this();
}

// ***********************
//
bool SingleTextureResourceDesc::IsCacheable	() const
{
	return m_isCacheable;
}

// ***********************
//
SingleTextureResourceDescConstPtr SingleTextureResourceDesc::Create( const std::string & imagePath, UInt32 width, UInt32 height, TextureFormat format, bool isCacheable )
{
	return std::make_shared< SingleTextureResourceDesc >( imagePath, width, height, format, isCacheable );
}

// ***********************
//
SingleTextureResourceDesc::SingleTextureResourceDesc( const std::string & imagePath, UInt32 width, UInt32 height, TextureFormat format, bool isCacheable )
	: m_imagePath( imagePath )
	, m_width( width )
	, m_height( height )
	, m_format( format )
	, m_isCacheable( isCacheable )
{}

// ***********************
//
const std::string & SingleTextureResourceDesc::GetImagePath() const
{
	return m_imagePath;
}

// ***********************
//
UInt32 SingleTextureResourceDesc::GetWidth() const
{
	return m_width;
}

// ***********************
//
UInt32 SingleTextureResourceDesc::GetHeight() const
{
	return m_height;
}

// ***********************
//
TextureFormat SingleTextureResourceDesc::GetFormat() const
{
	return m_format;
}


} // bv

