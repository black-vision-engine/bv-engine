#include "Text.h"

#include "Glyph.h"
#include "System/FileIO.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <cassert>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <FreeType/ftglyph.h>

namespace bv { namespace model {

TextAtlas::TextAtlas( unsigned int w, unsigned int h, unsigned int bitsPrePixel, unsigned int gw, unsigned int gh )
    : m_width( w )
    , m_height( h )
    , m_glyphWidth( gw )
    , m_glyphHeight( gh )
    , m_bitsPerPixel( bitsPrePixel )
{
    m_data = new char[ w * h * bitsPrePixel / 8 ];
}

TextAtlas*      TextAtlas::Crate           ( unsigned int w, unsigned int h, unsigned int bitsPrePixel, unsigned int gw, unsigned int gh )
{
    return new TextAtlas(w, h, bitsPrePixel, gw, gh);
}

Text::Text( const std::wstring& text, const std::string& fontFile, unsigned int fontSize )
    : m_text( text )
    , m_fontFile( fontFile )
    , m_fontSize( fontSize )
{
    BuildAtlas();
}

#define GENERATE_TEST_RAW_FILE

void                Text::BuildAtlas()
{
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

    int padding_px          = 2;                // total space in glyph size for outlines
    int slot_glyph_size     = m_fontSize;       // glyph maximum size in pixels

    FT_Set_Pixel_Sizes( face, m_fontSize - padding_px, m_fontSize - padding_px );

    std::vector< unsigned char* >   glyphBuffer;
    std::vector< unsigned int >     gpitch;
    std::vector< Glyph* >           glyphVec;

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

        newGlyph->size = m_fontSize;
        newGlyph->width = face->glyph->bitmap.width;
        newGlyph->height = face->glyph->bitmap.rows;
        newGlyph->bearingX = face->glyph->bitmap_left;
        newGlyph->bearingY = face->glyph->bitmap_top;
        newGlyph->advance = face->glyph->advance.x;
        newGlyph->padding = padding_px;
        gpitch.push_back( face->glyph->bitmap.pitch );

        unsigned char* glyphData = (unsigned char*)malloc( newGlyph->height * face->glyph->bitmap.pitch );

        // copy buffer because it seems to be overwritten
        memcpy ( glyphData, face->glyph->bitmap.buffer, newGlyph->height * face->glyph->bitmap.pitch );
        glyphBuffer.push_back( glyphData );
        glyphVec.push_back( newGlyph );
        m_glyphs[ ch ] = newGlyph;
        // get y-offset to place glyphs on baseline. this is in the bounding box structure
        //FT_Glyph glyph; // a handle to the glyph image
        //if (FT_Get_Glyph (face->glyph, &glyph))
        //{
        //    std::cerr << "Could not get glyph handle " << i << std::endl;
        //    return;
        //}

        //// get bbox. "truncated" mode means get dimensions in pixels
        //FT_BBox bbox;
        //FT_Glyph_Get_CBox (glyph, FT_GLYPH_BBOX_TRUNCATE, &bbox);
        //gymin[i] = bbox.yMin;
        glyphsNum++;
    }

    std::vector< bool >             glyphVecTexSet( glyphsNum, false);

    unsigned int atlasSize = ( unsigned int )std::ceil( sqrt( (float)glyphsNum ) );

    unsigned int altlasWidth = m_fontSize * atlasSize;
    unsigned int altlasHeight = m_fontSize * atlasSize;

    m_atlas = TextAtlas::Crate( altlasWidth, altlasHeight, 32, m_fontSize, m_fontSize );

    char* atlasData = m_atlas->GetWritableData();

    std::stringstream dataStream;

    auto atlasColumns  = altlasWidth / m_fontSize;

    for (unsigned int y = 0; y < m_atlas->GetHeight(); y++) 
    {
        for (unsigned int x = 0; x < m_atlas->GetHeight(); x++)
        {
    
            // work out which grid slot[col][row] we are in e.g out of 16x16
            unsigned int col = x / m_fontSize;
            unsigned int row = y / m_fontSize;
            unsigned int order = row * atlasColumns + col;

            if( order < 0 || order >= glyphVec.size() )
            {
                dataStream << (unsigned char)0 << (unsigned char)0 << (unsigned char)0 << (unsigned char)0;
                continue;
            }

            // pixel indices within padded glyph slot area
            unsigned int x_loc = x % m_fontSize - padding_px / 2;
            unsigned int y_loc = y % m_fontSize - padding_px / 2;
                
            if (x_loc < 0 || y_loc < 0 || x_loc >= glyphVec[order]->width || y_loc >= glyphVec[order]->height )
            {
                dataStream << (unsigned char)0 << (unsigned char)0 << (unsigned char)0 << (unsigned char)0;
            } 
            else 
            {
                if( !glyphVecTexSet[ order ] )
                {
                    glyphVec[order]->textureX = x;
                    glyphVec[order]->textureY = y;
                    glyphVecTexSet[ order ] = true;
                }
                // this is 1, but it's safer to put it in anyway
                int bytes_per_pixel = glyphVec[order]->width / gpitch[order];
                int bytes_in_glyph = glyphVec[order]->height * gpitch[order];
                int byte_order_in_glyph = y_loc * glyphVec[order]->width + x_loc;
                // print byte from glyph
                dataStream << glyphBuffer[order][byte_order_in_glyph] << glyphBuffer[order][byte_order_in_glyph] << glyphBuffer[order][byte_order_in_glyph] << glyphBuffer[order][byte_order_in_glyph];
            }
        }
    }

    auto str = dataStream.str();

    assert(str.size() == m_atlas->GetHeight() * m_atlas->GetWidth() * 4); // FIXME:

    memcpy( atlasData, &str[ 0 ], dataStream.str().size() );

#ifdef GENERATE_TEST_RAW_FILE

    auto file = File::Open( "test.raw", File::FOMReadWrite );

    file << dataStream;

    file.Close();

#endif // GENERATE_TEST_RAW_FILE
}

} // model
} // bv