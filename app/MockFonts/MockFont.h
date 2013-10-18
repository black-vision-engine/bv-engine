#pragma once

#include "Glyph.h"

#include <string>
#include <hash_map>

namespace bv {

class TextAtlas
{
private:

    char*                   m_data;
    unsigned int            m_width;
    unsigned int            m_height;
    unsigned int            m_glyphWidth;
    unsigned int            m_glyphHeight;
    unsigned int            m_bitsPerPixel;

public:

    unsigned int            GetWidth        () const    { return m_width; }
    unsigned int            GetHeight       () const    { return m_height; }
    unsigned int            GetGlyphWidth   () const    { return m_glyphWidth; }
    unsigned int            GetGlyphHeight  () const    { return m_glyphHeight; }
    const char*             GetData         () const    { return m_data; }
    char*                   GetWritableData ()          { return m_data; }

    TextAtlas( unsigned int w, unsigned int h, unsigned int bitsPrePixel, unsigned int gw, unsigned int gh );

    static TextAtlas*       Crate           ( unsigned int w, unsigned int h, unsigned int bitsPrePixel, unsigned int gw, unsigned int gh );
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

}
