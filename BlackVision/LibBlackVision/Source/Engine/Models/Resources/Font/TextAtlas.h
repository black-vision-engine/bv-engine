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

    void                    SetGlyph		( wchar_t wch, const Glyph * glyph, bool outline = false );

public:

    SizeType	            m_glyphWidth;
    SizeType	            m_glyphHeight;


    SizeType				GetBitsPerPixel () const;

    SizeType				GetWidth        () const;
    SizeType				GetHeight       () const;
    
    SizeType				GetGlyphX       ( wchar_t c ) const;
    SizeType				GetGlyphY       ( wchar_t c ) const;

    SizeType				GetGlyphWidth   ( wchar_t c ) const;
    SizeType				GetGlyphHeight  ( wchar_t c ) const;

    const Glyph *			GetGlyph		( wchar_t c, bool outline = false ) const;

    Float32                 GetKerning      ( wchar_t c0, wchar_t c1 ) const;

    MemoryChunkConstPtr     GetData         () const;
    MemoryChunkConstPtr     GetWritableData ();
    SizeType				GetSizeInBytes  () const;

    TextAtlas();
    TextAtlas( SizeType w, SizeType h, SizeType bitsPrePixel, SizeType gw, SizeType gh );

    static TextAtlasPtr		Create          ( SizeType w, SizeType h, SizeType bitsPrePixel, SizeType gw, SizeType gh );

	TextureResourceConstPtr	GetResourceHandle() const;

    friend class Text;

    // serialization
    void                    Save( std::ostream& out ) const;
    void                    Load( std::istream& in );
};

} // model
} // bv
