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
	return true;
}

// ***********************
//
SingleTextureResourceDescConstPtr Create( const std::string & imagePath, UInt32 width, UInt32 height, TextureFormat format )
{
	return std::make_shared< SingleTextureResourceDesc >( imagePath, width, height, format );
}

// ***********************
//
SingleTextureResourceDesc::SingleTextureResourceDesc( const std::string & imagePath, UInt32 width, UInt32 height, TextureFormat format )
	: m_imagePath( imagePath )
	, m_width( width )
	, m_height( height )
	, m_format( format )
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

