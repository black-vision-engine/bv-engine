#include "FreeTypeEngine.h"

#include "Assets/Font/Glyph.h"
#include "Assets/Font/Text.h"
#include "Assets/Font/TextAtlas.h"
#include "Assets/Texture/TextureCache.h"

#include "LibImage.h"
#include "Tools/MipMapBuilder/Source/MipMapBuilder.h"

#include "Mathematics/Rect.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_STROKER_H
#include <FreeType/ftglyph.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <cmath>

namespace bv { 

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


// A horizontal pixel span generated by the FreeType renderer.

struct Span
{
  Span() { }
  Span( Int32 _x, Int32 _y, Int32 _width, Int32 _coverage)
  : x(_x), y(_y), width(_width), coverage(_coverage) { }

  Int32 x, y, width, coverage;
};

class Spans
{
public:
	mathematics::Rect		m_boundingRect;

	std::vector< Span * > m_spans;

	Span * operator [] ( SizeType i )
	{
		return m_spans[ i ];
	}

	const Span * operator [] ( SizeType i ) const
	{
		return m_spans[ i ];
	}

	SizeType	size() const
	{
		return m_spans.size();
	}

	void push_back( Span * s )
	{
		m_spans.push_back( s );
	}

	bool empty() const
	{
		return m_spans.empty();
	}
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
FreeTypeEnginePtr					FreeTypeEngine::Create( const std::string & fontFilePath, size_t fontSize )
{
	return FreeTypeEnginePtr( new FreeTypeEngine( fontFilePath, fontSize ) );
}

namespace {

void	RasterizeSpans( const Spans & spans, SizeType pitch, char * buffer, UInt8 channel )
{
	auto ymax = (Int32)spans.m_boundingRect.ymax; // FIXME: remove explicit casting
	auto xmin = (Int32)spans.m_boundingRect.xmin;

	for( auto i = spans.size(); i-- > 0;)
	{
		auto s = spans[ i ];
		for ( Int32 w = 0; w < s->width; ++w )
		{
			auto c = 4 * (pitch * (ymax - s->y) + s->x -  xmin +  w);
			buffer[ c + channel ] = (char)s->coverage;
			buffer[ c + 3 ] = (char)s->coverage;
		}
	}
}

// Each time the renderer calls us back we just push another span entry on
// our list.

void
RasterCallback(const int y,
               const int count,
               const FT_Span * const spans,
               void * const user) 
{
  Spans *sptr = (Spans *)user;
  for (int i = 0; i < count; ++i) 
    sptr->push_back(new Span( spans[i].x, y, spans[i].len, spans[i].coverage));
}


// Set up the raster parameters and render the outline.

void
RenderSpans(FT_Library &library,
            FT_Outline * const outline,
            Spans *spans) 
{
  FT_Raster_Params params;
  memset(&params, 0, sizeof(params));
  params.flags = FT_RASTER_FLAG_AA | FT_RASTER_FLAG_DIRECT;
  params.gray_spans = RasterCallback;
  params.user = spans;

  FT_Outline_Render(library, outline, &params);
}

}


FreeTypeEngine::FreeTypeEngine( const std::string & fontFilePath, SizeType fontSize )
	: m_fontSize( fontSize )
	, m_face( nullptr )
	, m_library( nullptr )
	, m_maxHeight( 0 )
	, m_maxWidth( 0 )
	, m_fontFilePath( fontFilePath )
{
	if (FT_Init_FreeType (&m_library))
    {
        std::cerr << "Could not init FreeType library\n" << std::endl;
    }

    if (FT_New_Face (m_library, m_fontFilePath.c_str(), 0, &m_face))
    {
        std::cerr << "Could not open font\n" << std::endl;
    }

	FT_Set_Pixel_Sizes( m_face, (FT_UInt)m_fontSize, (FT_UInt)m_fontSize );
}

// *********************************
//
Glyph*							FreeTypeEngine::RenderGlyph( wchar_t ch, Spans & spans, SizeType outlineWidth )
{
    // Load the glyph we are looking for.
    FT_UInt gindex = FT_Get_Char_Index(m_face, ch);

    if (FT_Load_Glyph(m_face, gindex, FT_LOAD_NO_BITMAP) == 0)
    {
		// Need an outline for this to work.
		if (m_face->glyph->format == FT_GLYPH_FORMAT_OUTLINE)
		{
			// Render the basic glyph to a span list.

			if( outlineWidth == 0 )
				RenderSpans(m_library, &m_face->glyph->outline, &spans);
			else
			{
				        // Set up a stroker.
				FT_Stroker stroker;
				FT_Stroker_New(m_library, &stroker);
				FT_Stroker_Set(stroker,
                       (int)(outlineWidth * 64),
                       FT_STROKER_LINECAP_ROUND,
                       FT_STROKER_LINEJOIN_ROUND,
                       0);

				FT_Glyph glyph;
				if (FT_Get_Glyph(m_face->glyph, &glyph) == 0)
				{
					FT_Glyph_StrokeBorder(&glyph, stroker, 0, 1);
				}

				if (glyph->format == FT_GLYPH_FORMAT_OUTLINE)
				{
				// Render the outline spans to the span list
					FT_Outline *o =
						&reinterpret_cast<FT_OutlineGlyph>(glyph)->outline;
					RenderSpans( m_library, o, &spans);
				}

				// Clean up afterwards.
				FT_Stroker_Done(stroker);
				FT_Done_Glyph(glyph);
			}

			// Now we need to put it all together.
			if (!spans.empty())
			{
				// Figure out what the bounding rect is for both the span lists.
				mathematics::Rect rect(	(float)spans[ 0 ]->x,
							(float)spans[ 0 ]->y,
							(float)spans[ 0 ]->x,
							(float)spans[ 0 ]->y);
				for ( SizeType i = 0; i < spans.size(); ++i )
				{
					auto s = spans[ i ];
					rect.Include(glm::vec2((float)s->x, (float)s->y));
					rect.Include(glm::vec2((float)s->x + s->width - 1, (float)s->y));
				}

				spans.m_boundingRect = rect;

				// This is unused in this test but you would need this to draw
				// more than one glyph.
				Int32 bearingX	= m_face->glyph->metrics.horiBearingX >> 6;
				Int32 bearingY	= m_face->glyph->metrics.horiBearingY >> 6;

				// Get some metrics of our image.
				//int imgWidth = (int)rect.Width(),
				//	imgHeight = (int)rect.Height(),
				//	imgSize = imgWidth * imgHeight;

				auto newGlyph = new Glyph();

				newGlyph->code = ch;
				newGlyph->size = m_fontSize;
				newGlyph->width = (int)rect.Width();
				newGlyph->height = (int)rect.Height();

				if( newGlyph->height > m_maxHeight )
					m_maxHeight = newGlyph->height;

				if( newGlyph->width > m_maxWidth )
					m_maxWidth = newGlyph->width;


				newGlyph->bearingX = bearingX;
				newGlyph->bearingY = bearingY;
				newGlyph->advanceX = m_face->glyph->advance.x >> 6;
				newGlyph->advanceY = m_face->glyph->advance.y >> 6;

				//if( outlineWidth != 0 )
				//{
				//	newGlyph->advanceX = std::max( newGlyph->advanceX, (Int32)newGlyph->width );
				//	newGlyph->advanceY = std::max( newGlyph->advanceY, (Int32)newGlyph->height );
				//}

				return newGlyph;
			}
		}
	}

	return nullptr;
}

// *********************************
//
namespace
{

// ******************************
//
UInt32 RoundUpToPowerOfTwo( UInt32 v )
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;

	return v;
}

// *********************************
//
UInt32 CalculateLevelsNum( UInt32 widht, UInt32 height )
{
	auto wLog2 = std::log( widht ) / std::log( 2 );
	auto hLog2 = std::log( height ) / std::log( 2 );

	return ( UInt32 )std::max( wLog2, hLog2 );
}

// *********************************
//
TextureAssetDescConstPtr GenerateTextAtlasAssetDescriptor( const std::string & fontFileName, UInt32 width, UInt32 height, SizeType fontSize, MipMapFilterType mmFilterType, UInt32 mmLevels )
{
	auto namePrefix = fontFileName + std::to_string( fontSize ) + std::to_string( (UInt32)mmFilterType );

	auto zeroLevelDesc = SingleTextureAssetDesc::Create( namePrefix, width, height, TextureFormat::F_A8R8G8B8, true );

	auto mmSizes = tools::GenerateMipmapsSizes( tools::ImageSize( width, height ) );

	MipMapAssetDescConstPtr mmDesc = nullptr;

	std::vector< SingleTextureAssetDescConstPtr > mipMapsDescs;
	for( SizeType i = 0; i < std::min( mmSizes.size(), (SizeType)mmLevels); ++i )
	{
		mipMapsDescs.push_back( SingleTextureAssetDesc::Create( namePrefix, mmSizes[ i ].width, mmSizes[ i ].height, TextureFormat::F_A8R8G8B8, true ) );
	}

	if( mipMapsDescs.size() > 0 )
	{
		mmDesc = MipMapAssetDesc::Create( mipMapsDescs );
	}

	return TextureAssetDesc::Create( zeroLevelDesc, mmDesc );
}


} // anonymous

// *********************************
//
TextAtlasConstPtr	FreeTypeEngine::CreateAtlas( UInt32 padding, UInt32 outlineWidth, const std::wstring & wcharsSet, bool generateMipMaps )
{
	SizeType							glyphsNum	= wcharsSet.size();
	Int32								spadding	= (Int32)padding;

	std::map< wchar_t, Glyph * >		glyphs;
	std::map< wchar_t, Glyph * >		outlineGlyphs;
	std::map< wchar_t, Spans >			spans;
	std::map< wchar_t, Spans >			outlineSpans;

    for ( auto ch : wcharsSet )
    {
		spans[ ch ] = Spans();
		glyphs[ ch ] = RenderGlyph( ch, spans[ ch ], 0 );
	}

	if( outlineWidth != 0 )
	{
		for ( auto ch : wcharsSet )
		{
			outlineSpans[ ch ] = Spans();
			outlineGlyphs[ ch ] = RenderGlyph( ch, outlineSpans[ ch ], outlineWidth );
		}
	}

	auto atlasSize = (UInt32) std::ceil( sqrt( (float)glyphsNum ) );

    auto maxWidth  = m_maxWidth		+ spadding * 2;
    auto maxHeight = m_maxHeight	+ spadding * 2;

	UInt32 levelsNum = 0;

	if( generateMipMaps )
	{
		levelsNum = CalculateLevelsNum( maxWidth, maxHeight );
		padding += 2 * levelsNum;
		spadding = (Int32)padding;

		maxWidth  = m_maxWidth	+ spadding * 2;
		maxHeight = m_maxHeight	+ spadding * 2;
	}


    auto altlasWidth	= RoundUpToPowerOfTwo( maxWidth	* atlasSize );
    auto altlasHeight	= RoundUpToPowerOfTwo( maxHeight * atlasSize );

	auto atlas = TextAtlas::Create( altlasWidth, altlasHeight, 32, maxWidth, maxHeight );

	auto atlasTextureDesc = GenerateTextAtlasAssetDescriptor( m_fontFilePath, altlasWidth, altlasHeight, m_fontSize, MipMapFilterType::BILINEAR, levelsNum );

	auto atlasTextureRes = TextureCache::GetInstance().Get( atlasTextureDesc );

	if( atlasTextureRes == nullptr )
	{
		for ( auto ch : wcharsSet )
			atlas->SetGlyph( ch, glyphs[ ch ] );

		if( outlineWidth != 0 )
			for ( auto ch : wcharsSet )
				atlas->SetGlyph( ch, outlineGlyphs[ ch ], true );

		char* atlasData = new char[ altlasWidth * altlasHeight * 4 ]; //const_cast< char * >( atlas->GetWritableData()->Get() );// FIXME: Remove const_cast

		memset( atlasData, 0, altlasWidth * altlasHeight * 4 );

		char * currAddress = atlasData;

		for( UInt32 y = 0; y < atlasSize; ++y )
		{
			currAddress = ( atlasData + y * maxHeight *  altlasWidth * 4 );
			currAddress += altlasWidth * padding * 4;

			for( UInt32 x = 0; x < atlasSize; ++x )
			{
				if( y * atlasSize + x < wcharsSet.size() )
				{
					currAddress += padding * 4;

					auto ch = wcharsSet[ y * atlasSize + x ];
					auto & sps = spans[ ch ];
					auto glyph = glyphs[ ch ];

					char * startRasterizeHere = currAddress;

					if( outlineWidth != 0 )
					{
						auto & osps = outlineSpans[ ch ];
						auto oglyph = outlineGlyphs[ ch ];

						startRasterizeHere += ( m_maxHeight - oglyph->height ) * altlasWidth * 4;
						startRasterizeHere += ( m_maxWidth - oglyph->width ) * 4;

						RasterizeSpans( osps, altlasWidth, startRasterizeHere, 1 );

						startRasterizeHere += (SizeType)( osps.m_boundingRect.ymax -  sps.m_boundingRect.ymax ) * altlasWidth * 4;
						startRasterizeHere += (SizeType)( osps.m_boundingRect.xmax -  sps.m_boundingRect.xmax ) * 4;

						RasterizeSpans( sps, altlasWidth, startRasterizeHere, 0 );

						currAddress += ( m_maxWidth + padding ) * 4;

						oglyph->textureY = ( m_maxHeight + 2 * padding ) * y +  2 * padding + ( m_maxHeight	- oglyph->height );
						oglyph->textureX = ( m_maxWidth	+ 2 * padding ) * x +  2 * padding + ( m_maxWidth	- oglyph->width );
						oglyph->padding = padding;


						glyph->textureY = ( m_maxHeight + 2 * padding ) * y +  2 * padding + ( m_maxHeight	- glyph->height ) - (SizeType)( osps.m_boundingRect.ymax -  sps.m_boundingRect.ymax );
						glyph->textureX = ( m_maxWidth	+ 2 * padding ) * x +  2 * padding + ( m_maxWidth	- glyph->width )  - (SizeType)( osps.m_boundingRect.xmax -  sps.m_boundingRect.xmax );
						glyph->padding = padding;
					}
					else
					{
						startRasterizeHere += ( m_maxHeight - glyph->height ) * altlasWidth * 4;
						startRasterizeHere += ( m_maxWidth - glyph->width ) * 4;

						RasterizeSpans( sps, altlasWidth, startRasterizeHere, 0 );

						currAddress += ( m_maxWidth + padding ) * 4;

						glyph->textureY = ( m_maxHeight + 2 * padding ) * y +  2 * padding + ( m_maxHeight	- glyph->height );
						glyph->textureX = ( m_maxWidth	+ 2 * padding ) * x +  2 * padding + ( m_maxWidth	- glyph->width );
						glyph->padding = padding;
					}
				}
			}
		}
	
		auto atlasMC = MemoryChunk::Create( atlasData, altlasWidth * altlasHeight * 4 );
		MipMapAssetConstPtr mipmaps = nullptr;

		if( generateMipMaps )
		{
			tools::Image32 img32 = { atlasMC, altlasWidth, altlasHeight };
			auto mipmap = tools::GenerateMipmaps( img32, levelsNum, image::FilterType::FT_BILINEAR ); // FIXME: filter type is hardcoded.

			std::vector< SingleTextureAssetConstPtr > mipMapsRes;
			for( SizeType i = 0; i < mipmap.size(); ++i )
			{
				auto key = TextureCache::GenKeyForGeneratedMipMap( m_fontFilePath + std::to_string( m_fontSize ), mipmap[ i ].width, mipmap[ i ].height, TextureFormat::F_A8R8G8B8, i, MipMapFilterType::BILINEAR );
				mipMapsRes.push_back( SingleTextureAsset::Create( mipmap[ i ].data, key, mipmap[ i ].width, mipmap[ i ].height, TextureFormat::F_A8R8G8B8 ) );
			}

			mipmaps = MipMapAsset::Create( mipMapsRes );
		}

		auto key = TextureCache::GenKeyForSingleTexture( m_fontFilePath + std::to_string( m_fontSize ), altlasWidth, altlasHeight, TextureFormat::F_A8R8G8B8 );

		auto singleTex = SingleTextureAsset::Create( atlasMC, key, altlasWidth, altlasHeight, TextureFormat::F_A8R8G8B8 );
		atlasTextureRes = TextureAsset::Create( singleTex, mipmaps );

		TextureCache::GetInstance().Add( atlasTextureDesc, atlasTextureRes );
	}
		
	atlas->m_textureAsset = atlasTextureRes;

	image::SaveRAWImage( "testFreeType.raw", atlas->GetWritableData() );

	atlas->m_kerningMap = BuildKerning( m_face, wcharsSet );

	return atlas;
}

// *********************************
//
TextAtlasConstPtr FreeTypeEngine::CreateAtlas( UInt32 padding, const std::wstring & wcharsSet, bool generateMipMaps )
{
	return CreateAtlas( padding, 0, wcharsSet, generateMipMaps );
}

} // bv

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