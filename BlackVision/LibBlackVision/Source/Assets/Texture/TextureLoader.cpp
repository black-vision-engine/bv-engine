#include "TextureLoader.h"
#include "LibImage.h"

#include "Assets/Texture/TextureCache.h"
#include "MipMapBuilder.h"
#include "Assets/Cache/RawDataCache.h"

#include <cassert>

namespace bv {

// ******************************
//
namespace
{

image::FilterType ToMMBuilderFilterType( MipMapFilterType ft )
{
	switch( ft )
	{
	case MipMapFilterType::BOX:
		return image::FilterType::FT_BOX;
	case MipMapFilterType::BILINEAR:
		return image::FilterType::FT_BILINEAR;
	case MipMapFilterType::B_SPLINE:
		return image::FilterType::FT_B_SPLINE;
	case MipMapFilterType::BICUBIC:
		return image::FilterType::FT_BICUBIC;
	case MipMapFilterType::CATMULL_ROM:
		return image::FilterType::FT_CATMULL_ROM;
	case MipMapFilterType::LANCZOS:
		return image::FilterType::FT_LANCZOS;
	default:
		assert( !"Impossible enum value" );
		return image::FilterType::FT_BOX;
	}
}

} // anounymous

// ******************************
//
AssetConstPtr TextureLoader::LoadAsset( const AssetDescConstPtr & desc ) const
{
	auto typedDesc = QueryTypedDesc< TextureAssetDescConstPtr >( desc );

	assert( typedDesc );

	switch( typedDesc->GetLoadingType() )
	{
		case TextureAssetLoadingType::LOAD_ONLY_ORIGINAL_TEXTURE:
		{
			return LoadOrginalTextureOnly( typedDesc );
		}

		case TextureAssetLoadingType::LOAD_ORIGINAL_TEXTURE_AND_MIP_MAPS:
		{
			return LoadTextureAndMipMaps( typedDesc );
		}

		case TextureAssetLoadingType::LOAD_ORIGINAL_TEXTURE_AND_GENERATE_MIP_MAPS:
		{
			return LoadTextureAndGenerateMipMaps( typedDesc );
		}

		default:
		{
			assert( !"Should never be here" );
			return nullptr;
		}
	}
}

// ******************************
//
TextureFormat				TextureLoader::ToTextureFormat( UInt32 bpp, UInt32 channelNum )
{
	switch( bpp )
	{
	case 128:
		if( channelNum == 4 )
			return TextureFormat::F_A32FR32FG32FB32F;
	case 32:
		if( channelNum == 4 )
			return TextureFormat::F_A8R8G8B8;
		else if( channelNum == 1 )
			return TextureFormat::F_A32F;

	case 8:
		if( channelNum == 1 )
			return TextureFormat::F_A8;
	}

	assert( !"Should never be here" );
	return TextureFormat::F_TOTAL;
}

// ******************************
//
TextureFormat				TextureLoader::NearestSupportedTextureFormat	( TextureFormat format )
{
	switch( format )
	{
	case TextureFormat::F_R32FG32FB32F:
		return TextureFormat::F_A32FR32FG32FB32F;
	case TextureFormat::F_R8G8B8:
		return TextureFormat::F_A8R8G8B8;
	default:
		return format;
	}
}

// ******************************
//
SingleTextureAssetConstPtr	TextureLoader::LoadSingleTexture( const SingleTextureAssetDescConstPtr & sinlgeTextureResDesc, bool loadFromCache )
{
	auto key		= TextureCache::GenKeyForSingleTexture( sinlgeTextureResDesc );
	auto imgPath	= sinlgeTextureResDesc->GetImagePath();

	MemoryChunkConstPtr mmChunk = loadFromCache ? RawDataCache::GetInstance().Get( Hash::FromString( key ) ) : nullptr;
	
	if( mmChunk ) // if found in the cache
	{
		auto format		= NearestSupportedTextureFormat( sinlgeTextureResDesc->GetFormat() );
		return SingleTextureAsset::Create( mmChunk, key, sinlgeTextureResDesc->GetWidth(), sinlgeTextureResDesc->GetHeight(), format );
	}
	else
	{
		UInt32 w			= 0;
		UInt32 h			= 0;
		UInt32 bpp			= 0;
		UInt32 channelNum	= 0;

		mmChunk = LoadImage( imgPath, &w, &h, &bpp, &channelNum );
		if( loadFromCache )
		{
			auto res = RawDataCache::GetInstance().Add( Hash::FromString( key ), mmChunk );
			{res;}
			assert( res );
			{ res; }
		}

		if( !mmChunk )
		{
			return nullptr;
		}

		auto format		= ToTextureFormat( bpp, channelNum );

		return SingleTextureAsset::Create( mmChunk, key, sinlgeTextureResDesc->GetWidth(), sinlgeTextureResDesc->GetHeight(), format );
	}
}

// ******************************
//
MemoryChunkConstPtr TextureLoader::LoadImage( const std::string & path, UInt32 * width, UInt32 * height, UInt32 * bpp, UInt32 * channelNum )
{
	MemoryChunkConstPtr data = nullptr;

	if( path.find( ".raw" ) != std::string::npos )
    {
		data = image::LoadRAWImage( path );
    }
	else
    {
		data = image::LoadImage( path, width, height, bpp, channelNum );
    }

	return data;
}

// ******************************
//
TextureAssetConstPtr TextureLoader::LoadOrginalTextureOnly			( const TextureAssetDescConstPtr & desc )
{
	assert( desc->GetLoadingType() == TextureAssetLoadingType::LOAD_ONLY_ORIGINAL_TEXTURE );

	auto origRes = LoadSingleTexture( desc->GetOrigTextureDesc(), desc->GetOrigTextureDesc()->IsCacheable() );
	
	return TextureAsset::Create( origRes, nullptr );
}

// ******************************
//
TextureAssetConstPtr TextureLoader::LoadTextureAndMipMaps			( const TextureAssetDescConstPtr & desc )
{
	assert( desc->GetLoadingType() == TextureAssetLoadingType::LOAD_ORIGINAL_TEXTURE_AND_MIP_MAPS );

	auto origRes = LoadSingleTexture( desc->GetOrigTextureDesc(), desc->GetOrigTextureDesc()->IsCacheable() );

	auto mipMapsSize = desc->GetMipMapsDesc()->GetLevelsNum();

	std::vector< SingleTextureAssetConstPtr > mipMapsRes;

	for( SizeType i = 0; i < mipMapsSize; ++i )
	{
		auto levelDesc = desc->GetMipMapsDesc()->GetLevelDesc( i );
		mipMapsRes.push_back( LoadSingleTexture( levelDesc, levelDesc->IsCacheable() ) );
	}

	auto mipMapRes = MipMapAsset::Create( mipMapsRes );

	return TextureAsset::Create( origRes, mipMapRes );


}

// ******************************
//
TextureAssetConstPtr TextureLoader::LoadTextureAndGenerateMipMaps	( const TextureAssetDescConstPtr & desc )
{
	assert( desc->GetLoadingType() == TextureAssetLoadingType::LOAD_ORIGINAL_TEXTURE_AND_GENERATE_MIP_MAPS );

	auto origW = desc->GetOrigTextureDesc()->GetWidth();
	auto origH = desc->GetOrigTextureDesc()->GetHeight();

	auto mm = tools::GenerateMipmaps(	desc->GetOrigTextureDesc()->GetImagePath(),
										(int)desc->GetMipMapsDesc()->GetLevelsNum(),
										ToMMBuilderFilterType( desc->GetMipMapsDesc()->GetFilter() ) );


	std::vector< SingleTextureAssetConstPtr > mipMapsRes;

	SingleTextureAssetConstPtr origRes = LoadSingleTexture( desc->GetOrigTextureDesc(), desc->GetOrigTextureDesc()->IsCacheable() );

	SizeType i = 0;
	if( mm[ 0 ].width == origW && mm[ 0 ].height == origH )
	{
		mipMapsRes.push_back( origRes );
		i = 1;
	}

	for(; i < mm.size(); ++i )
	{
		auto w = mm[ i ].width;
		auto h = mm[ i ].height;
		auto key = TextureCache::GenKeyForGeneratedMipMap( desc->GetOrigTextureDesc()->GetImagePath(), w, h, TextureFormat::F_A8R8G8B8, i, desc->GetMipMapsDesc()->GetFilter() );
		mipMapsRes.push_back( SingleTextureAsset::Create( mm[ i ].data, key, w, h, TextureFormat::F_A8R8G8B8 ) );
	}

	return TextureAsset::Create( origRes, MipMapAsset::Create( mipMapsRes ) );
}

} // bv
