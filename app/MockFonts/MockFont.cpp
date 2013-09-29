#include "MockFont.h"

namespace bv {

TextAtlas::TextAtlas( unsigned int w, unsigned int h, unsigned int bitsPrePixel )
    : m_width( w )
    , m_height( h )
    , m_bitsPerPixel( bitsPrePixel )
{
    m_data = new char[ w * h * bitsPrePixel / 8 ];
}

Text::Text( const std::wstring& text )
{

}

}
