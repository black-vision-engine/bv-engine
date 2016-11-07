#include "stdafx.h"

#include "Text.h"

#include "Serialize.h"
#include "MipMapBuilder.h"


namespace bv { 

// *********************************
//
TextAtlas::TextAtlas()
    : m_glyphWidth( 0 )
    , m_glyphHeight( 0 )
{}

// *********************************
//
TextAtlas::~TextAtlas()
{
    for( auto it = m_glyphs.cbegin(); it != m_glyphs.cend(); )
    {
        delete it->second;
        m_glyphs.erase( it++ );
    }

    for( auto it = m_outlineGlyphs.cbegin(); it != m_outlineGlyphs.cend(); )
    {
        delete it->second;
        m_outlineGlyphs.erase( it++ );
    }
}

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
TextureAssetDescConstPtr TextAtlas::GenerateTextAtlasAssetDescriptor( const std::string & fontFileName, UInt32 width, UInt32 height, SizeType fontSize, SizeType blurSize, SizeType outlineSize, UInt32 mmLevels, const std::wstring & charSet )
{
    auto namePrefix = fontFileName + "W" + std::to_string( width ) + "H" + std::to_string( height ) + "FS" + std::to_string( fontSize ) + "BS" + std::to_string( blurSize ) + "OS" + std::to_string( outlineSize ) + "CS" + std::string( charSet.begin(), charSet.end() );

	auto zeroLevelDesc = GeneratedSingleTextureAssetDesc::Create( namePrefix, width, height, TextureFormat::F_A8R8G8B8, true );

	MipMapAssetDescConstPtr mmDesc = nullptr;

	if ( mmLevels > 0 )
	{
		auto mmSizes = tools::GenerateMipmapsSizes( tools::ImageSize( width, height ) );

		mmDesc = MipMapAssetDesc::Create( MipMapFilterType::BILINEAR, zeroLevelDesc, ( Int32 )mmLevels );
	}

	return TextureAssetDesc::Create( zeroLevelDesc, mmDesc );
}

} // bv