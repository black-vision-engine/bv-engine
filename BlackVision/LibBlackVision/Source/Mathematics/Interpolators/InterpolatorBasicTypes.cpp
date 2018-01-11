#include "stdafx.h"

#include "InterpolatorBasicTypes.h"

namespace bv
{


namespace Convert
{

// ***********************
//
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

// ***********************
//
static const std::pair< WrapMethod, const char* > wm2s_data [] =
{ std::make_pair( WrapMethod::clamp, "clamp" )
, std::make_pair( WrapMethod::pingPong, "pingPong" )
, std::make_pair( WrapMethod::repeat, "repeat" )
, std::make_pair( WrapMethod::clamp, "" ) };


IMPLEMENT_ENUM_SERIALIZATION( CurveType, ct2s_data );
IMPLEMENT_ENUM_SERIALIZATION( WrapMethod, wm2s_data );


} // Convert

} // bv
