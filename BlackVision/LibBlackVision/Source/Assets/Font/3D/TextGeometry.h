#pragma once


#include "Assets/Font/TextRepresentation.h"
#include "FTContour.h"

#include <map>


namespace bv
{

class Text;
struct Glyph;

class TextGeometry;
DEFINE_PTR_TYPE( TextGeometry )
DEFINE_CONST_PTR_TYPE( TextGeometry )

DEFINE_UPTR_TYPE( FTContour );



class TextGeometry : public TextRepresentation
{
private:

    mutable std::map< wchar_t, ContoursList >   m_contours;
    IFontEnginePtr                              m_fontEngine;

public:

    explicit        TextGeometry    ( IFontEnginePtr engine )
        : m_fontEngine( engine )
    {}

    const ContoursList &        CreateCharacter     ( wchar_t ch, float size ) const;
};


}   // bv
