#include "stdafx.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"

#include "Memory/MemoryLeaks.h"


namespace bv { 
    
namespace SerializationHelper {

// FIXME: nrl - refactor effects
std::pair< nrl::NNodeEffectType, const char* > ne2s[] = {
    std::make_pair( nrl::NNodeEffectType::NNET_ALPHA_MASK, "alpha mask" )
    , std::make_pair( nrl::NNodeEffectType::NNET_BLUR, "blur" )
    , std::make_pair( nrl::NNodeEffectType::NNET_DEFAULT, "default" )
    , std::make_pair( nrl::NNodeEffectType::NNET_LIGHT_SCATTERING, "light scattering" )
    , std::make_pair( nrl::NNodeEffectType::NNET_MIX_CHANNELS, "mix channels" )
    , std::make_pair( nrl::NNodeEffectType::NNET_NODE_MASK, "node mask" )
    , std::make_pair( nrl::NNodeEffectType::NNET_WIREFRAME, "wireframe" )
    , std::make_pair( nrl::NNodeEffectType::NNET_SHADOW, "shadow" )
	, std::make_pair( nrl::NNodeEffectType::NNET_GLOW, "glow" )
	, std::make_pair( nrl::NNodeEffectType::NNET_SOFT_MASK, "soft mask" )
//    , std::make_pair( nrl::NNodeEffectType::NNET_IMAGE_MASK, "image mask" )
    , std::make_pair( nrl::NNodeEffectType::NNET_Z_SORT, "z sort" )
    , std::make_pair( nrl::NNodeEffectType::NNET_TOTAL, "" ) };

template<>
std::string T2String< nrl::NNodeEffectType >( const nrl::NNodeEffectType& t )
{
    return SerializationHelper::Enum2String< nrl::NNodeEffectType >( ne2s, t );
}

template<>
nrl::NNodeEffectType String2T( const std::string & s, const nrl::NNodeEffectType& defaultType )
{
    auto effectType = String2Enum( ne2s, s );
    if( effectType == nrl::NNodeEffectType::NNET_TOTAL )
        return defaultType;
    return effectType;
}

} // SerializationHelper


namespace model {



} }
