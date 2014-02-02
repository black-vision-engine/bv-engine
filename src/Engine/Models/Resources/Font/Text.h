#pragma once

#include "Glyph.h"

#include <string>
#include <hash_map>

namespace bv { namespace model {

class Text;

struct GlyphCoords
{
    unsigned int            textureX;
    unsigned int            textureY;

    unsigned int            width;
    unsigned int            height;

    unsigned int            bearingX;
    unsigned int            bearingY;

    GlyphCoords(unsigned int tX,
                unsigned int tY,
                unsigned int w,
                unsigned int h,
                unsigned int bX,
                unsigned int bY)
                : textureX( tX )
                , textureY( tY )
                , width( w )
                , height( h )
                , bearingX( bX )
                , bearingY( bX )

    {}
};

class TextAtlas
{
private:

    char*                   m_data;
    unsigned int            m_width;
    unsigned int            m_height;
    unsigned int            m_bitsPerPixel;


    std::hash_map< wchar_t, GlyphCoords >   m_glyphsPositions;

    void                    SetGlyphCoords  ( wchar_t wch, const GlyphCoords& coords );

public:

    unsigned int            m_glyphWidth;
    unsigned int            m_glyphHeight;


    unsigned int            GetBitsPerPixel () const;

    unsigned int            GetWidth        () const;
    unsigned int            GetHeight       () const;
    
    unsigned int            GetGlyphX       ( wchar_t c ) const;
    unsigned int            GetGlyphY       ( wchar_t c ) const;

    unsigned int            GetGlyphWidth   ( wchar_t c ) const;
    unsigned int            GetGlyphHeight  ( wchar_t c ) const;

    const GlyphCoords&      GetGlyphCoords  ( wchar_t c ) const;

    const char*             GetData         () const;
    char*                   GetWritableData ();
    unsigned int            GetSizeInBytes  () const;

    TextAtlas( unsigned int w, unsigned int h, unsigned int bitsPrePixel, unsigned int gw, unsigned int gh );

    static TextAtlas*       Crate           ( unsigned int w, unsigned int h, unsigned int bitsPrePixel, unsigned int gw, unsigned int gh );

    friend class Text;
};

class Text
{
private:
    std::wstring                        m_text;
    std::string                         m_fontFile;
    TextAtlas*                          m_atlas;
    unsigned int                        m_fontSize;
    std::hash_map< wchar_t, Glyph * >   m_glyphs;

    void                                BuildAtlas();

public:

    const Glyph *                       GetGlyph( wchar_t wch ) const { return m_glyphs.find( wch )->second; } 
    const std::wstring&                 GetText () const { return m_text; }
    const TextAtlas*                    GetAtlas() const { return m_atlas; }

    explicit                            Text( const std::wstring& text, const std::string& fontFile, unsigned int fontSize );
};

class MockFontEngine
{
private:

    std::hash_map< Text*, Text* >           m_textMap;

public:
    const Text*                             AddText( const std::wstring& text, const std::string& fontFile );
};

} // model
} // bv
