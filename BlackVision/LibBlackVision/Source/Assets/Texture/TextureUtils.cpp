#include "TextureUtils.h"

#include "Assets/Texture/TextureCache.h"
#include "MipMapBuilder.h"
#include "Assets/Cache/RawDataCache.h"

#include <cassert>

namespace bv {

// ******************************
// Anonymous namespace begin
namespace
{

// ******************************
//
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
TextureAssetConstPtr	TextureUtils::GenerateMipMaps ( SingleTextureAssetConstPtr texture, Int32 numLevels, MipMapFilterType ft )
{
	if( numLevels > 0 )
	{
		tools::Image img = { texture->GetData(), texture->GetWidth(), texture->GetHeight(), ToBPP( texture->GetFormat() ) };
		auto mipmap = tools::GenerateMipmaps( img, numLevels, ToMMBuilderFilterType( ft ) );

		std::vector< SingleTextureAssetConstPtr > mipMapsRes;
		for( SizeType i = 0; i < mipmap.size(); ++i )
		{		
			assert( ToBPP( texture->GetFormat() ) == mipmap[ i ].bpp );  // Mipmaps should have same format.
			mipMapsRes.push_back( SingleTextureAsset::Create( mipmap[ i ].data, "", mipmap[ i ].width, mipmap[ i ].height, texture->GetFormat(), true ) );
		}

		auto mipmaps = MipMapAsset::Create( mipMapsRes );

		return TextureAsset::Create( texture, mipmaps );
	}
	else
	{
		return TextureAsset::Create( texture, nullptr );
	}
}

// ******************************
//
TextureFormat			TextureUtils::ToTextureFormat( UInt32 bpp, UInt32 channelNum )
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
TextureFormat			TextureUtils::NearestSupportedTextureFormat	( TextureFormat format )
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
UInt32					TextureUtils::ToBPP							( TextureFormat format )
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
SingleTextureAssetConstPtr	TextureUtils::LoadSingleTexture( const SingleTextureAssetDescConstPtr & singleTextureResDesc, bool cacheOnDisk )
{
	auto key		= singleTextureResDesc->GetKey();

	auto isCacheable = singleTextureResDesc->IsCacheable();

	MemoryChunkConstPtr mmChunk = isCacheable ? RawDataCache::GetInstance().Get( Hash::FromString( key ) ) : nullptr;
	
	if( mmChunk ) // if found in the cache
	{
		auto format		= TextureUtils::NearestSupportedTextureFormat( singleTextureResDesc->GetFormat() );
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
		
		if( !mmChunk )
		{
			return nullptr;
		}
		
		if( isCacheable )
		{
			auto res = RawDataCache::GetInstance().Add( Hash::FromString( key ), mmChunk, cacheOnDisk );
			assert( res );
			{ res; }
		}

		auto format		= TextureUtils::ToTextureFormat( bpp, channelNum );

		return SingleTextureAsset::Create( mmChunk, key, singleTextureResDesc->GetWidth(), singleTextureResDesc->GetHeight(), format, cacheOnDisk );
	}
}

// ******************************
//
MemoryChunkConstPtr TextureUtils::LoadImage( const std::string & path, UInt32 * width, UInt32 * height, UInt32 * bpp, UInt32 * channelNum )
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
TextureAssetConstPtr TextureUtils::LoadOrginalTextureOnly			( const TextureAssetDescConstPtr & desc )
{
	assert( desc->GetLoadingType() == TextureAssetLoadingType::LOAD_ONLY_ORIGINAL_TEXTURE );

	auto origRes = LoadSingleTexture( desc->GetOrigTextureDesc(), false );
	
	return TextureAsset::Create( origRes, nullptr );
}

// ******************************
//
TextureAssetConstPtr TextureUtils::LoadTextureAndMipMaps			( const TextureAssetDescConstPtr & desc )
{
	assert( desc->GetLoadingType() == TextureAssetLoadingType::LOAD_ORIGINAL_TEXTURE_AND_MIP_MAPS );

	auto origRes = LoadSingleTexture( desc->GetOrigTextureDesc(), false );

	auto mipMapsSize = desc->GetMipMapsDesc()->GetLevelsNum();

	std::vector< SingleTextureAssetConstPtr > mipMapsRes;

	for( SizeType i = 0; i < mipMapsSize; ++i )
	{
		auto levelDesc = desc->GetMipMapsDesc()->GetLevelDesc( i );
		mipMapsRes.push_back( LoadSingleTexture( levelDesc, false ) );
	}

	auto mipMapRes = MipMapAsset::Create( mipMapsRes );

	return TextureAsset::Create( origRes, mipMapRes );


}

// ******************************
//
TextureAssetConstPtr TextureUtils::LoadTextureAndGenerateMipMaps	( const TextureAssetDescConstPtr & desc )
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

	SingleTextureAssetConstPtr origRes = LoadSingleTexture( desc->GetOrigTextureDesc(), false );

	std::vector< SingleTextureAssetConstPtr > mipMapsRes;

	if( desc->IsCacheable() )  // check if already in cache
	{
		auto mmDesc = desc->GetMipMapsDesc();

		if( mmDesc )
		{
			mipMapsRes.resize( mmDesc->GetLevelsNum() );

			auto cachedMMAsset = LoadSingleTexture( mmDesc->GetLevelDesc( 0 ), true );

			if( cachedMMAsset )
			{
				mipMapsRes[ 0 ] = cachedMMAsset;
			}
			else
			{
				auto textureWithMipMap =TextureUtils::GenerateMipMaps( origRes, 1, mmDesc->GetFilter() );
				//auto data	= origRes->GetData();
				//auto w		= origW;
				//auto h		= origH;
				//auto bpp	= TextureUtils::ToBPP( origRes->GetFormat() );

				//tools::Image img = { data, w, h, bpp };
				//auto mm = tools::GenerateMipmaps(	img,
				//									1,
				//									ToMMBuilderFilterType( mmDesc->GetFilter() ) );

				auto key = mmDesc->GetLevelDesc( 0 )->GetKey();

				auto mm = textureWithMipMap->GetMipMaps()->GetLevel( 0 );

				mipMapsRes[ 0 ] = SingleTextureAsset::Create(	mm->GetData(),
																key,
																mm->GetWidth(),
																mm->GetHeight(),
																mm->GetFormat(),
																mm->GetCacheOnHardDrive() );
			}


			for(SizeType i = 1; i < mmDesc->GetLevelsNum(); ++i )
			{
				auto cachedMMAsset = LoadSingleTexture( mmDesc->GetLevelDesc( i ), true );
				if( cachedMMAsset )
				{
					mipMapsRes[ i ] = cachedMMAsset;
				}
				else
				{
					auto textureWithMipMap =TextureUtils::GenerateMipMaps( mipMapsRes[ i - 1 ], 2, mmDesc->GetFilter() );
					//auto data	= mipMapsRes[ i - 1 ]->GetData();
					//auto w		= mipMapsRes[ i - 1 ]->GetWidth();
					//auto h		= mipMapsRes[ i - 1 ]->GetHeight();
					//auto bpp	= TextureLoader::ToBPP( mipMapsRes[ i - 1 ]->GetFormat() );

					//tools::Image img = { data, w, h, bpp };
					//auto mm = tools::GenerateMipmaps(	img,
					//									2,
					//									ToMMBuilderFilterType( desc->GetMipMapsDesc()->GetFilter() ) );

					auto key = mmDesc->GetLevelDesc( i )->GetKey();

					auto mm = textureWithMipMap->GetMipMaps()->GetLevel( 1 );

					mipMapsRes[ i ] = SingleTextureAsset::Create(	mm->GetData(),
																	key, 
																	mm->GetWidth(), 
																	mm->GetHeight(),
																	mm->GetFormat(), 
																	mm->GetCacheOnHardDrive() );
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


}  // bv