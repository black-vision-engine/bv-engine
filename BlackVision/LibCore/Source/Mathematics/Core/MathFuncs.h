#pragma once

#include "CoreDEF.h"

namespace bv { namespace mathematics {

Int32 round( Float32 val );
Int32 round( Float32 val );

Float32 divmod( Float32 t, Float32 * i );
Float64 divmod( Float64 t, Float64 * i );

// *************************************
//
inline UInt32 gcd( UInt32 a, UInt32 b )
{
    return b == 0 ? a : gcd( b, a % b );
}

// *************************************
//
inline UInt32 lcm( UInt32 a, UInt32 b )
{
    return a * b / gcd( a, b );
}

} //mathemtics
} //bv
