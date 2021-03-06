#include "stdafx.h"

#include "TextureAssetDescriptor.h"
#include "LibImage.h"
#include "Engine/Types/EnumsUtils.h"
#include "Tools/Utils.h"
#include "TextureUtils.h"
#include "ProjectManager.h"
#include <cassert>

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"
#include "Serialization/BV/BVSerializeContext.h"



#include "Memory/MemoryLeaks.h"



namespace bv
{

const std::string TextureAssetDesc::uid = "TEXTURE_ASSET_DESC";


namespace Convert
{

// ***********************
//
	std::pair< MipMapFilterType, const char* > mipmapFiltering[] =
	{ std::make_pair( MipMapFilterType::BICUBIC, "bicubic" )
	, std::make_pair( MipMapFilterType::BILINEAR, "bilinear" )
	, std::make_pair( MipMapFilterType::BOX, "box" )
	, std::make_pair( MipMapFilterType::B_SPLINE, "b spline" )
	, std::make_pair( MipMapFilterType::CATMULL_ROM, "catmull rom" )
	, std::make_pair( MipMapFilterType::LANCZOS, "lanczos" )
	, std::make_pair( MipMapFilterType::MMFT_TOTAL, "none" )
	, std::make_pair( MipMapFilterType::MMFT_TOTAL, "" )
};

IMPLEMENT_ENUM_SERIALIZATION( MipMapFilterType, mipmapFiltering )


} // Convert


// ***********************
//
void                TextureAssetDesc::Serialize       ( ISerializer& ser ) const
{
    auto context = static_cast<BVSerializeContext*>( ser.GetSerializeContext() );

ser.EnterChild( "asset" );
    ser.SetAttribute( "type", UID() );
    ser.SetAttribute( "path", m_originalTextureDesc->GetImagePath() );

    if( context->extendedAssetData )
    {
        ser.SetAttribute( "width", Convert::T2String( m_originalTextureDesc->GetWidth() ) );
        ser.SetAttribute( "height", Convert::T2String( m_originalTextureDesc->GetHeight() ) );
        ser.SetAttribute( "estimatedMemoryUsage", Convert::T2String( EstimateMemoryUsage() ) );
    }

    if( m_mipMapsDescs )
        ser.SetAttribute( "filter", Convert::T2String( m_mipMapsDescs->GetFilter() ) );
    else
        ser.SetAttribute( "filter", "none" );

    if( m_loadingType == TextureAssetLoadingType::LOAD_ONLY_ORIGINAL_TEXTURE )
        ser.SetAttribute( "loading_type", "ONLY_ORIGINAL" );
    else if( m_loadingType == TextureAssetLoadingType::LOAD_ORIGINAL_TEXTURE_AND_GENERATE_MIP_MAPS )
        ser.SetAttribute( "loading_type", "GENERATE_MIPMAPS" );
    else
        ser.SetAttribute( "loading_type", "LOAD_WITH_MIPMAPS" );

    if( m_loadingType == TextureAssetLoadingType::LOAD_ORIGINAL_TEXTURE_AND_MIP_MAPS )
    {
        m_mipMapsDescs->Serialize( ser );
    }

ser.ExitChild();
}

// ***********************
//
void TextureAssetDesc::Deserialize     ( const IDeserializer& sob )
{
    auto obj = SerializationHelper::Create< const TextureAssetDesc >( sob );

    if( obj == nullptr || obj->GetUID() != uid )
    {
        assert( !"Failed" ); //FIXME: Error handling.
        return;
    }

    *this = *obj; // if you think this is ugly, see why it cannot be done:
}


// ***********************
//
ISerializableConstPtr TextureAssetDesc::Create          ( const IDeserializer& deser )
{
    auto path = deser.GetAttribute( "path" );
    auto loadingType = deser.GetAttribute( "loading_type" );

    if( loadingType == "LOAD_WITH_MIPMAPS" )
    {
        if( deser.EnterChild( "mipmaps" ) )
        {
            deser.EnterChild( "asset" );
            do
            {
            } while( deser.NextChild() );
            deser.ExitChild(); // asset

            deser.ExitChild(); // mipmaps
        }
    }

    auto filterS = deser.GetAttribute( "filter" );
    if( loadingType == "GENERATE_MIPMAPS" && filterS != "none" )
    {
        return Create( path, Convert::String2T( filterS, MipMapFilterType::BILINEAR ), true );
    }
    else
    {
        return Create( path, true );
    }
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
    auto props = image::GetImageProps( ProjectManager::GetInstance()->ToAbsPath( imageFilePath ).Str() );

    if( !props.error.empty() )
    {
        // Return descriptor pointing to invalid texture. Extern code will deal with it.
        return Create( SingleTextureAssetDesc::Create( imageFilePath, 0, 0, TextureFormat::F_A8R8G8B8, isCacheable ) );
    }

    return Create( SingleTextureAssetDesc::Create( imageFilePath, props.width, props.height, EnumsUtils::Convert( props.format ), isCacheable ) );
}

// ***********************
//
TextureAssetDescConstPtr	TextureAssetDesc::Create( const std::string & imageFilePath, MipMapFilterType mmFilter, bool isCacheable )
{
    auto props = image::GetImageProps( ProjectManager::GetInstance()->ToAbsPath( imageFilePath ).Str() );

    if( !props.error.empty() )
    {
        // Return descriptor pointing to invalid texture. Extern code will deal with it.
        return Create( SingleTextureAssetDesc::Create( imageFilePath, 0, 0, TextureFormat::F_A8R8G8B8, isCacheable ), mmFilter );
    }

    return Create( SingleTextureAssetDesc::Create( imageFilePath, props.width, props.height, EnumsUtils::Convert( props.format ), isCacheable ), mmFilter );
}

// ***********************
//
TextureAssetDescConstPtr	TextureAssetDesc::Create( const std::string & imageFilePath, const StringVector & mipMapsPaths, bool isCacheable )
{
    auto props = image::GetImageProps( ProjectManager::GetInstance()->ToAbsPath( imageFilePath ).Str() );

    if( !props.error.empty() )
    {
        // Return descriptor pointing to invalid texture. Extern code will deal with it.
        return Create( SingleTextureAssetDesc::Create( imageFilePath, 0, 0, TextureFormat::F_A8R8G8B8, isCacheable ) );
    }

    std::vector< SingleTextureAssetDescConstPtr > mmDescs;

    for( auto mmFilePath : mipMapsPaths )
    {
        auto mmProps = image::GetImageProps( mmFilePath );

        if( !mmProps.error.empty() )
        {
            // Return descriptor pointing to invalid texture. Extern code will deal with it.
            return Create( SingleTextureAssetDesc::Create( imageFilePath, 0, 0, TextureFormat::F_A8R8G8B8, isCacheable ) );
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
    if( m_key.empty() )
        m_key = ComputeKey();

    return m_key;
}


// ***********************
//
std::string         TextureAssetDesc::ComputeKey() const
{
    switch( GetLoadingType() )
    {
    case TextureAssetLoadingType::LOAD_ONLY_ORIGINAL_TEXTURE:
        return this->GetOrigTextureDesc()->GetKey();
    case TextureAssetLoadingType::LOAD_ORIGINAL_TEXTURE_AND_GENERATE_MIP_MAPS:
        return this->GetOrigTextureDesc()->GetKey() + "ML" + toString( this->GetMipMapsDesc()->GetLevelsNum() ) + "MF" + toString( ( int )this->GetMipMapsDesc()->GetFilter() );
    case TextureAssetLoadingType::LOAD_ORIGINAL_TEXTURE_AND_MIP_MAPS:
    {
        auto ret = this->GetOrigTextureDesc()->GetKey();

        for( SizeType i = 0; i < this->GetMipMapsDesc()->GetLevelsNum(); ++i )
            ret += this->GetMipMapsDesc()->GetLevelDesc( i )->GetKey();

        ret += "ML" + toString( this->GetMipMapsDesc()->GetLevelsNum() ) + "MF" + toString( ( int )this->GetMipMapsDesc()->GetFilter() );

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
SizeType                TextureAssetDesc::EstimateMemoryUsage () const
{
    SizeType assetSize = 0;
    UInt32 pixelSize = TextureUtils::ToBPP( m_originalTextureDesc->GetFormat() ) / 8;

    if( m_loadingType == TextureAssetLoadingType::LOAD_ORIGINAL_TEXTURE_AND_GENERATE_MIP_MAPS )
    {
        // Generating mip maps changes size of tezture.
        auto newWidth = RoundUpToPowerOfTwo( m_originalTextureDesc->GetWidth() );
        auto newHeight = RoundUpToPowerOfTwo( m_originalTextureDesc->GetHeight() );

        // Estimate mip maps size as the same as original texture size.
        assetSize += 2 * pixelSize * newWidth * newHeight;
    }
    else
    {
        if( m_originalTextureDesc != nullptr )
            assetSize += m_originalTextureDesc->EstimateMemoryUsage();

        if( m_mipMapsDescs != nullptr )
            assetSize += m_mipMapsDescs->EstimateMemoryUsage();
    }

    return assetSize;
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

