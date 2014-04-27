#include "mathfuncs.h"

#include <cmath>


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
    *i = ( t - ret ) / *i;

    return ret;
}

// *************************************
//
float divmod( float t, float * i )
{
    float ret = std::fmod( t, *i );
    *i = ( t - ret ) / *i;

    return ret;
}

} //bv
