#include "Text.h"
#include "TextAtlas.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>

#pragma warning(push)
#pragma warning(disable : 4512) //warning C4512: 'boost::archive::detail::helper_collection::predicate' : assignment operator could not be generated

#include "boost/archive/text_oarchive.hpp"
#include "boost/archive/text_iarchive.hpp"
#include "boost/serialization/map.hpp"
#include "boost/filesystem/path.hpp"

#pragma warning(pop)

#include "Glyph.h"
#include "AtlasCache.h"
#include "IO/FileIO.h"
#include "LibImage.h"
#include "LibEffect.h"
#include "Assets/Assets.h"
#include "Assets/Font/Engines/FreeTypeEngine.h"
#include "MipMapBuilder.h"
#include "Assets/Texture/TextureCache.h"


namespace bv { 

// *********************************
//
TextConstPtr Text::Create(const std::wstring& supportedCharsSet
						, const std::string& fontFile
						, UInt32 fontSize
						, UInt32 blurSize
						, UInt32 outlineSize
						, bool withMipmaps )
{
	return std::make_shared< Text >( supportedCharsSet, fontFile, fontSize, blurSize, outlineSize, withMipmaps );
}


// *********************************
//
Text::Text( const std::wstring& supportedCharsSet, const std::string& fontFile, UInt32 fontSize, UInt32 blurSize, UInt32 outlineSize, bool withMipmaps )
    : m_supportedCharsSet( supportedCharsSet )
    , m_fontFile( fontFile )
    , m_fontSize( fontSize )
    , m_blurSize( blurSize )
	, m_outlineWidth( outlineSize )
	, m_withMipmaps( withMipmaps )
{
	m_fontEngine = FreeTypeEngine::Create( fontFile, fontSize );
    BuildAtlas();
}

//#define GENERATE_TEST_BMP_FILE

// *********************************
//
TextAtlasConstPtr Text::LoadFromCache( bool useMipMaps )
{
    auto fac = FontAtlasCache::Load( CACHE_DIRECTORY + CACHE_DB_FILE_NAME );

    boost::filesystem::path fontPath( m_fontFile );
    auto fontName = fontPath.filename().string();

	auto entry = fac->GetEntry( fontName, m_fontSize, this->m_blurSize, m_outlineWidth, useMipMaps );

    if( entry != nullptr )
	{
		return entry->m_textAtlas;
	}
    else
	{
		return nullptr;
	}
}

// *********************************
//
void				Text::AddToCache()
{
	auto fac = FontAtlasCache::Load( CACHE_DIRECTORY + CACHE_DB_FILE_NAME );

    boost::filesystem::path fontPath( m_fontFile );
    auto fontName = fontPath.filename().string();

	auto mmLevelsNum = m_atlas->m_textureAsset->GetMipMaps() ? m_atlas->m_textureAsset->GetMipMaps()->GetLevelsNum() : 0;

	auto entry = new FontAtlasCacheEntry( m_atlas, fontName, m_fontSize, m_blurSize, m_outlineWidth, m_fontFile, ( UInt32 )mmLevelsNum );
    fac->AddEntry( *entry );
}


// *********************************
//
namespace
{

// *********************************
//
UInt32 GetMMLevelsNum( UInt32 fontSize )
{
	return ( UInt32 )std::floor( std::log( fontSize ) / std::log( 2 ) );
}

// *********************************
//
UInt32 CalculatePadding( UInt32 fontSize, UInt32 blurSize, bool withMipMaps )
{
	UInt32 padding = blurSize + 1;

	if( withMipMaps )
	{
		auto mmLevelsNum = GetMMLevelsNum( fontSize );
		padding += 2 * mmLevelsNum;
	}

	return padding;
}

} // anonymous

// *********************************
//
void Text::BuildAtlas        ()
{
	bool useMipMaps = true;
    m_atlas = LoadFromCache( useMipMaps );

    if( m_atlas != nullptr )
	{
		assert( m_atlas->GetAsset()->GetOriginal() ); // If atlas is in db it should be also in raw data cache.
		return;
	}

	auto  padding = CalculatePadding( m_fontSize, m_blurSize, useMipMaps ); // Update padding in case of bluring the atlas.

	m_atlas = m_fontEngine->CreateAtlas( padding, m_outlineWidth, m_supportedCharsSet, useMipMaps );

	BlurAtlas();

	GenerateMipMaps();

	AddTexturesKey();

	AddToCache();

#ifdef GENERATE_TEST_BMP_FILE

    image::SaveBMPImage( "test.bmp", m_atlas->GetData(), (unsigned int) m_atlas->GetWidth(), (unsigned int) m_atlas->GetHeight(), (unsigned int) m_atlas->GetBitsPerPixel() );
#endif // GENERATE_TEST_BMP_FILE

#ifdef MAKE_FREETYPE_TESTING_TEXT
    GenrateTestFreeTypeText( L"AV::AVAVA", face );
#endif // MAKE_FREETYPE_TESTING_TEXT
}


// *********************************
//
void Text::GenerateMipMaps()
{
	UInt32 levelsNum = GetMMLevelsNum( m_fontSize );

	if( levelsNum > 0 )
	{
		tools::Image img = { m_atlas->m_textureAsset->GetOriginal()->GetData(), m_atlas->GetWidth(), m_atlas->GetHeight(), m_atlas->GetBitsPerPixel() };
		auto mipmap = tools::GenerateMipmaps( img, levelsNum, image::FilterType::FT_BILINEAR ); // FIXME: filter type is hardcoded.

		std::vector< SingleTextureAssetConstPtr > mipMapsRes;
		for( SizeType i = 0; i < mipmap.size(); ++i )
		{		
			mipMapsRes.push_back( SingleTextureAsset::Create( mipmap[ i ].data, "", mipmap[ i ].width, mipmap[ i ].height, TextureFormat::F_A8R8G8B8, true ) );
		}

		auto mipmaps = MipMapAsset::Create( mipMapsRes );

		std::const_pointer_cast< TextAtlas >( m_atlas )->m_textureAsset = TextureAsset::Create( m_atlas->m_textureAsset->GetOriginal(), mipmaps );
	}
}

// *********************************
//
void Text::AddTexturesKey()
{
	auto oldTA = m_atlas->m_textureAsset;
	auto atlasW = m_atlas->GetWidth();
	auto atlasH = m_atlas->GetHeight();

	UInt32 levelsNum = GetMMLevelsNum( m_fontSize );

	auto atlasAssetDesc = TextAtlas::GenerateTextAtlasAssetDescriptor( m_fontFile, atlasW, atlasH, m_fontSize, m_blurSize, m_outlineWidth, MipMapFilterType::BILINEAR, levelsNum );

	auto origKey = TextureCache::GenKeyForSingleTexture( atlasAssetDesc->GetOrigTextureDesc() );

	auto newOrigTexture = SingleTextureAsset::Create( oldTA->GetOriginal()->GetData(), origKey, atlasW, atlasH, TextureFormat::F_A8R8G8B8, true );

	std::vector< SingleTextureAssetConstPtr > mmsSTAs;

	for( SizeType i = 0; i < atlasAssetDesc->GetMipMapsDesc()->GetLevelsNum(); ++i )
	{
		auto key			= TextureCache::GenKeyForSingleTexture( atlasAssetDesc->GetMipMapsDesc()->GetLevelDesc( i ) );

		auto mm = oldTA->GetMipMaps()->GetLevel( i );
		auto w = mm->GetWidth();
		auto h = mm->GetHeight();
		auto f = mm->GetFormat();

		mmsSTAs.push_back( SingleTextureAsset::Create( mm->GetData(), key, w, h, f, true ) );
	}
	
	std::const_pointer_cast< TextAtlas >( m_atlas )->m_textureAsset = TextureAsset::Create( newOrigTexture, MipMapAsset::Create( mmsSTAs ) );
}

// *********************************
//
void Text::BlurAtlas()
{
	if( m_blurSize > 0 )
	{
		auto atlasW = m_atlas->GetWidth();
		auto atlasH = m_atlas->GetHeight();
		auto oldData = std::const_pointer_cast< MemoryChunk >( m_atlas->m_textureAsset->GetOriginal()->GetData() );

		//image::SaveBMPImage( "test.bmp", oldData, (unsigned int) m_atlas->GetWidth(), (unsigned int) m_atlas->GetHeight(), (unsigned int) m_atlas->GetBitsPerPixel() );


		//UInt32 w;
		//UInt32 h;
		//UInt32 bbp;
		//auto img = image::LoadImage( "100x100.png", &w, &h, &bbp );
		//auto bluredData = image::BlurImage( img, w, h, bbp, 5 );

		//image::SaveBMPImage( "testhost.bmp", bluredData, w, h, bbp );

		//bluredData = bv::effect::GLBlurImage( img, w, h, bbp, 5 );

		//image::SaveBMPImage( "testbgpu.bmp", bluredData, w, h, bbp );

		auto bluredData = bv::effect::GLBlurImage( oldData, m_atlas->GetWidth(), m_atlas->GetHeight(), m_atlas->GetBitsPerPixel(), m_blurSize );

		//image::SaveBMPImage( "testbgpu.bmp", bluredData, m_atlas->GetWidth(), m_atlas->GetHeight(), m_atlas->GetBitsPerPixel() );

		auto newSingleTextureRes = SingleTextureAsset::Create( bluredData, "", atlasW, atlasH, TextureFormat::F_A8R8G8B8, true );
		std::const_pointer_cast< TextAtlas >( m_atlas )->m_textureAsset = TextureAsset::Create( newSingleTextureRes, nullptr );
	}
}

} // bv