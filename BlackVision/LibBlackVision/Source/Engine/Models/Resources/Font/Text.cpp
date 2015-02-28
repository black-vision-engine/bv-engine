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

#include "Serialize.h"

#include "Glyph.h"
#include "AtlasCache.h"
#include "System/FileIO.h"
#include "Engine/Models/Resources/TextureHelpers.h"
#include "Engine/Models/Resources/TextureLoader.h"
#include "Engine/Models/Resources/Font/Engines/FreeTypeEngine.h"


namespace bv { namespace model {

// *********************************
//
TextConstPtr Text::Create(const std::wstring& supportedCharsSet
						, const std::string& fontFile
						, SizeType fontSize
						, SizeType blurSize
						, SizeType outlineSize )
{
	return std::make_shared< Text >( supportedCharsSet, fontFile, fontSize, blurSize, outlineSize );
}


// *********************************
//
Text::Text( const std::wstring& supportedCharsSet, const std::string& fontFile, SizeType fontSize, SizeType blurSize, SizeType outlineSize )
    : m_supportedCharsSet( supportedCharsSet )
    , m_fontFile( fontFile )
    , m_fontSize( fontSize )
    , m_blurSize( blurSize )
	, m_outlineWidth( outlineSize )
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

	auto entry = fac->GetEntry( fontName, m_fontSize, this->m_blurSize, m_outlineWidth, m_fontFile, false, false );

    if( entry != nullptr )
        return entry->m_textAtlas;
    else
        return nullptr;
}

// *********************************
//
void Text::BuildAtlas        ()
{
    m_atlas = LoadFromCache();

    if( m_atlas != nullptr )
        return;

	m_atlas = m_fontEngine->CreateAtlas(this->m_blurSize + 1, m_outlineWidth, m_supportedCharsSet );

    if ( m_blurSize > 0 )
    {
		auto oldData = std::const_pointer_cast< MemoryChunk >( m_atlas->m_textureResource->GetOriginal()->GetData() );
		auto bluredData = TextureHelper::Blur( oldData, (unsigned int) m_atlas->GetWidth(), (unsigned int) m_atlas->GetHeight(), (unsigned int) m_atlas->GetBitsPerPixel(), (unsigned int) m_blurSize );
		auto newSingleTextureRes = SingleTextureResource::
		m_atlas->m_textureResource = TextureResource::Create( (TextureHelper::Blur( oldData, (unsigned int) m_atlas->GetWidth(), (unsigned int) m_atlas->GetHeight(), (unsigned int) m_atlas->GetBitsPerPixel(), (unsigned int) m_blurSize ) );
    }

	auto fac = FontAtlasCache::Load( CACHE_DIRECTORY + CACHE_DB_FILE_NAME );

    boost::filesystem::path fontPath( m_fontFile );
    auto fontName = fontPath.filename().string();

    auto entry = new FontAtlasCacheEntry( m_atlas, fontName, m_fontSize, m_blurSize, m_outlineWidth, m_fontFile, "", false, false );
    fac->AddEntry( *entry );

#ifdef GENERATE_TEST_BMP_FILE

    TextureHelper::WriteBMP( "test.bmp", m_atlas->GetData(), (unsigned int) m_atlas->GetWidth(), (unsigned int) m_atlas->GetHeight(), (unsigned int) m_atlas->GetBitsPerPixel() );
#endif // GENERATE_TEST_BMP_FILE

#ifdef MAKE_FREETYPE_TESTING_TEXT
    GenrateTestFreeTypeText( L"AV::AVAVA", face );
#endif // MAKE_FREETYPE_TESTING_TEXT
}

} // model
} // bv