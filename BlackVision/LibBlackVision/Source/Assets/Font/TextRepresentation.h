#pragma once

#include "CoreDEF.h"

#include <map>


namespace bv {



class Text;
struct Glyph;

class TextRepresentation;
class IFontEngine;
DEFINE_PTR_TYPE( TextRepresentation )
DEFINE_CONST_PTR_TYPE( TextRepresentation )
DEFINE_PTR_TYPE( IFontEngine )
DEFINE_CONST_PTR_TYPE( IFontEngine )


class TextRepresentation
{
public: // Only for non intrusive serialization. Should be protected

    std::map< wchar_t, const Glyph * >                  m_glyphs;
    std::map< std::pair< wchar_t, wchar_t >, float >	m_kerningMap;

    UInt32          m_maxWidth;
    UInt32          m_maxHeight;

public:
    explicit    TextRepresentation  () : m_maxHeight( 0 ), m_maxWidth( 0 ) {}
    virtual     ~TextRepresentation ();

public:

    Float32                 GetKerning      ( wchar_t c0, wchar_t c1 ) const;
    virtual const Glyph *   GetGlyph        ( wchar_t c, bool outline = false ) const;

    UInt32                  ComputeMaxWidth     () const;
    UInt32                  ComputetMaxHeight   () const;

    Int32                   ComputeMaxAdvanceX   () const;
    Int32                   ComputetMaxAdvanceY  () const;

private:

    void                    SetGlyph		( wchar_t wch, const Glyph * glyph );
};

} //bv

