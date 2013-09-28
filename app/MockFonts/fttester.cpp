#include "fttester.h"

#include <iostream>
#include <fstream>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <FreeType/ftglyph.h>

const std::string fontFile = "../dep/Media/fonts/arial.ttf";
FT_Library  GFreeType;
FT_Face     face;

namespace bv {

namespace {

// ***********************************
//
void face_info( FT_Face face, const std::string & fontName )
{
    std::cout << "Font:            " << fontName << std::endl;
    std::cout << "Num glyphs:      " << face->num_glyphs << std::endl;
    std::cout << "Flags:           " << face->face_flags << std::endl;
    std::cout << "Units per EM:    " << face->units_per_EM << std::endl;
    std::cout << "Num fixed sizes: " << face->num_fixed_sizes << std::endl;
    std::cout << "Available sizes: " << face->available_sizes << std::endl;
}
}

// ***********************************
//
void simple_freetype_test()
{
    auto error = FT_Init_FreeType( &GFreeType );

    if ( error )
    {
        std::cout << "FreeType error: " << error << std::endl; 
        return;
    }

    error = FT_New_Face( GFreeType, fontFile.c_str(), 0, &face );
    if ( error == FT_Err_Unknown_File_Format )
    {
        std::cout << "... the font file could be opened and read, but it appears that its font format is unsupported" << error << std::endl; 
        return;
    }
    else if ( error )
    {
        std::cout << "... another error code means that the font file could not be opened or read, or simply that it is broken... error: " << error << std::endl;
        return;
    }
    
    face_info( face, fontFile );
}

// ***********************************
//
void basic_write_atlas( const std::string & fontFile, const std::string & atlasFile )
{
    FT_Library ft;
    if (FT_Init_FreeType (&ft))
    {
        std::cerr << "Could not init FreeType library\n" << std::endl;
        return;
    }

    FT_Face face;
    if (FT_New_Face (ft, fontFile.c_str(), 0, &face))
    {
        std::cerr << "Could not open font\n" << std::endl;
        return;
    }

    std::ofstream file;
    file.open( atlasFile.c_str(), std::ios::out | std::ios::binary );
  
    int atlas_dimension_px  = 1024;             // atlas size in pixels
    int atlas_columns       = 16;               // number of glyphs across atlas
    int padding_px          = 6;                // total space in glyph size for outlines
    int slot_glyph_size     = 64;               // glyph maximum size in pixels
    int atlas_glyph_px      = 64 - padding_px;  // leave some padding for outlines

    int grows[256];     // glyph height in pixels
    int gwidth[256];    // glyph width in pixels
    int gpitch[256];    // bytes per row of pixels
    int gymin[256];     // offset for letters that dip below baseline like g and y

    unsigned char* glyph_buffer[256]; // stored glyph images

    // set height in pixels width 0 height 48 (48x48)
    FT_Set_Pixel_Sizes (face, 0, atlas_glyph_px);

    for ( unsigned long i = 33; i < 256; ++i )
    {
        if (FT_Load_Char (face, i, FT_LOAD_RENDER)) 
        {
            std::cerr << "Could not load character " << i << std::endl;
            return;
        }

        // draw glyph image anti-aliased
        FT_Render_Glyph (face->glyph, FT_RENDER_MODE_NORMAL);
        // get dimensions of bitmap
        grows[i] = face->glyph->bitmap.rows;
        gwidth[i] = face->glyph->bitmap.width;
        gpitch[i] = face->glyph->bitmap.pitch;
    
        // copy buffer because it seems to be overwritten
        glyph_buffer[i] = (unsigned char*)malloc (grows[i] * gpitch[i]);
        memcpy (glyph_buffer[i], face->glyph->bitmap.buffer, face->glyph->bitmap.rows * face->glyph->bitmap.pitch);
    
        // get y-offset to place glyphs on baseline. this is in the bounding box structure
        FT_Glyph glyph; // a handle to the glyph image
        if (FT_Get_Glyph (face->glyph, &glyph))
        {
            std::cerr << "Could not get glyph handle " << i << std::endl;
            return;
        }

        // get bbox. "truncated" mode means get dimensions in pixels
        FT_BBox bbox;
        FT_Glyph_Get_CBox (glyph, FT_GLYPH_BBOX_TRUNCATE, &bbox);
        gymin[i] = bbox.yMin;
    }


    for (int y = 0; y < atlas_dimension_px; y++) 
    {
        for (int x = 0; x < atlas_dimension_px; x++)
        {
    
            // work out which grid slot[col][row] we are in e.g out of 16x16
            int col = x / slot_glyph_size;
            int row = y / slot_glyph_size;
            int order = row * atlas_columns + col;
            int glyph_index = order + 32;
      
            // an actual glyph bitmap exists for these indices
            if (glyph_index > 32 && glyph_index < 256) 
            {
                // pixel indices within padded glyph slot area
                int x_loc = x % slot_glyph_size - padding_px / 2;
                int y_loc = y % slot_glyph_size - padding_px / 2;
                
                if (x_loc < 0 || y_loc < 0 || x_loc >= gwidth[glyph_index] || y_loc >= grows[glyph_index])
                {
                    file << (unsigned char)0 << (unsigned char)0 << (unsigned char)0 << (unsigned char)0;
                } 
                else 
                {
                  // this is 1, but it's safer to put it in anyway
                  int bytes_per_pixel = gwidth[glyph_index] / gpitch[glyph_index];
                  int bytes_in_glyph = grows[glyph_index] * gpitch[glyph_index];
                  int byte_order_in_glyph = y_loc * gwidth[glyph_index] + x_loc;
                  // print byte from glyph
                  file << glyph_buffer[glyph_index][byte_order_in_glyph] << glyph_buffer[glyph_index][byte_order_in_glyph] << glyph_buffer[glyph_index][byte_order_in_glyph] << glyph_buffer[glyph_index][byte_order_in_glyph];
                }
        // otherwise write black
            } 
            else 
            {
                file << (unsigned char)0 << (unsigned char)0 << (unsigned char)0 << (unsigned char)0;
            }
        }
    }

    // convert raw image to png
    file.close ();
    //system ("convert -depth 8 -size 1024x1024 rgba:atlas.raw atlas.png");
}

}
