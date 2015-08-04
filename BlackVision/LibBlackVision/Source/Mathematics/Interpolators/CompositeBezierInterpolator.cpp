#include "CompositeBezierInterpolator.h"

namespace bv {

void CompositeBezierInterpolator::AddKey             ( TimeValueT t, const ValueT & v ) 
{ 
    keys.push_back( Key<TimeValueT, ValueT>( t, v ) ); // FIXME sortme
}

float CompositeBezierInterpolator::PreEvaluate( float /*t*/ ) const { return keys[ 0 ].val; } // never FIXME :P
float CompositeBezierInterpolator::PostEvaluate( float /*t*/ ) const { return keys[ keys.size()-1 ].val; } // never FIXME :P

CompositeBezierInterpolator::ValueT CompositeBezierInterpolator::Evaluate         ( TimeValueT t ) const 
{ 
    if( keys.size() == 0 )
    {
        assert( false ); // FIXME: error handling FTW
        return 0.f;
    }

    if( t < keys[0].t )
        return PreEvaluate( t );

    int i = 0;
    while( t > keys[ i ].t && i < keys.size()-1 )
        i++;

    if( t > keys[i].t )
        return PostEvaluate( t );

    return keys[ i ].val;
}

} // bv