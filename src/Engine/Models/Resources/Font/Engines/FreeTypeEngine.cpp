#include "FreeTypeEngine.h"

#include "Engine/Models/Resources/Font/Glyph.h"
#include "Engine/Models/Resources/Font/Text.h"


#include "Engine/Models/Resources/TextureHelpers.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include <FreeType/ftglyph.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <map>

namespace bv { namespace model {

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

// *********************************
//
FreeTypeEngineConstPtr				FreeTypeEngine::Create()
{
	return std::make_shared< FreeTypeEngine >();
}

// *********************************
//
const TextAtlas *	FreeTypeEngine::CreateAtlas( const std::string& fontFilePath, SizeType fontSize, SizeType padding, const std::wstring & wcharsSet ) const
{
	unsigned int					glyphsNum	= 0;
	std::map< wchar_t, Glyph * >	glyphs;
	Int32							spadding	= (Int32)padding;

    FT_Library ft;
    if (FT_Init_FreeType (&ft))
    {
        std::cerr << "Could not init FreeType library\n" << std::endl;
        return nullptr;
    }

    FT_Face face;
    if (FT_New_Face (ft, fontFilePath.c_str(), 0, &face))
    {
        std::cerr << "Could not open font\n" << std::endl;
        return nullptr;
    }

    //int padding_px          = 0;                // total space in glyph size for outlines
    auto slot_glyph_size     = fontSize;       // glyph maximum size in pixels

    FT_Set_Pixel_Sizes( face, (FT_UInt)fontSize, (FT_UInt)fontSize );

    std::vector< GlyphDataInfo >    glyphsDataInfos;

    Int32 maxWidth = 0;
    Int32 maxHeight = 0;

    for ( auto ch : wcharsSet )
    {
        auto it = glyphs.find( ch );

        if( it != glyphs.end() )
        {
            continue;
        }

        if (FT_Load_Char (face, ch, FT_LOAD_RENDER)) 
        {
            std::cerr << "Could not load character " << ch << std::endl;
            return nullptr;
        }

        // draw glyph image anti-aliased
        FT_Render_Glyph (face->glyph, FT_RENDER_MODE_NORMAL);
		FT_Bitmap abitmap;

		auto outW = face->glyph->bitmap.width + 5;
		auto outH = face->glyph->bitmap.rows;

		abitmap.buffer = new unsigned char[outW * outH];
		memset( abitmap.buffer, 0, outW * outH );
		abitmap.pitch = outW;
		abitmap.rows = outH;
		abitmap.width = outW;
		abitmap.num_grays = FT_PIXEL_MODE_GRAY;
		abitmap.pixel_mode = FT_PIXEL_MODE_GRAY;
		if(true)
		{
			FT_Outline_Get_Bitmap( ft, &face->glyph->outline, &abitmap);
			TextureHelper::WriteBMP( "testFreeType.bmp", MemoryChunkConstPtr( new MemoryChunk( ( char* )abitmap.buffer, outW * outH ) ), outW, outH, 8 );
		}


        // get dimensions of bitmap
        auto newGlyph = new Glyph();

        newGlyph->code = ch;
        newGlyph->size = fontSize;
        newGlyph->width = face->glyph->bitmap.width;
        newGlyph->height = face->glyph->bitmap.rows;

        if( (int)newGlyph->height > maxHeight )
            maxHeight = (Int32)newGlyph->height;

        if( (int)newGlyph->width > maxWidth )
            maxWidth = (Int32)newGlyph->width;


        newGlyph->bearingX = face->glyph->bitmap_left;
        newGlyph->bearingY = face->glyph->bitmap_top;
        newGlyph->advanceX = face->glyph->advance.x >> 6;
        newGlyph->advanceY = face->glyph->advance.y >> 6;
        //newGlyph->padding = padding_px;

        char* glyphData = (char*)malloc( newGlyph->height * face->glyph->bitmap.pitch );

        // copy buffer because it seems to be overwritten
        memcpy ( glyphData, face->glyph->bitmap.buffer, newGlyph->height * face->glyph->bitmap.pitch );
        glyphsDataInfos.push_back( GlyphDataInfo( newGlyph, face->glyph->bitmap.pitch, glyphData ) );
        glyphs[ ch ] = newGlyph;

        glyphsNum++;
    }

    unsigned int atlasSize = ( unsigned int )std::ceil( sqrt( (float)glyphsNum ) );

    maxWidth  = maxWidth	+ spadding * 2;
    maxHeight = maxHeight	+ spadding * 2;

    size_t altlasWidth = maxWidth * atlasSize;
    size_t altlasHeight = maxHeight * atlasSize;

    auto atlas = TextAtlas::Crate( altlasWidth, altlasHeight, 32, maxWidth, maxHeight );

    char* atlasData = const_cast< char * >( atlas->GetWritableData()->Get() );// FIXME: Remove const_cast

    auto atlasColumns  =  altlasWidth / maxWidth;

    std::vector<bool>   textureCoordsSet( glyphsDataInfos.size(), false );


    for (SizeType y = 0; y < atlas->GetHeight(); y++) 
    {
        for (SizeType x = 0; x < atlas->GetWidth(); x++)
        {
    
            // work out which grid slot[col][row] we are in e.g out of 16x16
            SizeType col = x / maxWidth;
            SizeType row = y / maxHeight;
            SizeType order = row * atlasColumns + col;

            SizeType dataElem = y * atlas->GetWidth() * atlas->GetBitsPerPixel() / 8 + ( x * atlas->GetBitsPerPixel() / 8 );

            if( order < 0 || order >= glyphsDataInfos.size() )
            {
                atlasData[ dataElem ] = 0;
                atlasData[ dataElem + 1 ] = 0;
                atlasData[ dataElem + 2 ] = 0;
                atlasData[ dataElem + 3 ] = 0;
                continue;
            }

            // pixel indices within padded glyph slot area
            Int32 x_loc = (x % maxWidth)	- ( maxWidth	- ((Int32)glyphsDataInfos[order].glyph->width ) ) + spadding;
            Int32 y_loc = (y % maxHeight)	- ( maxHeight	- ((Int32)glyphsDataInfos[order].glyph->height) ) + spadding;


            if( !textureCoordsSet[ order ] )
            {
                glyphsDataInfos[order].glyph->textureX = x + padding;
                glyphsDataInfos[order].glyph->textureY = y + padding;
                textureCoordsSet[ order ] = true;
                atlas->SetGlyphCoords(
                        glyphsDataInfos[order].glyph->code
                    ,   GlyphCoords(    glyphsDataInfos[order].glyph->textureX
                                    ,   glyphsDataInfos[order].glyph->textureY
                                    ,   maxWidth - 2 * spadding
                                    ,   maxHeight - 2 * spadding
                                    ,   (maxWidth - 2 * spadding ) - (int)glyphsDataInfos[order].glyph->width
                                    ,   (maxHeight - 2 * spadding ) - (int)glyphsDataInfos[order].glyph->height
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
                auto bytes_per_pixel = glyphsDataInfos[order].glyph->width / glyphsDataInfos[order].pitch;
                auto bytes_in_glyph  = glyphsDataInfos[order].glyph->height * glyphsDataInfos[order].pitch;
                auto byte_order_in_glyph = y_loc * glyphsDataInfos[order].glyph->width + x_loc;
                // print byte from glyph
                auto p = glyphsDataInfos[order].data[byte_order_in_glyph];
                atlasData[ dataElem ] = p;
                atlasData[ dataElem + 1 ] = p;
                atlasData[ dataElem + 2 ] = p;
                atlasData[ dataElem + 3 ] = p;
            }
        }
    }

	atlas->m_kerningMap = BuildKerning( face, wcharsSet );

	return atlas;
}

// *********************************
//
const TextAtlas *	FreeTypeEngine::CreateAtlas( const std::string& fontFilePath, SizeType fontSize, SizeType padding, SizeType outline, const std::wstring & wcharsSet )	const
{
	return nullptr;
}

} // bv
} // model

namespace {
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

} // anonymous