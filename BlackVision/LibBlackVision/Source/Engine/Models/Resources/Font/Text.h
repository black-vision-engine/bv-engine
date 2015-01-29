#pragma once

#include "Engine/Models/Resources/IResource.h"
#include "Engine/Models/Resources/Font/IFontEngine.h"

#include <string>
#include <map>

namespace bv { namespace model {

class Text;
struct Glyph;


class TextAtlas
{
public: // Only for non intrusive serialization. Should be private

    ResourceHandlePtr									m_textureHandle;

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

    static TextAtlas*       Crate           ( SizeType w, SizeType h, SizeType bitsPrePixel, SizeType gw, SizeType gh );

	ResourceHandlePtr		GetResourceHandle() const;

    friend class Text;

    // serialization
    void                    Save( std::ostream& out ) const;
    void                    Load( std::istream& in );
};

class Text
{
private:
    std::wstring                        m_supportedCharsSet;
    std::string                         m_fontFile;
    const TextAtlas *                   m_atlas;
    SizeType							m_fontSize;
    SizeType							m_blurSize;
	SizeType							m_outlineWidth; // if > 0 text is outlined
	IFontEnginePtr						m_fontEngine;

    void                                BuildAtlas			();
	void                                BuildAtlasOutlined	();
    const TextAtlas *                   LoadFromCache		();

public:

    const TextAtlas *                   GetAtlas() const { return m_atlas; }

    explicit                            Text( const std::wstring& supportedCharsSet
											, const std::string& fontFile
											, SizeType fontSize
											, SizeType blurSize
											, SizeType outlineSize );
};

} // model
} // bv