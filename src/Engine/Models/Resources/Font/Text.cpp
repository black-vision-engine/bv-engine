#include "Text.h"
#include "Serialize.h"

#include "Glyph.h"
#include "AtlasCache.h"
#include "System/FileIO.h"
#include "Engine/Models/Resources/TextureHelpers.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <FreeType/ftglyph.h>

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
    : m_width( 0 )
    , m_height( 0 )
    , m_glyphWidth( 0 )
    , m_glyphHeight( 0 )
    , m_bitsPerPixel( 0 )
    , m_data( nullptr )
{}

// *********************************
//
TextAtlas::TextAtlas( unsigned int w, unsigned int h, unsigned int bitsPrePixel, unsigned int gw, unsigned int gh )
    : m_width( w )
    , m_height( h )
    , m_glyphWidth( gw )
    , m_glyphHeight( gh )
    , m_bitsPerPixel( bitsPrePixel )
{
    m_data = new char[ GetSizeInBytes() ];
}

// *********************************
//
TextAtlas*      TextAtlas::Crate           ( unsigned int w, unsigned int h, unsigned int bitsPrePixel, unsigned int gw, unsigned int gh )
{
    return new TextAtlas(w, h, bitsPrePixel, gw, gh);
}

// *********************************
//
Text::Text( const std::wstring& text, const std::string& fontFile, unsigned int fontSize )
    : m_text( text )
    , m_fontFile( fontFile )
    , m_fontSize( fontSize )
{
    BuildAtlas();
}

// *********************************
//
const char*             TextAtlas::GetData         () const
{
    return m_data;
}

// *********************************
//
char*                   TextAtlas::GetWritableData ()
{
    return m_data;
}

// *********************************
//
unsigned int            TextAtlas::GetSizeInBytes  () const
{
    return ( m_bitsPerPixel / 8 ) * m_height * m_width;
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
    return m_bitsPerPixel;
}

// *********************************
//
unsigned int            TextAtlas::GetWidth        () const
{
    return m_width;
}

// *********************************
//
unsigned int            TextAtlas::GetHeight       () const
{
    return m_height;
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

    auto entry = fac->GetEntry( "ARIAL", m_fontSize, m_fontFile, false, false );

    if( entry != nullptr )
        return entry->m_textAtlas;
    else
        return nullptr;
}

// *********************************
//
void                Text::BuildAtlas()
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
        newGlyph->advance = face->glyph->advance.x;
        //newGlyph->padding = padding_px;

        char* glyphData = (char*)malloc( newGlyph->height * face->glyph->bitmap.pitch );

        // copy buffer because it seems to be overwritten
        memcpy ( glyphData, face->glyph->bitmap.buffer, newGlyph->height * face->glyph->bitmap.pitch );
        glyphsDataInfos.push_back( GlyphDataInfo( newGlyph, face->glyph->bitmap.pitch, glyphData ) );
        m_glyphs[ ch ] = newGlyph;

        glyphsNum++;
    }

    unsigned int atlasSize = ( unsigned int )std::ceil( sqrt( (float)glyphsNum ) );

    int padding = 1;

    maxWidth  = maxWidth + padding * 2;
    maxHeight = maxHeight + padding * 2;

    unsigned int altlasWidth = maxWidth * atlasSize;
    unsigned int altlasHeight = maxHeight * atlasSize;

    m_atlas = TextAtlas::Crate( altlasWidth, altlasHeight, 32, maxWidth, maxHeight );

    char* atlasData = m_atlas->GetWritableData();

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
                                    ,   glyphsDataInfos[order].glyph->bearingY )
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
    auto entry = new FontAtlasCacheEntry( m_atlas, "ARIAL", m_fontSize, m_fontFile, false, false );
    fac->AddEntry( *entry );

#ifdef GENERATE_TEST_BMP_FILE

    TextureHelper::WriteBMP( "test.bmp", atlasData, m_atlas->GetWidth(), m_atlas->GetHeight(), m_atlas->GetBitsPerPixel() );

#endif // GENERATE_TEST_BMP_FILE
}

} // model
} // bv