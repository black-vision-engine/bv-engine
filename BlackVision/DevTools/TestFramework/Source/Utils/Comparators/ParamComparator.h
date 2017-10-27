#pragma once


#include "Mathematics/Interpolators/CompositeInterpolator.h"
#include "Mathematics/Transform/MatTransform.h"

#include "Utils/Accessors/CompositeTransformAccessor.h"
#include "Utils/Accessors/CompositeInterpolatorAccessor.h"
#include "Utils/Accessors/EvaluatorsAccessors.h"


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

    template< typename ParamType >
    static bool             CompareEvaluators       ( const IEvaluator< TimeType, ParamType > & expected, const IEvaluator< TimeType, ParamType > & actual );

    template< typename ParamType >
    static bool             CompareEvaluators       ( std::shared_ptr< IEvaluator< TimeType, ParamType > > expected, std::shared_ptr< IEvaluator< TimeType, ParamType > > actual );

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
    auto & expectedEvals = TEST_ACCESSOR( CompositeInterpolator )::GetEvaluators( &expected );
    auto & actualEvals = TEST_ACCESSOR( CompositeInterpolator )::GetEvaluators( &actual );

    if( actualEvals.size() == expectedEvals.size() )
    {
        for( SizeType i = 0; i < actualEvals.size(); ++i )
        {
            if( !ParamComparator::CompareEvaluators( *expectedEvals[ i ].get(), *actualEvals[ i ].get() ) )
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
inline bool             ParamComparator::CompareEvaluators      ( const IEvaluator< TimeType, ParamType > & expected, const IEvaluator< TimeType, ParamType > & actual )
{
    if( expected.GetType() != actual.GetType() )
        return false;

    bool equal = false;
    switch( actual.GetType() )
    {
        case EvaluatorType::ET_BEZIER:
            equal = TEST_ACCESSOR( BezierEvaluator )::Compare( static_cast< const BezierEvaluator< TimeType, ParamType > & >( expected ), static_cast< const BezierEvaluator< TimeType, ParamType > & >( actual ) );
            break;
        case EvaluatorType::ET_CONSTANT:
            equal = TEST_ACCESSOR( ConstEvaluator )::Compare( static_cast< const ConstEvaluator< TimeType, ParamType > & >( expected ), static_cast< const ConstEvaluator< TimeType, ParamType > & >( actual ) );
            break;
        case EvaluatorType::ET_LINEAR:
            equal = TEST_ACCESSOR( LinearEvaluator )::Compare( static_cast< const LinearEvaluator< TimeType, ParamType > & >( expected ), static_cast< const LinearEvaluator< TimeType, ParamType > & >( actual ) );
            break;
        case EvaluatorType::ET_POLYNOMIAL:
            equal = TEST_ACCESSOR( PolynomialEvaluator )::Compare( static_cast< const PolynomialEvaluator< TimeType, ParamType > & >( expected ), static_cast< const PolynomialEvaluator< TimeType, ParamType > & >( actual ) );
            break;
    }

    if( !equal )
        return false;
    return true;
}

// ***********************
//
template<>
inline bool             ParamComparator::CompareEvaluators< std::string >      ( const IEvaluator< TimeType, std::string > & expected, const IEvaluator< TimeType, std::string > & actual )
{
    if( expected.GetType() != actual.GetType() )
        return false;

    if( actual.GetType() != EvaluatorType::ET_CONSTANT )
        return false;

    return TEST_ACCESSOR( ConstEvaluator )::Compare( static_cast< const ConstEvaluator< TimeType, std::string > & >( expected ), static_cast< const ConstEvaluator< TimeType, std::string > & >( actual ) );
}

// ***********************
//
template<>
inline bool             ParamComparator::CompareEvaluators< std::wstring >      ( const IEvaluator< TimeType, std::wstring > & expected, const IEvaluator< TimeType, std::wstring > & actual )
{
    if( expected.GetType() != actual.GetType() )
        return false;

    if( actual.GetType() != EvaluatorType::ET_CONSTANT )
        return false;

    return TEST_ACCESSOR( ConstEvaluator )::Compare( static_cast< const ConstEvaluator< TimeType, std::wstring > & >( expected ), static_cast< const ConstEvaluator< TimeType, std::wstring > & >( actual ) );
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


