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

UInt32						TextureLoader::ToBPP							( TextureFormat format )
{
	switch( format )
	{
	case TextureFormat::F_A32FR32FG32FB32F:
		return 128;
	case TextureFormat::F_R32FG32FB32F:
		return 96;
	case TextureFormat::F_A32F:
		return 32;
	case TextureFormat::F_A8R8G8B8:
		return 32;
	case TextureFormat::F_R8G8B8:
		return 24;
	case TextureFormat::F_A8:
		return 8;
	case TextureFormat::F_L8:
		return 8;
	default:
		assert( !"Should never be here !" );
		return 0;
	}
}

// ******************************
//
SingleTextureAssetConstPtr	TextureLoader::LoadSingleTexture( const SingleTextureAssetDescConstPtr & singleTextureResDesc, bool loadFromCache )
{
	auto key		= singleTextureResDesc->GetKey();

	MemoryChunkConstPtr mmChunk = loadFromCache ? RawDataCache::GetInstance().Get( Hash::FromString( key ) ) : nullptr;
	
	if( mmChunk ) // if found in the cache
	{
		auto format		= NearestSupportedTextureFormat( singleTextureResDesc->GetFormat() );
		return SingleTextureAsset::Create( mmChunk, key, singleTextureResDesc->GetWidth(), singleTextureResDesc->GetHeight(), format );
	}
	else
	{
		UInt32 w			= 0;
		UInt32 h			= 0;
		UInt32 bpp			= 0;
		UInt32 channelNum	= 0;

		auto imgPath		= singleTextureResDesc->GetImagePath();

		mmChunk = LoadImage( imgPath, &w, &h, &bpp, &channelNum );
		if( loadFromCache )
		{
			auto res = RawDataCache::GetInstance().Add( Hash::FromString( key ), mmChunk );
			assert( res );
			{ res; }
		}

		if( !mmChunk )
		{
			return nullptr;
		}

		auto format		= ToTextureFormat( bpp, channelNum );

		return SingleTextureAsset::Create( mmChunk, key, singleTextureResDesc->GetWidth(), singleTextureResDesc->GetHeight(), format );
	}
}

// ******************************
//
MemoryChunkConstPtr TextureLoader::LoadImage( const std::string & path, UInt32 * width, UInt32 * height, UInt32 * bpp, UInt32 * channelNum )
{
	if( path.find( ".raw" ) != std::string::npos )
    {
		return image::LoadRAWImage( path );
    }
	else
    {
		return image::LoadImage( path, width, height, bpp, channelNum );
    }
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

	if( desc->IsCacheable() )  // check if already in cache
	{
		auto cachedTextureAsset = TextureCache::GetInstance().Get( desc );

		if( cachedTextureAsset )
		{
			return cachedTextureAsset;
		}
	}

	auto origW = desc->GetOrigTextureDesc()->GetWidth();
	auto origH = desc->GetOrigTextureDesc()->GetHeight();

	SingleTextureAssetConstPtr origRes = LoadSingleTexture( desc->GetOrigTextureDesc(), desc->GetOrigTextureDesc()->IsCacheable() );

	std::vector< SingleTextureAssetConstPtr > mipMapsRes;

	if( desc->IsCacheable() )  // check if already in cache
	{
		auto mmDesc = desc->GetMipMapsDesc();

		if( mmDesc )
		{
			mipMapsRes.resize( mmDesc->GetLevelsNum() );

			auto cachedMMAsset = LoadSingleTexture( mmDesc->GetLevelDesc( 0 ) );

			if( cachedMMAsset )
			{
				mipMapsRes[ 0 ] = cachedMMAsset;
			}
			else
			{
				auto data	= origRes->GetData();
				auto w		= origW;
				auto h		= origH;
				auto bpp	= TextureLoader::ToBPP( origRes->GetFormat() );

				tools::Image img = { data, w, h, bpp };
				auto mm = tools::GenerateMipmaps(	img,
													1,
													ToMMBuilderFilterType( mmDesc->GetFilter() ) );

				auto key = mmDesc->GetLevelDesc( 0 )->GetKey();

				mipMapsRes[ 0 ] = SingleTextureAsset::Create(	mm[ 0 ].data,
																key,
																mm[ 0 ].width,
																mm[ 0 ].height,
																origRes->GetFormat(),
																true );
			}


			for(SizeType i = 1; i < mmDesc->GetLevelsNum(); ++i )
			{
				auto cachedMMAsset = LoadSingleTexture( mmDesc->GetLevelDesc( i ) );
				if( cachedMMAsset )
				{
					mipMapsRes[ i ] = cachedMMAsset;
				}
				else
				{
					auto data	= mipMapsRes[ i - 1 ]->GetData();
					auto w		= mipMapsRes[ i - 1 ]->GetWidth();
					auto h		= mipMapsRes[ i - 1 ]->GetHeight();
					auto bpp	= TextureLoader::ToBPP( mipMapsRes[ i - 1 ]->GetFormat() );

					tools::Image img = { data, w, h, bpp };
					auto mm = tools::GenerateMipmaps(	img,
														2,
														ToMMBuilderFilterType( desc->GetMipMapsDesc()->GetFilter() ) );

					auto key = mmDesc->GetLevelDesc( i )->GetKey();

					mipMapsRes[ i ] = SingleTextureAsset::Create(	mm[ 1 ].data,
																	key, 
																	mm[ 1 ].width, 
																	mm[ 1 ].height, 
																	origRes->GetFormat(), 
																	true );
				}
			}
		}
	}

	auto textureAsset = TextureAsset::Create( origRes, MipMapAsset::Create( mipMapsRes ) );

	if( desc->IsCacheable() )  // Cache the texture asset
	{
		TextureCache::GetInstance().Add( desc, textureAsset );
	}

	return textureAsset;
}

} // bv
