#include "TextureAssetDescriptor.h"
#include "LibImage.h"
#include "Engine/Types/EnumsUtils.h"
#include <cassert>

namespace bv
{

const std::string TextureAssetDesc::uid = "TEXTURE_ASSET_DESC";

// ***********************
//
void                TextureAssetDesc::Serialize       ( SerializeObject & sob ) const
{
    sob.SetName( "asset" );
    sob.SetValue( "path", m_originalTextureDesc->GetImagePath() );
        
    sob.Pop();
}

// ***********************
//
ISerializablePtr     TextureAssetDesc::Create          ( DeserializeObject & dob )
{
    auto a = Create( dob.GetValue( "path" ), false ).get();
    auto as = (TextureAssetDesc*)a;
    auto asset = std::shared_ptr< TextureAssetDesc >( as );

    return asset;
}

// ***********************
//
const std::string &	TextureAssetDesc::GetUID() const
{
	return TextureAssetDesc::UID();
}

// ***********************
//
const std::string & TextureAssetDesc::UID()
{
	return TextureAssetDesc::uid;
}

// ***********************
//
bool TextureAssetDesc::IsCacheable() const
{
	return true;
}

// ***********************
//
VoidConstPtr TextureAssetDesc::QueryThis() const
{
	return shared_from_this();
}

// ***********************
//
TextureAssetDescConstPtr	TextureAssetDesc::Create( const std::string & imageFilePath, bool isCacheable )
{
	auto props = image::GetImageProps( imageFilePath );

	if( !props.error.empty() )
	{
		return nullptr;
	}

	return Create( SingleTextureAssetDesc::Create( imageFilePath, props.width, props.height, EnumsUtils::Convert( props.format ), isCacheable ) );
}

// ***********************
//
TextureAssetDescConstPtr	TextureAssetDesc::Create( const std::string & imageFilePath, MipMapFilterType mmFilter, bool isCacheable )
{
	auto props = image::GetImageProps( imageFilePath );

	if( !props.error.empty() )
	{
		return nullptr;
	}

	return Create( SingleTextureAssetDesc::Create( imageFilePath, props.width, props.height, EnumsUtils::Convert( props.format ), isCacheable ), mmFilter );
}

// ***********************
//
TextureAssetDescConstPtr	TextureAssetDesc::Create( const std::string & imageFilePath, const StringVector & mipMapsPaths, bool isCacheable )
{
	auto props = image::GetImageProps( imageFilePath );

	if( !props.error.empty() )
	{
		return nullptr;
	}

	std::vector< SingleTextureAssetDescConstPtr > mmDescs;

	for( auto mmFilePath : mipMapsPaths )
	{
		auto mmProps = image::GetImageProps( mmFilePath );

		if( !mmProps.error.empty() )
		{
			return nullptr;
		}

		mmDescs.push_back( SingleTextureAssetDesc::Create( mmFilePath, mmProps.width, mmProps.height, EnumsUtils::Convert( mmProps.format ), isCacheable ) );
	}

	auto mmResDesc = MipMapAssetDesc::Create( mmDescs );

	return Create( SingleTextureAssetDesc::Create( imageFilePath, props.width, props.height, EnumsUtils::Convert( props.format ), isCacheable ), mmResDesc );
}

// ***********************
//
TextureAssetDescConstPtr	TextureAssetDesc::Create( const SingleTextureAssetDescConstPtr & origDesc, const MipMapAssetDescConstPtr & mipmapsDesc )
{
	return std::make_shared< TextureAssetDesc >( origDesc, mipmapsDesc );
}

// ***********************
//
TextureAssetDescConstPtr	TextureAssetDesc::Create( const SingleTextureAssetDescConstPtr & origDesc, MipMapFilterType mmFilter )
{
	return std::make_shared< TextureAssetDesc >( origDesc, mmFilter );
}

// ***********************
//
TextureAssetDescConstPtr	TextureAssetDesc::Create( const SingleTextureAssetDescConstPtr & origDesc )
{
	return std::make_shared< TextureAssetDesc >( origDesc );
}

// ***********************
//
TextureAssetDesc::TextureAssetDesc( const SingleTextureAssetDescConstPtr & origDesc, const MipMapAssetDescConstPtr & mipmapsDesc )
	: m_originalTextureDesc( origDesc )
	, m_mipMapsDescs( mipmapsDesc )
{
	 if( !mipmapsDesc )
	 {
		 m_loadingType = TextureAssetLoadingType::LOAD_ONLY_ORIGINAL_TEXTURE;
	 }
	 else
	 {
		 m_loadingType = TextureAssetLoadingType::LOAD_ORIGINAL_TEXTURE_AND_MIP_MAPS;
	 }
}

// ***********************
//
TextureAssetDesc::TextureAssetDesc( const SingleTextureAssetDescConstPtr & origDesc, MipMapFilterType mmFilter )
	: m_originalTextureDesc( origDesc )
	, m_mipMapsDescs()
{
	m_loadingType = TextureAssetLoadingType::LOAD_ORIGINAL_TEXTURE_AND_GENERATE_MIP_MAPS;
	m_mipMapsDescs = MipMapAssetDesc::Create( mmFilter, origDesc );
}

// ***********************
//
TextureAssetDesc::TextureAssetDesc( const SingleTextureAssetDescConstPtr & origDesc )
	: m_originalTextureDesc( origDesc )
	, m_mipMapsDescs()
{
	m_loadingType = TextureAssetLoadingType::LOAD_ONLY_ORIGINAL_TEXTURE;
}

// ***********************
//
TextureAssetLoadingType TextureAssetDesc::GetLoadingType() const
{
	return m_loadingType;
}

// ***********************
//
SingleTextureAssetDescConstPtr TextureAssetDesc::GetOrigTextureDesc() const
{
	return m_originalTextureDesc;
}

// ***********************
//
MipMapAssetDescConstPtr TextureAssetDesc::GetMipMapsDesc() const
{
	return m_mipMapsDescs;
}

//// ***********************
////
//SizeType TextureAssetDesc::GetMipMapsDescNum	() const
//{
//	return m_mipMapsDescs.size();
//}
//
//// ***********************
////
//SingleTextureAssetDescConstPtr TextureAssetDesc::GetMipMapDesc( SizeType i ) const
//{
//	if( i < m_mipMapsDescs.size() )
//		return m_mipMapsDescs[ i ];
//	else
//		return nullptr;
//}


} // bv

