#include "stdafx.h"

#include "mathfuncs.h"

#include <cmath>
#include <cstdio>




#include "Memory/MemoryLeaks.h"



namespace bv {

// *************************************
//
int round( float val )
{
    return static_cast<int>( std::floor( 0.5f + val ) );
}

// *************************************
//
int round( double val )
{
    return static_cast<int>( std::floor( 0.5 + val ) );
}

// *************************************
//
double divmod( double t, double * i )
{
    double ret = std::fmod( t, *i );
    double dura = *i;
    *i = ( t - ret ) / *i;

    printf( " D:%.2f", ret );

    if ( ret < 0.0 )
    {
        *i -= 1.0;
        return dura + ret;
    }

    return ret;
}

// *************************************
//
float divmod( float t, float * i )
{
    float ret = std::fmod( t, *i );
    float dura = *i;
    *i = ( t - ret ) / *i;
    
    if ( ret < 0.0f )
    {
        *i -= 1.0f;
        return dura + ret;
    }

    return ret;
}

} //bv
