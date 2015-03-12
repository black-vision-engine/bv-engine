#include "boost/archive/text_oarchive.hpp"
#include "boost/archive/text_iarchive.hpp"
#include "boost/serialization/map.hpp"

#include "Text.h"
#include "TextAtlas.h"
#include "Glyph.h"
#include "Engine/Models/Resources/Texture/TextureLoader.h"
#include <cassert>

namespace boost { namespace serialization {

template< class Archive >
void serialize( Archive & ar, bv::model::Glyph & glyph, const unsigned int version )
{
    { version; } // FIXME: suppress unused warning
    ar & glyph.textureX;
    ar & glyph.textureY;
    ar & glyph.width;
    ar & glyph.height;
    ar & glyph.bearingX;
    ar & glyph.bearingY;
    ar & glyph.advanceX;
    ar & glyph.advanceY;
	ar & glyph.padding;
//    ar & glyphCoords.glyphX;
//    ar & glyphCoords.glyphY;
//    ar & glyphCoords.glyphWidth;
//    ar & glyphCoords.glyphHeight;
}

template< >
void serialize< boost::archive::text_iarchive >( boost::archive::text_iarchive & ar, bv::model::TextAtlas & textAtlas, const unsigned int version )
{
    { version; textAtlas; } // FIXME: suppress unused warning

    ar >> textAtlas.m_glyphs;
	ar >> textAtlas.m_outlineGlyphs;
    ar >> textAtlas.m_glyphWidth;
    ar >> textAtlas.m_glyphHeight;
    ar >> textAtlas.m_kerningMap;
	ar >> textAtlas.m_blurSize;
}

template< >
void serialize< boost::archive::text_oarchive >( boost::archive::text_oarchive & ar, bv::model::TextAtlas & textAtlas, const unsigned int version )
{
    { version; } // FIXME: suppress unused warning

    ar & textAtlas.m_glyphs;
	ar & textAtlas.m_outlineGlyphs;
    ar << textAtlas.m_glyphWidth;
    ar << textAtlas.m_glyphHeight;
    ar & textAtlas.m_kerningMap;
	ar & textAtlas.m_blurSize;
}

} // serialization
} // boost