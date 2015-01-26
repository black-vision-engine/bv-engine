#include "SingleTextureResourceDescriptor.h"

namespace bv
{

const std::string SingleTextureResourceDesc::uid = "SINGLE_TEXTURE_RESOURCE_DESC";

// ***********************
//
const std::string &	SingleTextureResourceDesc::UID() const
{
	return SingleTextureResourceDesc::uid;
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

} // bv

