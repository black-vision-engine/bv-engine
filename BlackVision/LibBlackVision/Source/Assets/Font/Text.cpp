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
#include "Assets/Assets.h"
#include "Assets/Font/Engines/FreeTypeEngine.h"


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

#define GENERATE_TEST_BMP_FILE

// *********************************
//
TextAtlasConstPtr Text::LoadFromCache()
{
    auto fac = FontAtlasCache::Load( CACHE_DIRECTORY + CACHE_DB_FILE_NAME );

    boost::filesystem::path fontPath( m_fontFile );
    auto fontName = fontPath.filename().string();

	auto entry = fac->GetEntry( fontName, m_fontSize, this->m_blurSize, m_outlineWidth, true );


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
void Text::BuildAtlas        ()
{
    m_atlas = LoadFromCache();

    if( m_atlas != nullptr )
	{
		return;
	}

	auto  padding = this->m_blurSize + 1; // Update padding in case of bluring the atlas.

	m_atlas = m_fontEngine->CreateAtlas( padding, m_outlineWidth, m_supportedCharsSet, true );

	assert( m_blurSize == 0 ); //TODO: Implement
  //  if ( m_blurSize > 0 )
  //  {
		//auto oldData = std::const_pointer_cast< MemoryChunk >( m_atlas->m_textureAsset->GetOriginal()->GetData() );
		//auto bluredData = TextureHelper::Blur( oldData, (unsigned int) m_atlas->GetWidth(), (unsigned int) m_atlas->GetHeight(), (unsigned int) m_atlas->GetBitsPerPixel(), (unsigned int) m_blurSize );
		//auto atlasFilePath = FontAtlasCache::GenerateTextAtlasCacheFileName(  )
		//auto newSingleTextureRes = SingleTextureAsset::Create( bluredData,  );
		//m_atlas->m_textureAsset = TextureAsset::Create( (TextureHelper::Blur( oldData, (unsigned int) m_atlas->GetWidth(), (unsigned int) m_atlas->GetHeight(), (unsigned int) m_atlas->GetBitsPerPixel(), (unsigned int) m_blurSize ) );
  //  }

	AddToCache();

#ifdef GENERATE_TEST_BMP_FILE

    image::SaveBMPImage( "test.bmp", m_atlas->GetData(), (unsigned int) m_atlas->GetWidth(), (unsigned int) m_atlas->GetHeight(), (unsigned int) m_atlas->GetBitsPerPixel() );
#endif // GENERATE_TEST_BMP_FILE

#ifdef MAKE_FREETYPE_TESTING_TEXT
    GenrateTestFreeTypeText( L"AV::AVAVA", face );
#endif // MAKE_FREETYPE_TESTING_TEXT
}

} // bv