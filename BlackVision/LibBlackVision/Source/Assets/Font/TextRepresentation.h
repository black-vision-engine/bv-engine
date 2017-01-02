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



class TextRepresentation
{
public: // Only for non intrusive serialization. Should be protected

    std::map< wchar_t, const Glyph * >                  m_glyphs;
    std::map< std::pair< wchar_t, wchar_t >, float >	m_kerningMap;

public:
    explicit    TextRepresentation  () {}
    virtual     ~TextRepresentation ();

public:

    Float32                 GetKerning      ( wchar_t c0, wchar_t c1 ) const;
    virtual const Glyph *   GetGlyph        ( wchar_t c, bool outline = false ) const;


private:

    void                    SetGlyph		( wchar_t wch, const Glyph * glyph );
};

} //bv

