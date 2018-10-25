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
const Glyph *       TextRepresentation::GetGlyph    ( wchar_t c, bool /*outline*/ ) const
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
    if( glyph->width > m_maxWidth )
        m_maxWidth = glyph->width;
    if( glyph->height > m_maxHeight )
        m_maxHeight = glyph->height;

    m_glyphs.insert( std::make_pair( wch, glyph ) );
}

// ***********************
//
UInt32              TextRepresentation::ComputeMaxWidth() const
{
    UInt32 maxWidth = 0;

    for( auto& glyph : m_glyphs )
    {
		if (glyph.second != NULL)
        if( glyph.second->width > maxWidth )
            maxWidth = glyph.second->width;
    }

    return maxWidth;
}

// ***********************
//
UInt32              TextRepresentation::ComputetMaxHeight() const
{
    UInt32 maxHeight = 0;

    for( auto& glyph : m_glyphs )
    {
		if (glyph.second != NULL)
        if( glyph.second->height > maxHeight )
            maxHeight = glyph.second->height;
    }

    return maxHeight;
}

// ***********************
//
Int32              TextRepresentation::ComputeMaxAdvanceX() const
{
    Int32 maxWidth = 0;

    for( auto& glyph : m_glyphs )
    {
		if (glyph.second != NULL)
        if( glyph.second->advanceX > maxWidth )
            maxWidth = glyph.second->advanceX;
    }

    return maxWidth;
}

// ***********************
//
Int32              TextRepresentation::ComputetMaxAdvanceY() const
{
    Int32 maxWidth = 0;

    for( auto& glyph : m_glyphs )
    {
		if(glyph.second!=NULL)
        if( glyph.second->advanceY > maxWidth )
            maxWidth = glyph.second->advanceY;
    }

    return maxWidth;
}

} //bv


