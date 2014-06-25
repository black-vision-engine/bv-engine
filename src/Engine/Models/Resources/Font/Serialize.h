#include "boost/archive/text_oarchive.hpp"
#include "boost/archive/text_iarchive.hpp"
#include "boost/serialization/map.hpp"

#include "Text.h"
#include "Engine/Models/Resources/TextureLoader.h"

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

template< >
void serialize< boost::archive::text_iarchive >( boost::archive::text_iarchive & ar, bv::model::TextAtlas& textAtlas, const unsigned int version )
{
    unsigned int size;
    ar >> size;

    auto textureExtra = new bv::model::TextureExtraData();

    ar >> textureExtra->m_bitsPerPixel;
    ar >> textureExtra->m_format;
    ar >> textureExtra->m_height;
    ar >> textureExtra->m_width;
    ar >> textureExtra->m_type;

    textAtlas.m_textureHandle = bv::model::ResourceHandlePtr( new bv::model::ResourceHandle( nullptr, size, textureExtra ) );

    ar >> textAtlas.m_glyphsPositions;
    ar >> textAtlas.m_glyphWidth;
    ar >> textAtlas.m_glyphHeight;
}

template< >
void serialize< boost::archive::text_oarchive >( boost::archive::text_oarchive & ar, bv::model::TextAtlas& textAtlas, const unsigned int version )
{
    ar << textAtlas.m_textureHandle->m_size;
    
    assert( textAtlas.m_textureHandle->GetExtra()->GetResourceExtraKind() == bv::model::ResourceExtraKind::RE_TEXTURE );
    auto textureExtra = static_cast< const bv::model::TextureExtraData * >( textAtlas.m_textureHandle->GetExtra() );

    ar << textureExtra->m_bitsPerPixel;
    ar << textureExtra->m_format;
    ar << textureExtra->m_height;
    ar << textureExtra->m_width;
    ar << textureExtra->m_type;

    ar & textAtlas.m_glyphsPositions;
    ar << textAtlas.m_glyphWidth;
    ar << textAtlas.m_glyphHeight;
}

} // serialization
} // boost