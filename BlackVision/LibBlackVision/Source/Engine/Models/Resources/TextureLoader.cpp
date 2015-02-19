#include "TextureLoader.h"
#include "TextureHelpers.h"

#include "Engine/Models/Resources/Texture/TextureResourceDescriptor.h"
#include "Engine/Models/Resources/Texture/SingleTextureResource.h"
#include "Engine/Models/Resources/Texture/TextureResource.h"
#include "Engine/Models/Resources/Texture/TextureCache.h"
#include "Tools/MipMapBuilder/Source/MipMapBuilder.h"
#include "Engine/Models/Resources/Cache/RawDataCache.h"

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
TextureLoader::TextureLoader( bool loadFormMemory )
    : m_loadFromMemory( loadFormMemory )
{}


// ******************************
//
IResourceNEWConstPtr TextureLoader::LoadResource( const ResourceDescConstPtr & desc ) const
{
	auto typedDesc = QueryTypedDesc< TextureResourceDescConstPtr >( desc );

	assert( typedDesc );

	TextureResourceConstPtr ret = nullptr;
	
	if ( typedDesc->IsCacheable() )
		return LoadFromCache( typedDesc );
	

	switch( typedDesc->GetLoadingType() )
	{
		case TextureResourceLoadingType::LOAD_ONLY_ORIGINAL_TEXTURE:
		{
			auto origRes = LoadSingleTexture( typedDesc->GetOrigTextureDesc(), false );
	
			ret = TextureResource::Create( origRes, nullptr );

			break;
		}

		case TextureResourceLoadingType::LOAD_ORIGINAL_TEXTURE_AND_MIP_MAPS:
		{
			auto origRes = LoadSingleTexture( typedDesc->GetOrigTextureDesc(), false );

			auto mipMapsSize = typedDesc->GetMipMapsDesc()->GetLevelsNum();

			std::vector< SingleTextureResourceConstPtr > mipMapsRes;

			for( SizeType i = 0; i < mipMapsSize; ++i )
				mipMapsRes.push_back( LoadSingleTexture( typedDesc->GetMipMapsDesc()->GetLevelDesc( i ), false ) );

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

			SingleTextureResourceConstPtr origRes;

			auto origKey = TextureCache::GenKeyForSingleTexture( typedDesc->GetOrigTextureDesc() );
			origRes = SingleTextureResource::Create( MemoryChunk::Create( mm[ 0 ].data, origW * origH * 4 ), origKey, origW, origH, TextureFormat::F_A8R8G8B8 );


			if( mm[ 0 ].width == origW && mm[ 0 ].height == origH )
				mipMapsRes.push_back( origRes );

			for( SizeType i = 1; i < mm.size(); ++i )
			{
				SizeType w = mm[ i ].width;
				SizeType h = mm[ i ].height;
				SizeType s = w * h * 4; // FIXME: Supporting only 32-bit image
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
		mmChunk = LoadImage( imgPath );

	if( !mmChunk )
		return nullptr;

	auto w			= sinlgeTextureResDesc->GetWidth();
	auto h			= sinlgeTextureResDesc->GetHeight();
	auto format		= sinlgeTextureResDesc->GetFormat();

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


// ******************************
//
TextureResourceConstPtr TextureLoader::LoadFromCache( const TextureResourceDescConstPtr & textureResDesc )
{
	auto origDesc = textureResDesc->GetOrigTextureDesc();

	auto origRes = LoadSingleTexture( origDesc );

	auto mmDesc = textureResDesc->GetMipMapsDesc();

	MipMapResourceConstPtr mmRes = nullptr;

	if( mmDesc )
	{
		std::vector< SingleTextureResourceConstPtr > mmVecTmp;

		for( SizeType i = 0; i < mmDesc->GetLevelsNum(); ++i )
		{
			auto desc = mmDesc->GetLevelDesc( i );
			mmVecTmp.push_back( LoadSingleTexture( desc ) );
		} 

		mmRes = MipMapResource::Create( mmVecTmp );
	}

	auto ret = TextureResource::Create( origRes, mmRes );

	TextureCache::GetInstance().Add( textureResDesc, ret );

	return ret;
}


// ******************************
//
ResourceHandle *        TextureLoader::LoadResource        ( IResource * res )  const
{
    unsigned int width   = 0;
    unsigned int height  = 0;
    unsigned int bpp     = 0;

	auto filePath = res->GetFilePath();

	MemoryChunkConstPtr data = nullptr;

	if( filePath.find(".raw") != std::string::npos)
    {
		data = TextureHelper::LoadRAW( filePath );
    }
	else
    {
		data = TextureHelper::LoadImg( filePath, &width, &height, &bpp );
    }

    if ( data != nullptr )
    {
        auto texExtra = new TextureExtraData( width, height, bpp, TextureFormat::F_A8R8G8B8, TextureType::T_2D );
        return new ResourceHandle( data, width * height * bpp / 8, texExtra );
    }

    return nullptr;
}

// ******************************
//
SizeType TextureExtraData::GetWidth            () const
{
    return m_width;
}

// ******************************
//
SizeType TextureExtraData::GetHeight           () const
{
    return m_height;
}

// ******************************
//
SizeType TextureExtraData::GetBitsPerPixel     () const
{
    return m_bitsPerPixel;
}

// ******************************
//
TextureFormat TextureExtraData::GetFormat () const
{
    return m_format;
}

// ******************************
//
TextureType TextureExtraData::GetType     () const
{
    return m_type;
}

// ******************************
//
TextureExtraData::TextureExtraData()
    : ResourceExtraData( ResourceExtraKind::RE_TEXTURE )
{
}

// ******************************
//
TextureExtraData::TextureExtraData( SizeType w, SizeType h, SizeType bitsPerPixel, TextureFormat format, TextureType type )
    : ResourceExtraData( ResourceExtraKind::RE_TEXTURE )
    , m_width( w )
    , m_height( h )
    , m_bitsPerPixel( bitsPerPixel )
    , m_format( format )
    , m_type( type )
{}

} // model
} // bv
