#pragma once

#include "math.h"

namespace bv
{

const float fEpsilon = 0.0000001f;

inline bool    CompareFloats( const float & expected, const float & actual )
{
    return ( fabs( expected - actual ) < fEpsilon );
}

}	// bv