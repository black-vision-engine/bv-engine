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
    unsigned int            m_bitsPerPixel;

public:

    unsigned int            GetWidth        () const    { return m_width; }
    unsigned int            GetHeight       () const    { return m_height; }
    const char*             GetData         () const    { return m_data; }
    char*                   GetWitableDate  ()          { return m_data; }

    TextAtlas( unsigned int w, unsigned int h, unsigned int bitsPrePixel );

    static TextAtlas*       Crate           ( unsigned int w, unsigned int h, unsigned int bitsPrePixel );
};

class Text
{
private:
    std::wstring                        m_text;
    const TextAtlas*                    m_atlas;
    std::hash_map< wchar_t, Glyph * >   m_glyphs;

public:

    explicit                            Text( const std::wstring& text );
};

class MockFontEngine
{
private:

    std::hash_map< Text*, Text* >           m_textMap;

public:
    const Text*                  AddText( const std::wstring& text );
};

}
