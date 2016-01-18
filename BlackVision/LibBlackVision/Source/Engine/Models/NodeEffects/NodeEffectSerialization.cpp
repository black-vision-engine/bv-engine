#include "ModelNodeEffectAlphaMask.h"
#include "ModelNodeEffectDefault.h"
#include "ModelNodeEffectNodeMask.h"
#include "ModelNodeEffectWireframe.h"

#include "Serialization/SerializationHelper.h"
//#include "Serialization/SerializationHelper.inl"

namespace bv { 
    
namespace SerializationHelper {

std::pair< bv::NodeEffectType, const char* > ne2s[] = {
    std::make_pair( bv::NodeEffectType::NET_ALPHA_MASK, "alpha mask" )
    , std::make_pair( bv::NodeEffectType::NET_DEFAULT, "default" )
    , std::make_pair( bv::NodeEffectType::NET_NODE_MASK, "node mask" )
    , std::make_pair( bv::NodeEffectType::NET_WIREFRAME, "wireframe" )
    , std::make_pair( bv::NodeEffectType::NET_SHADOW, "shadow" )
    , std::make_pair( bv::NodeEffectType::NET_LIGHT_SCATTERING, "light scattering" )
    , std::make_pair( bv::NodeEffectType::NET_TOTAL, "" ) };

template<>
std::string T2String< bv::NodeEffectType >( const bv::NodeEffectType& t )
{
    return SerializationHelper::Enum2String< bv::NodeEffectType >( ne2s, t );
}

template<>
bv::NodeEffectType String2T( const std::string & s, const bv::NodeEffectType& defaultType )
{
    auto effectType = String2T( ne2s, s );
    if( effectType == NodeEffectType::NET_TOTAL )
        return defaultType;
    return effectType;
}

} // SerializationHelper


namespace model {



} }
