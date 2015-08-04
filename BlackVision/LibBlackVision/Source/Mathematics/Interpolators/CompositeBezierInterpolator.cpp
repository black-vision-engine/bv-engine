#include "CompositeBezierInterpolator.h"

namespace bv {

void CompositeBezierInterpolator::AddKey             ( TimeValueT t, const ValueT & v ) 
{ 
    keys.push_back( Key<TimeValueT, ValueT>( t, v ) ); // FIXME sortme
}

CompositeBezierInterpolator::ValueT CompositeBezierInterpolator::Evaluate         ( TimeValueT t ) const 
{ 
    int i = 0;
    while( t > keys[ i ].t && i < keys.size()-1 )
        i++;
    return keys[ i ].val;
}

} // bv