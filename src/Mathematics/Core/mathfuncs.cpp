#include "mathfuncs.h"

#include <cmath>


namespace bv {

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
