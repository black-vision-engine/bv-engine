#include "stdafx.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"

#include "Memory/MemoryLeaks.h"


namespace bv {     
namespace Convert
{

// FIXME: nrl - refactor effects
// ***********************
//
std::pair< NodeEffectType, const char* > ne2s[] =
{
    std::make_pair( NodeEffectType::NET_ALPHA_MASK, "alpha mask" )
    , std::make_pair( NodeEffectType::NET_BLUR, "blur" )
    , std::make_pair( NodeEffectType::NET_DEFAULT, "default" )
    , std::make_pair( NodeEffectType::NET_LIGHT_SCATTERING, "light scattering" )
    , std::make_pair( NodeEffectType::NET_MIX_CHANNELS, "mix channels" )
    , std::make_pair( NodeEffectType::NET_NODE_MASK, "node mask" )
    , std::make_pair( NodeEffectType::NET_WIREFRAME, "wireframe" )
    , std::make_pair( NodeEffectType::NET_SHADOW, "shadow" )
	, std::make_pair( NodeEffectType::NET_GLOW, "glow" )
	, std::make_pair( NodeEffectType::NET_SOFT_MASK, "soft mask" )
//    , std::make_pair( NodeEffectType::NET_IMAGE_MASK, "image mask" )
    , std::make_pair( NodeEffectType::NET_Z_SORT, "z sort" )
    , std::make_pair( NodeEffectType::NET_COLOR_CORRECTION, "color_correction" )
    , std::make_pair( NodeEffectType::NET_TOTAL, "" )
};

IMPLEMENT_ENUM_SERIALIZATION( NodeEffectType, ne2s )


} // Convert


namespace model {



} }
