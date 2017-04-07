#include "stdafxCore.h"

#include "mathfuncs.h"

#include <cmath>
#include <cstdio>

#include "Memory/MemoryLeaks.h"

namespace bv { namespace mathematics {

// *************************************
//
Int32 round( Float32 val )
{
    return static_cast< Int32 >( std::floor( 0.5f + val ) );
}

// *************************************
//
Int32 round( Float64 val )
{
    return static_cast< Int32 >( std::floor( 0.5 + val ) );
}

// *************************************
//
Float64 divmod( Float64 t, Float64 * i )
{
    Float64 ret = std::fmod( t, *i );
    Float64 dura = *i;
    *i = ( t - ret ) / *i;

    printf( " D:%.2f", ret );

    if( ret < 0.0 )
    {
        *i -= 1.0;
        return dura + ret;
    }

    return ret;
}

// *************************************
//
Float32 divmod( Float32 t, Float32 * i )
{
    Float32 ret = std::fmod( t, *i );
    Float32 dura = *i;
    *i = ( t - ret ) / *i;

    if( ret < 0.0f )
    {
        *i -= 1.0f;
        return dura + ret;
    }

    return ret;
}

} // mathemtics
} //bv
