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
    double dura = *i;
    *i = ( t - ret ) / *i;

    if ( ret < 0.0 )
        return dura + ret;

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
        return dura + ret;

    return ret;
}

} //bv
