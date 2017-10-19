#pragma once


#include "Mathematics/Interpolators/CompositeInterpolator.h"
#include "Mathematics/Transform/MatTransform.h"

#include "Utils/Accessors/CompositeTransformAccessor.h"


namespace bv
{


// ***********************
//
class ParamComparator
{
public:

    template< typename ParamType >
    static bool             CompareKeys             ( const CompositeInterpolator< TimeType, ParamType > & expected, const CompositeInterpolator< TimeType, ParamType > & actual );

    template< typename ParamType >
    static bool             CompareEvaluators       ( const CompositeInterpolator< TimeType, ParamType > & expected, const CompositeInterpolator< TimeType, ParamType > & actual );

    static bool             CompareTransformKeys    ( const CompositeTransform & expected, const CompositeTransform & actual );
};


// ========================================================================= //
// Implementation
// ========================================================================= //


// ***********************
//
template< typename ParamType >
inline bool             ParamComparator::CompareKeys            ( const CompositeInterpolator< TimeType, ParamType > & expected, const CompositeInterpolator< TimeType, ParamType > & actual )
{
    auto & expectedKeys = expected.GetKeys();
    auto & actualKeys = actual.GetKeys();

    if( actualKeys.size() == expectedKeys.size() )
    {
        for( SizeType i = 0; i < actualKeys.size(); ++i )
        {
            if( expectedKeys[ i ].t != actualKeys[ i ].t )
                return false;
            if( expectedKeys[ i ].val != actualKeys[ i ].val )
                return false;
        }
    }
    else
        return false;

    return true;
}

// ***********************
//
template< typename ParamType >
inline bool             ParamComparator::CompareEvaluators      ( const CompositeInterpolator< TimeType, ParamType > & expected, const CompositeInterpolator< TimeType, ParamType > & actual )
{
    return false;
}

// ***********************
//
inline bool             ParamComparator::CompareTransformKeys   ( const CompositeTransform & expected, const CompositeTransform & actual )
{
    bool equal = true;

    equal = equal && CompareKeys( TEST_ACCESSOR( CompositeTransform )::TranslationX( expected ), TEST_ACCESSOR( CompositeTransform )::TranslationX( actual ) );
    equal = equal && CompareKeys( TEST_ACCESSOR( CompositeTransform )::TranslationY( expected ), TEST_ACCESSOR( CompositeTransform )::TranslationY( actual ) );
    equal = equal && CompareKeys( TEST_ACCESSOR( CompositeTransform )::TranslationZ( expected ), TEST_ACCESSOR( CompositeTransform )::TranslationZ( actual ) );

    equal = equal && CompareKeys( TEST_ACCESSOR( CompositeTransform )::Pitch( expected ), TEST_ACCESSOR( CompositeTransform )::Pitch( actual ) );
    equal = equal && CompareKeys( TEST_ACCESSOR( CompositeTransform )::Yaw( expected ), TEST_ACCESSOR( CompositeTransform )::Yaw( actual ) );
    equal = equal && CompareKeys( TEST_ACCESSOR( CompositeTransform )::Roll( expected ), TEST_ACCESSOR( CompositeTransform )::Roll( actual ) );

    equal = equal && CompareKeys( TEST_ACCESSOR( CompositeTransform )::ScaleX( expected ), TEST_ACCESSOR( CompositeTransform )::ScaleX( actual ) );
    equal = equal && CompareKeys( TEST_ACCESSOR( CompositeTransform )::ScaleY( expected ), TEST_ACCESSOR( CompositeTransform )::ScaleY( actual ) );
    equal = equal && CompareKeys( TEST_ACCESSOR( CompositeTransform )::ScaleZ( expected ), TEST_ACCESSOR( CompositeTransform )::ScaleZ( actual ) );

    equal = equal && CompareKeys( TEST_ACCESSOR( CompositeTransform )::CenterX( expected ), TEST_ACCESSOR( CompositeTransform )::CenterX( actual ) );
    equal = equal && CompareKeys( TEST_ACCESSOR( CompositeTransform )::CenterY( expected ), TEST_ACCESSOR( CompositeTransform )::CenterY( actual ) );
    equal = equal && CompareKeys( TEST_ACCESSOR( CompositeTransform )::CenterZ( expected ), TEST_ACCESSOR( CompositeTransform )::CenterZ( actual ) );

    return equal;
}

}	// bv


