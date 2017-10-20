#pragma once

#include "Mathematics/Interpolators/Functions/BezierFunction.h"
#include "Mathematics/Interpolators/Functions/ConstFunction.h"
#include "Mathematics/Interpolators/Functions/LinearFunction.h"
#include "Mathematics/Interpolators/Functions/PolynomialFunction.h"


namespace bv
{

// ***********************
//
class TEST_ACCESSOR( BezierEvaluator )
{
public:

    template< typename ParamType >
    static bool         Compare         ( const BezierEvaluator< TimeType, ParamType > & expected, const BezierEvaluator< TimeType, ParamType > & actual );
};

// ***********************
//
class TEST_ACCESSOR( ConstEvaluator )
{
public:

    template< typename ParamType >
    static bool         Compare         ( const ConstEvaluator< TimeType, ParamType > & expected, const ConstEvaluator< TimeType, ParamType > & actual );
};

// ***********************
//
class TEST_ACCESSOR( LinearEvaluator )
{
public:

    template< typename ParamType >
    static bool         Compare         ( const LinearEvaluator< TimeType, ParamType > & expected, const LinearEvaluator< TimeType, ParamType > & actual );
};

// ***********************
//
class TEST_ACCESSOR( PolynomialEvaluator )
{
public:

    template< typename ParamType >
    static bool         Compare         ( const PolynomialEvaluator< TimeType, ParamType > & expected, const PolynomialEvaluator< TimeType, ParamType > & actual );
};


// ========================================================================= //
// Implementation
// ========================================================================= //


const float FloatLimit = 1.0e-6f;

// ***********************
//
template< typename ParamType >
inline bool         CompareEpsilon      ( const ParamType & param1, const ParamType & param2 )
{
    return param1 == param2;
}

// ***********************
//
template<>
inline bool         CompareEpsilon< float >     ( const float & param1, const float & param2 )
{
    if( abs( param1 - param2 ) < FloatLimit )
        return true;
    return false;
}

// ***********************
//
template<>
inline bool         CompareEpsilon< glm::vec2 >     ( const glm::vec2 & param1, const glm::vec2 & param2 )
{
    if( abs( param1.x - param2.x ) < FloatLimit &&
        abs( param1.y - param2.y ) < FloatLimit )
        return true;
    return false;
}

// ***********************
//
template<>
inline bool         CompareEpsilon< glm::vec3 >     ( const glm::vec3 & param1, const glm::vec3 & param2 )
{
    if( abs( param1.x - param2.x ) < FloatLimit &&
        abs( param1.y - param2.y ) < FloatLimit &&
        abs( param1.z - param2.z ) < FloatLimit )
        return true;
    return false;
}

// ***********************
//
template<>
inline bool         CompareEpsilon< glm::vec4 >     ( const glm::vec4 & param1, const glm::vec4 & param2 )
{
    if( abs( param1.x - param2.x ) < FloatLimit &&
        abs( param1.y - param2.y ) < FloatLimit &&
        abs( param1.z - param2.z ) < FloatLimit &&
        abs( param1.w - param2.w ) < FloatLimit )
        return true;
    return false;
}


// ***********************
//
template< typename ParamType >
inline bool         operator!=          ( const Key< TimeType, ParamType > & key1, const Key< TimeType, ParamType > & key2 )
{
    if( key1.t == key2.t &&
        CompareEpsilon< ParamType >( key1.val, key2.val ) )
        return false;
    return true;
}


// ***********************
//
template< typename ParamType >
bool        TEST_ACCESSOR( BezierEvaluator )::Compare       ( const BezierEvaluator< TimeType, ParamType > & expected, const BezierEvaluator< TimeType, ParamType > & actual )
{
    if( expected.GetCurveType() != actual.GetCurveType() )
        return false;

    if( expected.key1 != actual.key1 )
        return false;

    if( expected.key2 != actual.key2 )
        return false;

    if( expected.v1 != actual.v1 )
        return false;

    if( expected.v2 != actual.v2 )
        return false;

    if( expected.m_tolerance != actual.m_tolerance )
        return false;

    return true;
}

// ***********************
//
template< typename ParamType >
bool        TEST_ACCESSOR( ConstEvaluator )::Compare        ( const ConstEvaluator< TimeType, ParamType > & expected, const ConstEvaluator< TimeType, ParamType > & actual )
{
    if( expected.GetCurveType() != actual.GetCurveType() )
        return false;

    if( expected.value != actual.value )
        return false;

    return true;
}

// ***********************
//
template< typename ParamType >
bool        TEST_ACCESSOR( LinearEvaluator )::Compare       ( const LinearEvaluator< TimeType, ParamType > & expected, const LinearEvaluator< TimeType, ParamType > & actual )
{
    if( expected.GetCurveType() != actual.GetCurveType() )
        return false;

    if( expected.key1 != actual.key1 )
        return false;

    if( expected.key2 != actual.key2 )
        return false;

    if( expected.m_tolerance != actual.m_tolerance )
        return false;

    return true;
}

// ***********************
//
template< typename ParamType >
bool        TEST_ACCESSOR( PolynomialEvaluator )::Compare       ( const PolynomialEvaluator< TimeType, ParamType > & expected, const PolynomialEvaluator< TimeType, ParamType > & actual )
{
    if( expected.GetCurveType() != actual.GetCurveType() )
        return false;

    if( expected.key1 != actual.key1 )
        return false;

    if( expected.key2 != actual.key2 )
        return false;

    if( expected.m_tolerance != actual.m_tolerance )
        return false;

    if( expected.bounce != actual.bounce )
        return false;

    if( expected.a != actual.a )
        return false;

    if( expected.b != actual.b )
        return false;

    if( expected.c != actual.c )
        return false;

    if( expected.d != actual.d )
        return false;

    if( expected.e != actual.e )
        return false;
    
    return true;
}


}	// bv

