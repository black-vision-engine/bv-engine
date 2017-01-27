#pragma once

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"

#include "InterpolatorBasicTypes.h"

#include <utility>
#include <string>

namespace bv
{

namespace SerializationHelper
{

struct MappingHelper
{
	static const std::pair< CurveType, const char* > * ct2s;
	static const std::pair< WrapMethod, const char* > * wm2s;
};

template<>
std::string T2String< WrapMethod >( const WrapMethod& wm );

template<>
WrapMethod String2T( const std::string & s, const WrapMethod& default );

template<> 
std::string T2String< CurveType >( const CurveType& ct );

template<> 
CurveType String2T( const std::string & s, const CurveType& default );

}

} // bv