#include "stdafx.h"

#include "CompositeInterpolatorSerializationHelper.h"

namespace bv
{
namespace SerializationHelper
{

static const std::pair< CurveType, const char* > ct2s_data [] =
{ std::make_pair( CurveType::CT_BEZIER, "bezier" )
, std::make_pair( CurveType::CT_COSINE_LIKE, "cosine" )
, std::make_pair( CurveType::CT_LINEAR, "linear" )
, std::make_pair( CurveType::CT_POINT, "point" )
, std::make_pair( CurveType::CT_CUBIC_IN, "cubic_in" )
, std::make_pair( CurveType::CT_CUBIC_OUT, "cubic_out" )
, std::make_pair( CurveType::CT_ELASTIC_IN, "elastic_in" )
, std::make_pair( CurveType::CT_ELASTIC_OUT, "elastic_out" )
, std::make_pair( CurveType::CT_ELASTIC_IN_BOUNCE, "elastic_in_bounce" )
, std::make_pair( CurveType::CT_ELASTIC_OUT_BOUNCE, "elastic_out_bounce" )
, std::make_pair( CurveType::CT_QUARTIC_INOUT, "quadric_inout" )
, std::make_pair( CurveType::CT_CUBIC_INTOUT, "cubic_inout" )
, std::make_pair( CurveType::CT_BEZIER, "" )        // default
};

static const std::pair< WrapMethod, const char* > wm2s_data [] =
{ std::make_pair( WrapMethod::clamp, "clamp" )
, std::make_pair( WrapMethod::pingPong, "pingPong" )
, std::make_pair( WrapMethod::repeat, "repeat" )
, std::make_pair( WrapMethod::clamp, "" ) };

const std::pair< CurveType, const char* > * MappingHelper::ct2s = ct2s_data;
const std::pair< WrapMethod, const char* > * MappingHelper::wm2s = wm2s_data;

template<>
std::string T2String< WrapMethod >( const WrapMethod& wm )
{
	return Enum2String( MappingHelper::wm2s, wm );
}
template<>

WrapMethod String2T( const std::string & s, const WrapMethod& default )
{
	if( s == "" )
		return default;
	else
		return String2Enum( MappingHelper::wm2s, s );
}

template<> std::string T2String< CurveType >( const CurveType& ct )
{
	return Enum2String( MappingHelper::ct2s, ct );
}

template<> CurveType String2T( const std::string & s, const CurveType& default )
{
	if( s == "" )
		return default;
	else
		return String2Enum( MappingHelper::ct2s, s );
}

} // SerializationHelper

} // bv
