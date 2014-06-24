#include "boost/archive/text_oarchive.hpp"
#include "boost/archive/text_iarchive.hpp"
#include "boost/serialization/map.hpp"

#include "IResource.h"
#include "TextureLoader.h"

namespace boost { namespace serialization {

template< class Archive >
void serialize( Archive & ar, bv::model::ResourceHandle& handle, const unsigned int version )
{
    ar & handle.m_extra;
    ar & handle.m_size;
}

template< class Archive >
void serialize( Archive & ar, bv::model::IResourceExtraData& extraData, const unsigned int version )
{
    assert( extraData.GetResourceExtraKind() == bv::model::ResourceExtraKind::RE_TEXTURE );
    auto textureExtraDat = static_cast< bv::model::TextureExtraData& >( extraData );
    ar & textureExtraData.m_bitsPerPixel;
    ar & textureExtraData.m_format;
    ar & textureExtraData.m_height;
    ar & textureExtraData.m_witdth;
    ar & textureExtraData.m_type;
    ar & textureExtraData.m_kind;
}

} // serialization
} // boost