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

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <FreeType/ftglyph.h>

#include <boost/filesystem/path.hpp>

namespace bv { namespace model {

// *********************************
//
void GlyphCoords::Save( std::ostream& out ) const
{
    boost::archive::text_oarchive oa( out );
    oa << *this;
}

// *********************************
//
void GlyphCoords::Load( std::istream& in )
{
    boost::archive::text_iarchive ia( in );
    ia >> *this;
}

// *********************************
//
TextAtlas::TextAtlas()
    : m_glyphWidth( 0 )
    , m_glyphHeight( 0 )
{}

// *********************************
//
TextAtlas::TextAtlas( unsigned int w, unsigned int h, unsigned int bitsPerPixel, unsigned int gw, unsigned int gh )
    : m_glyphWidth( gw )
    , m_glyphHeight( gh )
{
    auto size   = w * h * bitsPerPixel / 8;
    auto data  = std::make_shared< MemoryChunk >( new char[ size ], size );
    m_textureHandle = ResourceHandlePtr( new ResourceHandle( data, size, new TextureExtraData( w, h, bitsPerPixel, TextureFormat::F_A8R8G8B8, TextureType::T_2D ) ) );
}

// *********************************
//
TextAtlas*      TextAtlas::Crate           ( unsigned int w, unsigned int h, unsigned int bitsPrePixel, unsigned int gw, unsigned int gh )
{
    return new TextAtlas(w, h, bitsPrePixel, gw, gh);
}

// *********************************
//
Text::Text( const std::wstring& text, const std::string& fontFile, unsigned int fontSize, unsigned int blurSize )
    : m_text( text )
    , m_fontFile( fontFile )
    , m_fontSize( fontSize )
    , m_blurSize( blurSize )
{
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
unsigned int            TextAtlas::GetSizeInBytes  () const
{
    return m_textureHandle->GetSize();
}

// *********************************
//
void                    TextAtlas::SetGlyphCoords  ( wchar_t wch, const GlyphCoords& coords )
{
    m_glyphsPositions.insert(std::make_pair( wch, coords ) );
}

// *********************************
//
unsigned int            TextAtlas::GetBitsPerPixel () const
{
    assert( m_textureHandle->GetExtra()->GetResourceExtraKind() == ResourceExtraKind::RE_TEXTURE );
    auto texExtraData = static_cast< const TextureExtraData * >( m_textureHandle->GetExtra() );
    return texExtraData->GetBitsPerPixel();
}

// *********************************
//
unsigned int            TextAtlas::GetWidth        () const
{
    assert( m_textureHandle->GetExtra()->GetResourceExtraKind() == ResourceExtraKind::RE_TEXTURE );
    auto texExtraData = static_cast< const TextureExtraData * >( m_textureHandle->GetExtra() );
    return texExtraData->GetWidth();
}

// *********************************
//
unsigned int            TextAtlas::GetHeight       () const
{
    assert( m_textureHandle->GetExtra()->GetResourceExtraKind() == ResourceExtraKind::RE_TEXTURE );
    auto texExtraData = static_cast< const TextureExtraData * >( m_textureHandle->GetExtra() );
    return texExtraData->GetHeight();
}

// *********************************
//
const GlyphCoords*      TextAtlas::GetGlyphCoords  ( wchar_t c ) const
{
    auto it = m_glyphsPositions.find(c);

    if( it != m_glyphsPositions.end() )
    {
        return &( it->second );
    }

    return nullptr;
}

// *********************************
//
float                   TextAtlas::GetKerning      ( wchar_t c0, wchar_t c1 ) const
{
    auto it = m_kerningMap.find( std::make_pair( c0, c1 ) );

    if( it != m_kerningMap.end() )
        return it->second;
    else
        return 0.f;
}

// *********************************
//
unsigned int            TextAtlas::GetGlyphX       ( wchar_t c ) const
{
    return GetGlyphCoords( c )->textureX;
}

// *********************************
//
unsigned int            TextAtlas::GetGlyphY       ( wchar_t c ) const
{
    return GetGlyphCoords( c )->textureY;
}

// *********************************
//
unsigned int            TextAtlas::GetGlyphWidth   ( wchar_t c ) const
{
    return GetGlyphCoords( c )->width;
}

// *********************************
//
unsigned int            TextAtlas::GetGlyphHeight  ( wchar_t c ) const
{
    return GetGlyphCoords( c )->height;
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

#define GENERATE_TEST_BMP_FILE

struct GlyphDataInfo
{
    Glyph*          glyph;
    unsigned int    pitch;
    char*           data;

    GlyphDataInfo( Glyph* g, unsigned int p, char* d)
        : glyph( g )
        , pitch( p )
        , data( d )
    {}
};

// *********************************
//
TextAtlas*          Text::LoadFromCache()
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

namespace
{
// *********************************
//
std::map< std::pair< wchar_t, wchar_t >, float >        BuildKerning    ( FT_Face face, const std::wstring & text )
{
    std::map< std::pair< wchar_t, wchar_t >, float > ret;

    for( auto wch0 : text )
    {
        for( auto wch1 : text )
        {
            FT_Vector  kerning;

            auto leftGlyphIndex = FT_Get_Char_Index( face, wch0 );
            auto rightGlyphIndex = FT_Get_Char_Index( face, wch1 );

            auto error = FT_Get_Kerning(    face,           /* handle to face object */
                                            leftGlyphIndex,           /* left glyph index      */
                                            rightGlyphIndex,           /* right glyph index     */
                                            FT_KERNING_DEFAULT,   /* kerning mode          */
                                            &kerning );     /* target vector         */


            if( error == 0 )
            {
                ret[ std::make_pair( wch0, wch1 ) ] = float( kerning.x >> 6 );
            }
        }
    }

    return ret;
}

//#define MAKE_FREETYPE_TESTING_TEXT

#ifdef MAKE_FREETYPE_TESTING_TEXT

unsigned char * output = nullptr;

void my_draw_bitmap( FT_Bitmap* bitmap, int pen_x, int pen_y )
{
    for( int y = 0; y < bitmap->rows; ++y )
    {
        for( int x = 0; x < bitmap->width; ++x )
        {
            float alpha = bitmap->buffer[ y * bitmap->pitch + x ] / 255.f;

            float topCol = float( bitmap->buffer[ y * bitmap->pitch + x ] ) * alpha;
            float backCol = float( output[ ( ( pen_y + y ) * 1920 + ( pen_x + x ) ) * 4 ] ) * ( 1.f - alpha );

            output[ ( ( pen_y + y ) * 1920 + ( pen_x + x ) ) * 4 ] = unsigned char( topCol + backCol );
            output[ ( ( pen_y + y ) * 1920 + ( pen_x + x ) ) * 4 + 1 ] = unsigned char( topCol + backCol );
            output[ ( ( pen_y + y ) * 1920 + ( pen_x + x ) ) * 4 + 2 ] = unsigned char( topCol + backCol );
            output[ ( ( pen_y + y ) * 1920 + ( pen_x + x ) ) * 4 + 3 ] = unsigned char( topCol + backCol );
        }
    }
}

void GenrateTestFreeTypeText( const std::wstring& text, FT_Face face )
{
    output = new unsigned char[ 1920 * 1080 * 4 ];
    memset( output, 0, sizeof( output ) );


  FT_GlyphSlot  slot = face->glyph;  /* a small shortcut */
  FT_UInt       glyph_index;
  FT_Bool       use_kerning;
  FT_UInt       previous;
  int           pen_x, pen_y, n;

  pen_x = 200;
  pen_y = 300;

  use_kerning = FT_HAS_KERNING( face );
  previous    = 0;

  for ( n = 0; n < (int)text.size(); n++ )
  {
      std::cout << "char "  << text[n] << std::endl;
    /* convert character code to glyph index */
    glyph_index = FT_Get_Char_Index( face, text[n] );

    /* retrieve kerning distance and move pen position */
    if ( use_kerning && previous && glyph_index )
    {
      FT_Vector  delta;


      FT_Get_Kerning( face, previous, glyph_index,
                      FT_KERNING_DEFAULT, &delta );

      pen_x += delta.x >> 6;

      std::cout << "kerning " << ( delta.x >> 6 ) << std::endl;
    }

    /* load glyph image into the slot (erase previous one) */
    auto error = FT_Load_Glyph( face, glyph_index, FT_LOAD_RENDER );
    if ( error )
      continue;  /* ignore errors */

    /* now draw to our target surface */
    my_draw_bitmap( &slot->bitmap,
                    pen_x + slot->bitmap_left,
                    pen_y - slot->bitmap_top );

    std::cout << "pen_x "  << pen_x << " pen_y " << pen_y << std::endl;

    /* increment pen position */
    pen_x += slot->advance.x >> 6;

    std::cout << "advance.x "  << ( slot->advance.x >> 6 ) << std::endl;

    /* record current glyph index */
    previous = glyph_index;
  }

  TextureHelper::WriteBMP( "testFreeType.bmp", MemoryChunkConstPtr( new MemoryChunk( ( char* )output, 1920 * 1080 * 4 ) ), 1920, 1080, 32 );

}

#endif // MAKE_FREETYPE_TESTING_TEXT

}

// *********************************
//
void                Text::BuildAtlas        ()
{
    m_atlas = LoadFromCache();

    if( m_atlas != nullptr )
        return;

    unsigned int glyphsNum = 0;

    FT_Library ft;
    if (FT_Init_FreeType (&ft))
    {
        std::cerr << "Could not init FreeType library\n" << std::endl;
        return;
    }

    FT_Face face;
    if (FT_New_Face (ft, m_fontFile.c_str(), 0, &face))
    {
        std::cerr << "Could not open font\n" << std::endl;
        return;
    }

    //int padding_px          = 0;                // total space in glyph size for outlines
    int slot_glyph_size     = m_fontSize;       // glyph maximum size in pixels

    FT_Set_Pixel_Sizes( face, m_fontSize, m_fontSize );

    std::vector< GlyphDataInfo >    glyphsDataInfos;

    int maxWidth = 0;
    int maxHeight = 0;

    for ( auto ch : m_text )
    {
        auto it = m_glyphs.find( ch );

        if( it != m_glyphs.end() )
        {
            continue;
        }

        if (FT_Load_Char (face, ch, FT_LOAD_RENDER)) 
        {
            std::cerr << "Could not load character " << ch << std::endl;
            return;
        }

        // draw glyph image anti-aliased
        FT_Render_Glyph (face->glyph, FT_RENDER_MODE_NORMAL);
        // get dimensions of bitmap
        auto newGlyph = new Glyph();

        newGlyph->code = ch;
        newGlyph->size = m_fontSize;
        newGlyph->width = face->glyph->bitmap.width;
        newGlyph->height = face->glyph->bitmap.rows;

        if( (int)newGlyph->height > maxHeight )
            maxHeight = newGlyph->height;

        if( (int)newGlyph->width > maxWidth )
            maxWidth = newGlyph->width;


        newGlyph->bearingX = face->glyph->bitmap_left;
        newGlyph->bearingY = face->glyph->bitmap_top;
        newGlyph->advanceX = face->glyph->advance.x >> 6;
        newGlyph->advanceY = face->glyph->advance.y >> 6;
        //newGlyph->padding = padding_px;

        char* glyphData = (char*)malloc( newGlyph->height * face->glyph->bitmap.pitch );

        // copy buffer because it seems to be overwritten
        memcpy ( glyphData, face->glyph->bitmap.buffer, newGlyph->height * face->glyph->bitmap.pitch );
        glyphsDataInfos.push_back( GlyphDataInfo( newGlyph, face->glyph->bitmap.pitch, glyphData ) );
        m_glyphs[ ch ] = newGlyph;

        glyphsNum++;
    }

    unsigned int atlasSize = ( unsigned int )std::ceil( sqrt( (float)glyphsNum ) );

    int padding = this->m_blurSize + 1;

    maxWidth  = maxWidth + padding * 2;
    maxHeight = maxHeight + padding * 2;

    unsigned int altlasWidth = maxWidth * atlasSize;
    unsigned int altlasHeight = maxHeight * atlasSize;

    m_atlas = TextAtlas::Crate( altlasWidth, altlasHeight, 32, maxWidth, maxHeight );

    char* atlasData = const_cast< char * >( m_atlas->GetWritableData()->Get() );// FIXME: Remove const_cast

    auto atlasColumns  =  altlasWidth / maxWidth;

    std::vector<bool>   textureCoordsSet( glyphsDataInfos.size(), false );


    for (unsigned int y = 0; y < m_atlas->GetHeight(); y++) 
    {
        for (unsigned int x = 0; x < m_atlas->GetWidth(); x++)
        {
    
            // work out which grid slot[col][row] we are in e.g out of 16x16
            unsigned int col = x / maxWidth;
            unsigned int row = y / maxHeight;
            unsigned int order = row * atlasColumns + col;

            unsigned int dataElem = y * m_atlas->GetWidth() * m_atlas->GetBitsPerPixel() / 8 + ( x * m_atlas->GetBitsPerPixel() / 8 );

            if( order < 0 || order >= glyphsDataInfos.size() )
            {
                atlasData[ dataElem ] = 0;
                atlasData[ dataElem + 1 ] = 0;
                atlasData[ dataElem + 2 ] = 0;
                atlasData[ dataElem + 3 ] = 0;
                continue;
            }

            // pixel indices within padded glyph slot area
            int x_loc = (x % maxWidth) - ( maxWidth - ((int)glyphsDataInfos[order].glyph->width ) ) + padding;
            int y_loc = (y % maxHeight)  - ( maxHeight - ((int)glyphsDataInfos[order].glyph->height) ) + padding;


            if( !textureCoordsSet[ order ] )
            {
                glyphsDataInfos[order].glyph->textureX = x + padding;
                glyphsDataInfos[order].glyph->textureY = y + padding;
                textureCoordsSet[ order ] = true;
                m_atlas->SetGlyphCoords(
                        glyphsDataInfos[order].glyph->code
                    ,   GlyphCoords(    glyphsDataInfos[order].glyph->textureX
                                    ,   glyphsDataInfos[order].glyph->textureY
                                    ,   maxWidth - 2 * padding
                                    ,   maxHeight - 2 * padding
                                    ,   (maxWidth - 2 * padding ) - (int)glyphsDataInfos[order].glyph->width
                                    ,   (maxHeight - 2 * padding ) - (int)glyphsDataInfos[order].glyph->height
                                    ,   glyphsDataInfos[order].glyph->width
                                    ,   glyphsDataInfos[order].glyph->height
                                    ,   glyphsDataInfos[order].glyph->bearingX
                                    ,   glyphsDataInfos[order].glyph->bearingY
                                    ,   glyphsDataInfos[order].glyph->advanceX
                                    ,   glyphsDataInfos[order].glyph->advanceY )
                    );
            }
                
            if (x_loc < 0 || y_loc < 0 || x_loc >= (int)glyphsDataInfos[order].glyph->width || y_loc >= (int)glyphsDataInfos[order].glyph->height )
            {
                atlasData[ dataElem ] = 0;
                atlasData[ dataElem + 1 ] = 0;
                atlasData[ dataElem + 2 ] = 0;
                atlasData[ dataElem + 3 ] = 0;
            } 
            else 
            {
                // this is 1, but it's safer to put it in anyway
                int bytes_per_pixel = glyphsDataInfos[order].glyph->width / glyphsDataInfos[order].pitch;
                int bytes_in_glyph  = glyphsDataInfos[order].glyph->height * glyphsDataInfos[order].pitch;
                int byte_order_in_glyph = y_loc * glyphsDataInfos[order].glyph->width + x_loc;
                // print byte from glyph
                auto p = glyphsDataInfos[order].data[byte_order_in_glyph];
                atlasData[ dataElem ] = p;
                atlasData[ dataElem + 1 ] = p;
                atlasData[ dataElem + 2 ] = p;
                atlasData[ dataElem + 3 ] = p;
            }
        }
    }

    auto fac = FontAtlasCache::Load( CACHE_DIRECTORY + CACHE_DB_FILE_NAME );

    if ( m_blurSize > 0 )
    {
        auto oldData = m_atlas->m_textureHandle;
        m_atlas->m_textureHandle->SetData( TextureHelper::Blur( oldData->GetData(), m_atlas->GetWidth(), m_atlas->GetHeight(), m_atlas->GetBitsPerPixel(), m_blurSize ) );
    }

    boost::filesystem::path fontPath( m_fontFile );
    auto fontName = fontPath.filename().string();

    m_atlas->m_kerningMap = BuildKerning( face, m_text );

    auto entry = new FontAtlasCacheEntry( m_atlas, fontName, m_fontSize, m_blurSize, m_fontFile, false, false );
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