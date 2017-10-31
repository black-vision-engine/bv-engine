#pragma once

#include "CoreDEF.h"

#include "Serialize.h"

#include "Mathematics/Interpolators/CompositeInterpolator.h"
#include "Mathematics/Interpolators/CompositeInterpolator.inl"





namespace bv
{




// ***********************
//
inline std::shared_ptr< CompositeInterpolator< TimeType, std::string > >        DeserializeFloatCompositeInterpolator       ( const std::string & path )
{
    return Deserialize< CompositeInterpolator< TimeType, std::string > >( path, "interpolator" );
}

// ***********************
//
template< typename Type >
inline std::shared_ptr< BezierEvaluator< TimeType, Type > >                     DeserializeBezier   ( const std::string & path, CurveType curve, Key< TimeType, Type > k1, Key< TimeType, Type > k2 )
{
    BVXMLDeserializer deser( path, BVDeserializeContext::CreateContextFromEmptiness() );

    TimeType tolerance = 0.00001f;
    auto bezier = CreateDummyInterpolator< TimeType, Type >( curve, k1, k2, tolerance );

    deser.EnterChild( "interpolation" );
    bezier->Deserialize( deser );
    deser.ExitChild();

    return std::static_pointer_cast< BezierEvaluator< TimeType, Type > >( bezier );
}


}	// bv

