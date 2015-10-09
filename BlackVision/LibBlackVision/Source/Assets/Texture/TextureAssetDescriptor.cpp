#include "TextureAssetDescriptor.h"
#include "LibImage.h"
#include "Engine/Types/EnumsUtils.h"
#include "Tools/Utils.h"
#include <cassert>

namespace bv
{

const std::string TextureAssetDesc::uid = "TEXTURE_ASSET_DESC";

// ***********************
//
template<class Serializer>
void TextureAssetDesc::SerializeAsset( Serializer& sob ) const
{
sob.SetName( "asset" );
    sob.SetValue( "type", "tx" );
    sob.SetValue( "path", m_originalTextureDesc->GetImagePath() );

    if( m_mipMapsDescs )
        sob.SetValue( "mipmap", Filter2String( m_mipMapsDescs->GetFilter() ) );
    else
        sob.SetValue( "mipmap", "none" );
sob.Pop();
}

// ***********************
//
template<class Deserializer>
TextureAssetDescConstPtr TextureAssetDesc::DeserializeAsset( Deserializer& dob )
{
    auto path = dob.GetValue( "path" );

    auto filterS = dob.GetValue( "mipmap" );
    if( filterS == "none" )
        return Create( path, true );
    else
        return Create( path, String2Filter( filterS ), true );
}

// ***********************
//
std::string Filter2String( MipMapFilterType filter )
{
    if( filter == MipMapFilterType::BILINEAR )
        return "bilinear";
    else
    {
        assert( false );
        return std::to_string( (int) filter );
    }
}

// ***********************
//
void                TextureAssetDesc::Serialize       ( SerializeObject & sob ) const
{
	SerializeAsset( sob );
}

MipMapFilterType String2Filter( std::string string ) // FIXME for God's sake
{
//std::pair< MipMapFilterType, std::string > p[] =
//    { std::make_pair( MipMapFilterType::BICUBIC, "bicubic" ),
//    std::make_pair( MipMapFilterType::BILINEAR, "bilinear" ) };

    if( string == "bilinear" )
        return MipMapFilterType::BILINEAR;
    //else if( string == "none" )
    //    return MipMapFilterType::MMFT_TOTAL;
    else
    {
        assert( false );
        return (MipMapFilterType) std::stoi( string );
    }
}

// ***********************
//
ISerializableConstPtr TextureAssetDesc::Create          ( DeserializeObject & dob )
{
	return DeserializeAsset( dob );
}

// ***********************
//
void TextureAssetDesc::Serialize       ( JsonSerializeObject & sob ) const
{
	SerializeAsset( sob );
}
// ***********************
//
ISerializableConstPtr TextureAssetDesc::Create          ( JsonDeserializeObject & dob )
{
	/*dob.Push( "asset" );*/
	return DeserializeAsset( dob );
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
	return TextureAssetDescConstPtr( new TextureAssetDesc( origDesc, mipmapsDesc ) );
}

// ***********************
//
TextureAssetDescConstPtr	TextureAssetDesc::Create( const SingleTextureAssetDescConstPtr & origDesc, MipMapFilterType mmFilter )
{
	return TextureAssetDescConstPtr( new TextureAssetDesc( origDesc, mmFilter ) );
}

// ***********************
//
TextureAssetDescConstPtr	TextureAssetDesc::Create( const SingleTextureAssetDescConstPtr & origDesc )
{
	return TextureAssetDescConstPtr( new TextureAssetDesc( origDesc ) );
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
std::string				TextureAssetDesc::GetKey		() const
{
	switch( GetLoadingType() )
	{
		case TextureAssetLoadingType::LOAD_ONLY_ORIGINAL_TEXTURE:
			return this->GetOrigTextureDesc()->GetKey();
		case TextureAssetLoadingType::LOAD_ORIGINAL_TEXTURE_AND_GENERATE_MIP_MAPS:
			return this->GetOrigTextureDesc()->GetKey() + toString( this->GetMipMapsDesc()->GetLevelsNum() ) + toString( (int)this->GetMipMapsDesc()->GetFilter() );
		case TextureAssetLoadingType::LOAD_ORIGINAL_TEXTURE_AND_MIP_MAPS:
		{
			auto ret = this->GetOrigTextureDesc()->GetKey();

			for( SizeType i = 0; i < this->GetMipMapsDesc()->GetLevelsNum(); ++i )
				ret += this->GetMipMapsDesc()->GetLevelDesc( i )->GetKey();

			ret += toString( this->GetMipMapsDesc()->GetLevelsNum() ) + toString( (int)this->GetMipMapsDesc()->GetFilter() );

			return ret;
		}
		default:
			assert( !"Imposible enum value" );
			return "";
	}
}

// ***********************
//
std::string             TextureAssetDesc::GetProposedShortKey () const
{
    auto basename = AssetDesc::GetProposedShortKey();
    return basename.substr( 0, basename.find( '.' ) );
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

