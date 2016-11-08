#include "stdafx.h"
#include "TextRepresentation.h"

#include "Assets/Font/Glyph.h"


namespace bv {



// ***********************
//
TextRepresentation::~TextRepresentation()
{
    for( auto it = m_glyphs.cbegin(); it != m_glyphs.cend(); )
    {
        delete it->second;
        m_glyphs.erase( it++ );
    }

}

// *********************************
//
Float32             TextRepresentation::GetKerning      ( wchar_t c0, wchar_t c1 ) const
{
    auto it = m_kerningMap.find( std::make_pair( c0, c1 ) );

    if( it != m_kerningMap.end() )
        return it->second;
    else
        return 0.f;
}

// ***********************
//
const Glyph *       TextRepresentation::GetGlyph    ( wchar_t c ) const
{
    auto it = m_glyphs.find( c );

    if( it != m_glyphs.end() )
        return it->second;

    // Glyph not found.
    auto iter = m_glyphs.find( '_' );

    if( iter != m_glyphs.end() )
        return iter->second;
    else
        return nullptr;
}

// *********************************
//
void                TextRepresentation::SetGlyph    ( wchar_t wch, const Glyph * glyph )
{
    m_glyphs.insert( std::make_pair( wch, glyph ) );
}

} //bv


