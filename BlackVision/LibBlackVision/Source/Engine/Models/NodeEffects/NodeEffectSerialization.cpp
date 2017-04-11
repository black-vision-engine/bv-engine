#include "stdafx.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"

#include "Memory/MemoryLeaks.h"


namespace bv { 
    
namespace SerializationHelper {

// FIXME: nrl - refactor effects
std::pair< NodeEffectType, const char* > ne2s[] = {
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
    , std::make_pair( NodeEffectType::NET_TOTAL, "" ) };

template<>
std::string T2String< NodeEffectType >( const NodeEffectType& t )
{
    return SerializationHelper::Enum2String< NodeEffectType >( ne2s, t );
}

template<>
NodeEffectType String2T( const std::string & s, const NodeEffectType& defaultType )
{
    auto effectType = String2Enum( ne2s, s );
    if( effectType == NodeEffectType::NET_TOTAL )
        return defaultType;
    return effectType;
}

} // SerializationHelper


namespace model {



} }
