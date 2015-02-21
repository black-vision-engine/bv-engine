#include "TextureResourceDescriptor.h"
#include "LibImage.h"
#include <cassert>

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
TextureResourceDescConstPtr	TextureResourceDesc::Create( const std::string & imageFilePath )
{
	auto props = image::GetImageProps( imageFilePath );

	if( !props.error.empty() )
	{
		return nullptr;
	}

	return Create( SingleTextureResourceDesc::Create( imageFilePath, props.width, props.height, props.format ) );
}

// ***********************
//
TextureResourceDescConstPtr	TextureResourceDesc::Create( const std::string & imageFilePath, MipMapFilterType mmFilter )
{
	auto props = image::GetImageProps( imageFilePath );

	if( !props.error.empty() )
	{
		return nullptr;
	}

	return Create( SingleTextureResourceDesc::Create( imageFilePath, props.width, props.height, props.format ), mmFilter );
}

// ***********************
//
TextureResourceDescConstPtr	TextureResourceDesc::Create( const std::string & imageFilePath, const StringVector & mipMapsPaths )
{
	auto props = image::GetImageProps( imageFilePath );

	if( !props.error.empty() )
	{
		return nullptr;
	}

	std::vector< SingleTextureResourceDescConstPtr > mmDescs;

	for( auto mmFilePath : mipMapsPaths )
	{
		auto mmProps = image::GetImageProps( mmFilePath );

		if( !mmProps.error.empty() )
		{
			return nullptr;
		}

		mmDescs.push_back( SingleTextureResourceDesc::Create( mmFilePath, mmProps.width, mmProps.height, mmProps.format ) );
	}

	auto mmResDesc = MipMapResourceDesc::Create( mmDescs );

	return Create( SingleTextureResourceDesc::Create( imageFilePath, props.width, props.height, props.format ), mmResDesc );
}

// ***********************
//
TextureResourceDescConstPtr	TextureResourceDesc::Create( const SingleTextureResourceDescConstPtr & origDesc, const MipMapResourceDescConstPtr & mipmapsDesc )
{
	return std::make_shared< TextureResourceDesc >( origDesc, mipmapsDesc );
}

// ***********************
//
TextureResourceDescConstPtr	TextureResourceDesc::Create( const SingleTextureResourceDescConstPtr & origDesc, MipMapFilterType mmFilter )
{
	return std::make_shared< TextureResourceDesc >( origDesc, mmFilter );
}

// ***********************
//
TextureResourceDescConstPtr	TextureResourceDesc::Create( const SingleTextureResourceDescConstPtr & origDesc )
{
	return std::make_shared< TextureResourceDesc >( origDesc );
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
TextureResourceDesc::TextureResourceDesc( const SingleTextureResourceDescConstPtr & origDesc, MipMapFilterType mmFilter )
	: m_originalTextureDesc( origDesc )
	, m_mipMapsDescs()
{
	m_loadingType = TextureResourceLoadingType::LOAD_ORIGINAL_TEXTURE_AND_GENERATE_MIP_MAPS;
	m_mipMapsDescs = MipMapResourceDesc::Create( mmFilter, origDesc );
}

// ***********************
//
TextureResourceDesc::TextureResourceDesc( const SingleTextureResourceDescConstPtr & origDesc )
	: m_originalTextureDesc( origDesc )
	, m_mipMapsDescs()
{
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

