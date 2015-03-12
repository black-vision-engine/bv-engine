#include "TextureLoader.h"
#include "TextureHelpers.h"

#include "Assets/Texture/TextureAssetDescriptor.h"
#include "Assets/Texture/SingleTextureAsset.h"
#include "Assets/Texture/TextureAsset.h"
#include "Assets/Texture/TextureCache.h"
#include "Tools/MipMapBuilder/Source/MipMapBuilder.h"
#include "Assets/Cache/RawDataCache.h"

#include <cassert>

namespace bv { namespace model {

// ******************************
//
namespace
{

tools::FilterType ToMMBuilderFilterType( MipMapFilterType ft )
{
	switch( ft )
	{
	case MipMapFilterType::BOX:
		return tools::FilterType::BOX;
	case MipMapFilterType::BILINEAR:
		return tools::FilterType::BILINEAR;
	case MipMapFilterType::B_SPLINE:
		return tools::FilterType::B_SPLINE;
	case MipMapFilterType::BICUBIC:
		return tools::FilterType::BICUBIC;
	case MipMapFilterType::CATMULL_ROM:
		return tools::FilterType::CATMULL_ROM;
	case MipMapFilterType::LANCZOS:
		return tools::FilterType::LANCZOS;
	default:
		assert( !"Impossible enum value" );
		return tools::FilterType::BOX;
	}
}

} // anounymous

// ******************************
//
ResourceConstPtr TextureLoader::LoadResource( const ResourceDescConstPtr & desc ) const
{
	auto typedDesc = QueryTypedDesc< TextureResourceDescConstPtr >( desc );

	assert( typedDesc );

	TextureResourceConstPtr ret = nullptr;

	switch( typedDesc->GetLoadingType() )
	{
		case TextureResourceLoadingType::LOAD_ONLY_ORIGINAL_TEXTURE:
		{
			auto origRes = LoadSingleTexture( typedDesc->GetOrigTextureDesc(), typedDesc->GetOrigTextureDesc()->IsCacheable() );
	
			ret = TextureResource::Create( origRes, nullptr );

			break;
		}

		case TextureResourceLoadingType::LOAD_ORIGINAL_TEXTURE_AND_MIP_MAPS:
		{
			auto origRes = LoadSingleTexture( typedDesc->GetOrigTextureDesc(), typedDesc->GetOrigTextureDesc()->IsCacheable() );

			auto mipMapsSize = typedDesc->GetMipMapsDesc()->GetLevelsNum();

			std::vector< SingleTextureResourceConstPtr > mipMapsRes;

			for( SizeType i = 0; i < mipMapsSize; ++i )
			{
				auto levelDesc = typedDesc->GetMipMapsDesc()->GetLevelDesc( i );
				mipMapsRes.push_back( LoadSingleTexture( levelDesc, levelDesc->IsCacheable() ) );
			}

			auto mipMapRes = MipMapResource::Create( mipMapsRes );

			ret = TextureResource::Create( origRes, mipMapRes );
			break;
		}

		case TextureResourceLoadingType::LOAD_ORIGINAL_TEXTURE_AND_GENERATE_MIP_MAPS:
		{
			auto origW = typedDesc->GetOrigTextureDesc()->GetWidth();
			auto origH = typedDesc->GetOrigTextureDesc()->GetHeight();

			auto mm = tools::GenerateMipmaps(	typedDesc->GetOrigTextureDesc()->GetImagePath(),
												(int)typedDesc->GetMipMapsDesc()->GetLevelsNum(),
												ToMMBuilderFilterType( typedDesc->GetMipMapsDesc()->GetFilter() ) );


			std::vector< SingleTextureResourceConstPtr > mipMapsRes;

			SingleTextureResourceConstPtr origRes = LoadSingleTexture( typedDesc->GetOrigTextureDesc(), typedDesc->GetOrigTextureDesc()->IsCacheable() );

			if( mm[ 0 ].width == origW && mm[ 0 ].height == origH )
				mipMapsRes.push_back( origRes );

			for( SizeType i = 1; i < mm.size(); ++i )
			{
				UInt32 w = mm[ i ].width;
				UInt32 h = mm[ i ].height;
				UInt32 s = w * h * 4; // FIXME: Supporting only 32-bit image
				auto key = TextureCache::GenKeyForGeneratedMipMap( typedDesc->GetOrigTextureDesc()->GetImagePath(), w, h, TextureFormat::F_A8R8G8B8, i, typedDesc->GetMipMapsDesc()->GetFilter() );
				mipMapsRes.push_back( SingleTextureResource::Create( MemoryChunk::Create( mm[ i ].data, s ), key, w, h, TextureFormat::F_A8R8G8B8 ) );
			}

			ret = TextureResource::Create( origRes, MipMapResource::Create( mipMapsRes ) );
			break;
		}
	}


	return ret;
}

// ******************************
//
SingleTextureResourceConstPtr TextureLoader::LoadSingleTexture( const SingleTextureResourceDescConstPtr & sinlgeTextureResDesc, bool loadFromCache )
{
	auto key		= TextureCache::GenKeyForSingleTexture( sinlgeTextureResDesc );
	auto imgPath	= sinlgeTextureResDesc->GetImagePath();

	MemoryChunkConstPtr mmChunk = loadFromCache ? RawDataCache::GetInstance().Get( Hash::FromString( key ) ) : nullptr;
	
	if( !mmChunk )
	{
		mmChunk = LoadImage( imgPath );
		if( loadFromCache )
		{
			assert( RawDataCache::GetInstance().Add( Hash::FromString( key ), mmChunk ) );
		}
	}

	if( !mmChunk )
	{
		return nullptr;
	}

	auto w			= sinlgeTextureResDesc->GetWidth();
	auto h			= sinlgeTextureResDesc->GetHeight();
	auto format		= TextureFormat::F_A8R8G8B8;//sinlgeTextureResDesc->GetFormat();

	return SingleTextureResource::Create( mmChunk, key, w, h, format );
}

// ******************************
//
MemoryChunkConstPtr TextureLoader::LoadImage( const std::string & path )
{
	MemoryChunkConstPtr data = nullptr;

	unsigned int width   = 0;
    unsigned int height  = 0;
    unsigned int bpp     = 0;

	if( path.find( ".raw" ) != std::string::npos )
    {
		data = TextureHelper::LoadRAW( path );
    }
	else
    {
		data = TextureHelper::LoadImg( path, &width, &height, &bpp );
    }

	return data;
}

} // model
} // bv