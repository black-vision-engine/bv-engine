#include "TextureResourceDescriptor.h"

namespace bv
{

const std::string TextureResourceDesc::uid = "TEXTURE_RESOURCE_DESC";

// ***********************
//
const std::string &	TextureResourceDesc::UID() const
{
	return TextureResourceDesc::uid;
}

// ***********************
//
bool TextureResourceDesc::IsCacheable() const
{
	return true;
}

// ***********************
//
TextureResourceDescConstPtr	Create( const SingleTextureResourceDescConstPtr & origDesc, const std::vector< SingleTextureResourceDescConstPtr > & mipmapsDesc )
{
	return std::make_shared< TextureResourceDesc >( origDesc, mipmapsDesc );
}

// ***********************
//
TextureResourceDescConstPtr	Create( const SingleTextureResourceDescConstPtr & origDesc, bool generateMipMaps )
{
	return std::make_shared< TextureResourceDesc >( origDesc, generateMipMaps );
}

// ***********************
//
TextureResourceDesc::TextureResourceDesc( const SingleTextureResourceDescConstPtr & origDesc, const std::vector< SingleTextureResourceDescConstPtr > & mipmapsDesc )
	: m_originalTextureDesc( origDesc )
	, m_mipMapsDescs( mipmapsDesc )
	, m_loadingType( TextureResourceLoadingType::LOAD_ORIGINAL_TEXTURE_AND_MIP_MAPS )
{}

// ***********************
//
TextureResourceDesc::TextureResourceDesc( const SingleTextureResourceDescConstPtr & origDesc, bool generateMipMaps )
	: m_originalTextureDesc( origDesc )
	, m_mipMapsDescs()
{
	if( generateMipMaps )
		m_loadingType = TextureResourceLoadingType::LOAD_ORIGINAL_TEXTURE_AND_GENERATE_MIP_MAPS;
	else
		m_loadingType = TextureResourceLoadingType::LOAD_ONLY_ORIGINAL_TEXTURE;
}


} // bv

