#pragma once


#include "Mathematics/Interpolators/CompositeInterpolator.h"



namespace bv
{


// ***********************
//
class ParamComparator
{
public:

    template< typename ParamType >
    static bool             CompareKeys     ( const CompositeInterpolator< ParamType, TimeType > & expected, const CompositeInterpolator< ParamType, TimeType > & actual );

};


// ========================================================================= //
// Implementation
// ========================================================================= //


template< typename ParamType >
inline bool             ParamComparator::CompareKeys        ( const CompositeInterpolator< ParamType, TimeType > & expected, const CompositeInterpolator< ParamType, TimeType > & actual )
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

}	// bv


