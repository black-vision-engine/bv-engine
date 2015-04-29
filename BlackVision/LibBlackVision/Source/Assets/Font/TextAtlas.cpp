#include "Text.h"

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
#include "IO/FileIO.h"
#include "LibImage.h"
#include "Assets/Texture/TextureLoader.h"
#include "Assets/Font/Engines/FreeTypeEngine.h"
#include "Tools/MipMapBuilder/Source/MipMapBuilder.h"

namespace bv { 

// *********************************
//
TextAtlas::TextAtlas()
    : m_glyphWidth( 0 )
    , m_glyphHeight( 0 )
{}

// *********************************
//
TextAtlas::TextAtlas( UInt32 w, UInt32 h, UInt32 bitsPerPixel, UInt32 gw, UInt32 gh )
    : m_glyphWidth( gw )
    , m_glyphHeight( gh )
	, m_textureAsset( nullptr )
	, m_blurSize( 0 )
{
    auto size   = w * h * bitsPerPixel / 8;
    auto data  = std::make_shared< MemoryChunk >( new char[ size ], size );

    TextureFormat tf = TextureFormat::F_TOTAL;
	if( bitsPerPixel == 8 )
		tf = TextureFormat::F_A8;
	else if ( bitsPerPixel == 32 )
		tf = TextureFormat::F_A8R8G8B8;
	else
		assert( bitsPerPixel == 0 ); // For deserializing of TextAtlas 
}

// *********************************
//
TextAtlasPtr TextAtlas::Create( UInt32 w, UInt32 h, UInt32 bitsPrePixel, UInt32 gw, UInt32 gh )
{
    return std::make_shared< TextAtlas >(w, h, bitsPrePixel, gw, gh);
}

// *********************************
//
MemoryChunkConstPtr      TextAtlas::GetData         () const
{
	// FIXME: How about mimpmaps. This function should return TextureAssetConstPtr not MemoryChunkConstPtr
	return m_textureAsset->GetOriginal()->GetData();
}

// *********************************
//
MemoryChunkPtr			TextAtlas::GetWritableData ()
{
	// FIXME: How about mimpmaps. This function should return TextureAssetConstPtr not MemoryChunkConstPtr
	return std::const_pointer_cast< MemoryChunk >( m_textureAsset->GetOriginal()->GetData() );
}

// *********************************
//
SizeType				TextAtlas::GetSizeInBytes  () const
{
	// FIXME: How about mimpmaps. This function should return TextureAssetConstPtr not MemoryChunkConstPtr
	return m_textureAsset->GetOriginal()->GetData()->Size();
}

// *********************************
//
void                    TextAtlas::SetGlyph			( wchar_t wch, const Glyph * glyph, bool outline )
{
	if( outline )
		m_outlineGlyphs.insert(std::make_pair( wch, glyph ) );
	else
		m_glyphs.insert(std::make_pair( wch, glyph ) );
}

// *********************************
//
UInt32					TextAtlas::GetBitsPerPixel () const
{
	assert( m_textureAsset->GetOriginal()->GetFormat() == TextureFormat::F_A8R8G8B8 );
	return 32;
}

// *********************************
//
UInt32					TextAtlas::GetWidth        () const
{
    return m_textureAsset->GetOriginal()->GetWidth();
}

// *********************************
//
UInt32					TextAtlas::GetHeight       () const
{
    return m_textureAsset->GetOriginal()->GetHeight();
}

// *********************************
//
const Glyph *			TextAtlas::GetGlyph			( wchar_t c, bool outline ) const
{
	if(! outline )
	{
		auto it = m_glyphs.find(c);

		if( it != m_glyphs.end() )
			return it->second;
	}
	else
	{
		auto it = m_outlineGlyphs.find(c);

		if( it != m_outlineGlyphs.end() )
			return it->second;
	}


	auto it = m_glyphs.find('_');

	if( it != m_glyphs.end() )
			return it->second;
	else
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
UInt32					TextAtlas::GetGlyphX       ( wchar_t c ) const
{
    return GetGlyph( c )->textureX;
}

// *********************************
//
UInt32					TextAtlas::GetGlyphY       ( wchar_t c ) const
{
    return GetGlyph( c )->textureY;
}

// *********************************
//
UInt32					TextAtlas::GetGlyphWidth   ( wchar_t c ) const
{
    return GetGlyph( c )->width;
}

// *********************************
//
UInt32					TextAtlas::GetGlyphHeight  ( wchar_t c ) const
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
TextureAssetConstPtr	TextAtlas::GetAsset() const
{
	return m_textureAsset;
}


// *********************************
//
TextureAssetDescConstPtr TextAtlas::GenerateTextAtlasAssetDescriptor( const std::string & fontFileName, UInt32 width, UInt32 height, SizeType fontSize, SizeType blurSize, MipMapFilterType mmFilterType, SizeType mmLevels )
{
	auto namePrefix = fontFileName + std::to_string( fontSize ) + std::to_string( blurSize );

	namePrefix += std::to_string( (UInt32)mmFilterType );

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
		mmDesc = MipMapAssetDesc::Create( mipMapsDescs, MipMapFilterType::BILINEAR );
	}

	return TextureAssetDesc::Create( zeroLevelDesc, mmDesc );
}

} // bv