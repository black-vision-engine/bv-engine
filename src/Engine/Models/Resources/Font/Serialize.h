#include "boost/archive/text_oarchive.hpp"
#include "boost/archive/text_iarchive.hpp"
#include "boost/serialization/map.hpp"

#include "Text.h"

namespace boost { namespace serialization {

template< class Archive >
void serialize( Archive & ar, bv::model::GlyphCoords& glyphCoords, const unsigned int version )
{
    ar & glyphCoords.textureX;
    ar & glyphCoords.textureY;
    ar & glyphCoords.width;
    ar & glyphCoords.height;
    ar & glyphCoords.bearingX;
    ar & glyphCoords.bearingY;
    ar & glyphCoords.glyphX;
    ar & glyphCoords.glyphY;
    ar & glyphCoords.glyphWidth;
    ar & glyphCoords.glyphHeight;
}

template< class Archive >
void serialize( Archive & ar, bv::model::TextAtlas& textAtlas, const unsigned int version )
{
    ar & textAtlas.m_width;
    ar & textAtlas.m_height;
    ar & textAtlas.m_bitsPerPixel;
    ar & textAtlas.m_glyphsPositions;
    ar & textAtlas.m_glyphWidth;
    ar & textAtlas.m_glyphHeight;
}

} // serialization
} // boost