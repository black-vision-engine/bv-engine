#include "TextureResourceDescriptor.h"

namespace bv
{

const std::string TextureResourceDesc::uid = "TEXTURE_RESOURCE_DESC";

// ***********************
//
const std::string &	TextureResourceDesc::GetUID() const
{
	return TextureResourceDesc::UID();
}

// ***********************
//
const std::string & TextureResourceDesc::UID()
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
VoidConstPtr TextureResourceDesc::QueryThis() const
{
	return shared_from_this();
}

// ***********************
//
TextureResourceDescConstPtr	Create( const SingleTextureResourceDescConstPtr & origDesc, const MipMapResourceDescConstPtr & mipmapsDesc )
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
TextureResourceDesc::TextureResourceDesc( const SingleTextureResourceDescConstPtr & origDesc, const MipMapResourceDescConstPtr & mipmapsDesc )
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

// ***********************
//
TextureResourceLoadingType TextureResourceDesc::GetLoadingType() const
{
	return m_loadingType;
}

// ***********************
//
SingleTextureResourceDescConstPtr TextureResourceDesc::GetOrigTextureDesc() const
{
	return m_originalTextureDesc;
}

// ***********************
//
MipMapResourceDescConstPtr TextureResourceDesc::GetMipMapsDesc() const
{
	return m_mipMapsDescs;
}

//// ***********************
////
//SizeType TextureResourceDesc::GetMipMapsDescNum	() const
//{
//	return m_mipMapsDescs.size();
//}
//
//// ***********************
////
//SingleTextureResourceDescConstPtr TextureResourceDesc::GetMipMapDesc( SizeType i ) const
//{
//	if( i < m_mipMapsDescs.size() )
//		return m_mipMapsDescs[ i ];
//	else
//		return nullptr;
//}


} // bv
