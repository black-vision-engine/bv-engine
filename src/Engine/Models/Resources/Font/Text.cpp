#include "Text.h"

#include "boost/archive/text_oarchive.hpp"
#include "boost/archive/text_iarchive.hpp"
#include "boost/serialization/map.hpp"

#include "Serialize.h"


#include "Glyph.h"
#include "AtlasCache.h"
#include "System/FileIO.h"
#include "Engine/Models/Resources/TextureHelpers.h"
#include "Engine/Models/Resources/TextureLoader.h"
#include "Engine/Models/Resources/Font/Engines/FreeTypeEngine.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>

#include <boost/filesystem/path.hpp>

namespace bv { namespace model {

//// *********************************
////
//void GlyphCoords::Save( std::ostream& out ) const
//{
//    boost::archive::text_oarchive oa( out );
//    oa << *this;
//}
//
//// *********************************
////
//void GlyphCoords::Load( std::istream& in )
//{
//    boost::archive::text_iarchive ia( in );
//    ia >> *this;
//}

// *********************************
//
TextAtlas::TextAtlas()
    : m_glyphWidth( 0 )
    , m_glyphHeight( 0 )
{}

// *********************************
//
TextAtlas::TextAtlas( SizeType w, SizeType h, SizeType bitsPerPixel, SizeType gw, SizeType gh )
    : m_glyphWidth( gw )
    , m_glyphHeight( gh )
{
    auto size   = w * h * bitsPerPixel / 8;
    auto data  = std::make_shared< MemoryChunk >( new char[ size ], size );

	TextureFormat tf;
	if( bitsPerPixel == 8 )
		tf = TextureFormat::F_A8;
	else if ( bitsPerPixel == 32 )
		tf = TextureFormat::F_A8R8G8B8;
	else
		assert(false);

	m_textureHandle = ResourceHandlePtr( new ResourceHandle( data, size, new TextureExtraData( w, h, bitsPerPixel, tf, TextureType::T_2D ) ) );
}

// *********************************
//
TextAtlas*      TextAtlas::Crate           ( SizeType w, SizeType h, SizeType bitsPrePixel, SizeType gw, SizeType gh )
{
    return new TextAtlas(w, h, bitsPrePixel, gw, gh);
}

// *********************************
//
Text::Text( const std::wstring& supportedCharsSet, const std::string& fontFile, SizeType fontSize, SizeType blurSize, SizeType outlineSize )
    : m_supportedCharsSet( supportedCharsSet )
    , m_fontFile( fontFile )
    , m_fontSize( fontSize )
    , m_blurSize( blurSize )
	, m_outlineSize( outlineSize )
{
	m_fontEngine = FreeTypeEngine::Create( fontFile, fontSize );
    BuildAtlas();
}

// *********************************
//
MemoryChunkConstPtr      TextAtlas::GetData         () const
{
    return m_textureHandle->GetData();
}

// *********************************
//
MemoryChunkConstPtr      TextAtlas::GetWritableData ()
{
    return m_textureHandle->GetWritableData();
}

// *********************************
//
SizeType				TextAtlas::GetSizeInBytes  () const
{
    return m_textureHandle->GetSize();
}

// *********************************
//
void                    TextAtlas::SetGlyph			( wchar_t wch, const Glyph * glyph )
{
    m_glyphs.insert(std::make_pair( wch, glyph ) );
}

// *********************************
//
SizeType				TextAtlas::GetBitsPerPixel () const
{
    assert( m_textureHandle->GetExtra()->GetResourceExtraKind() == ResourceExtraKind::RE_TEXTURE );
    auto texExtraData = static_cast< const TextureExtraData * >( m_textureHandle->GetExtra() );
    return texExtraData->GetBitsPerPixel();
}

// *********************************
//
SizeType				TextAtlas::GetWidth        () const
{
    assert( m_textureHandle->GetExtra()->GetResourceExtraKind() == ResourceExtraKind::RE_TEXTURE );
    auto texExtraData = static_cast< const TextureExtraData * >( m_textureHandle->GetExtra() );
    return texExtraData->GetWidth();
}

// *********************************
//
SizeType				TextAtlas::GetHeight       () const
{
    assert( m_textureHandle->GetExtra()->GetResourceExtraKind() == ResourceExtraKind::RE_TEXTURE );
    auto texExtraData = static_cast< const TextureExtraData * >( m_textureHandle->GetExtra() );
    return texExtraData->GetHeight();
}

// *********************************
//
const Glyph *			TextAtlas::GetGlyph			( wchar_t c ) const
{
    auto it = m_glyphs.find(c);

    if( it != m_glyphs.end() )
    {
        return it->second;
    }

    return nullptr;
}

// *********************************
//
Float32                  TextAtlas::GetKerning      ( wchar_t c0, wchar_t c1 ) const
{
    auto it = m_kerningMap.find( std::make_pair( c0, c1 ) );

    if( it != m_kerningMap.end() )
        return it->second;
    else
        return 0.f;
}

// *********************************
//
SizeType				TextAtlas::GetGlyphX       ( wchar_t c ) const
{
    return GetGlyph( c )->textureX;
}

// *********************************
//
SizeType				TextAtlas::GetGlyphY       ( wchar_t c ) const
{
    return GetGlyph( c )->textureY;
}

// *********************************
//
SizeType				TextAtlas::GetGlyphWidth   ( wchar_t c ) const
{
    return GetGlyph( c )->width;
}

// *********************************
//
SizeType				TextAtlas::GetGlyphHeight  ( wchar_t c ) const
{
    return GetGlyph( c )->height;
}

// *********************************
//
void                    TextAtlas::Save( std::ostream& out ) const
{
    boost::archive::text_oarchive oa( out );
    oa << *this;
}

// *********************************
//
void                    TextAtlas::Load( std::istream& in )
{
    boost::archive::text_iarchive ia( in );
    ia >> *this;
}

// *********************************
//
ResourceHandlePtr		TextAtlas::GetResourceHandle() const
{
	return m_textureHandle;
}

// *********************************
//

#define GENERATE_TEST_BMP_FILE

// *********************************
//
const TextAtlas *	Text::LoadFromCache()
{
    auto fac = FontAtlasCache::Load( CACHE_DIRECTORY + CACHE_DB_FILE_NAME );

    boost::filesystem::path fontPath( m_fontFile );
    auto fontName = fontPath.filename().string();

    auto entry = fac->GetEntry( fontName, m_fontSize, this->m_blurSize, m_fontFile, false, false );

    if( entry != nullptr )
        return entry->m_textAtlas;
    else
        return nullptr;
}

// *********************************
//
void                Text::BuildAtlas        ()
{
    m_atlas = LoadFromCache();

    if( m_atlas != nullptr )
        return;

	m_atlas = m_fontEngine->CreateAtlas(this->m_blurSize + 1, m_supportedCharsSet );

    if ( m_blurSize > 0 )
    {
        auto oldData = m_atlas->m_textureHandle;
        m_atlas->m_textureHandle->SetData( TextureHelper::Blur( oldData->GetData(), m_atlas->GetWidth(), m_atlas->GetHeight(), m_atlas->GetBitsPerPixel(), m_blurSize ) );
    }

	auto fac = FontAtlasCache::Load( CACHE_DIRECTORY + CACHE_DB_FILE_NAME );

    boost::filesystem::path fontPath( m_fontFile );
    auto fontName = fontPath.filename().string();

    auto entry = new FontAtlasCacheEntry( m_atlas, fontName, m_fontSize, m_blurSize, m_fontFile, "", false, false );
    fac->AddEntry( *entry );

#ifdef GENERATE_TEST_BMP_FILE

    TextureHelper::WriteBMP( "test.bmp", m_atlas->GetData(), m_atlas->GetWidth(), m_atlas->GetHeight(), m_atlas->GetBitsPerPixel() );
#endif // GENERATE_TEST_BMP_FILE

#ifdef MAKE_FREETYPE_TESTING_TEXT
    GenrateTestFreeTypeText( L"AV::AVAVA", face );
#endif // MAKE_FREETYPE_TESTING_TEXT
}

} // model
} // bv