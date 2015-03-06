#pragma once

#include "Engine/Models/Resources/Texture/TextureResource.h"
#include "System/BasicTypes.h"

#include <string>
#include <map>

namespace bv { namespace model {

class Text;
struct Glyph;

class TextAtlas;
DEFINE_CONST_PTR_TYPE(TextAtlas)
DEFINE_PTR_TYPE(TextAtlas)

class TextAtlas
{
public: // Only for non intrusive serialization. Should be private

	TextureResourceConstPtr								m_textureResource;

	std::map< wchar_t, const Glyph * >                  m_outlineGlyphs;
    std::map< wchar_t, const Glyph * >                  m_glyphs;
    std::map< std::pair< wchar_t, wchar_t >, float >	m_kerningMap; 
	UInt32												m_blurSize;

    void                    SetGlyph		( wchar_t wch, const Glyph * glyph, bool outline = false );

public:

    UInt32					m_glyphWidth;
    UInt32					m_glyphHeight;


    UInt32					GetBitsPerPixel () const;

    UInt32					GetWidth        () const;
    UInt32					GetHeight       () const;
    
    UInt32					GetGlyphX       ( wchar_t c ) const;
    UInt32					GetGlyphY       ( wchar_t c ) const;

    UInt32					GetGlyphWidth   ( wchar_t c ) const;
    UInt32					GetGlyphHeight  ( wchar_t c ) const;

    const Glyph *			GetGlyph		( wchar_t c, bool outline = false ) const;

    Float32                 GetKerning      ( wchar_t c0, wchar_t c1 ) const;

    MemoryChunkConstPtr     GetData         () const;
    MemoryChunkPtr			GetWritableData ();
    SizeType				GetSizeInBytes  () const;

    TextAtlas();
    TextAtlas( UInt32 w, UInt32 h, UInt32 bitsPrePixel, UInt32 gw, UInt32 gh );

    static TextAtlasPtr		Create          ( UInt32 w, UInt32 h, UInt32 bitsPrePixel, UInt32 gw, UInt32 gh );

	TextureResourceConstPtr	GetResourceHandle() const;

    friend class Text;

    // serialization
    void                    Save( std::ostream& out ) const;
    void                    Load( std::istream& in );
};

} // model
} // bv
