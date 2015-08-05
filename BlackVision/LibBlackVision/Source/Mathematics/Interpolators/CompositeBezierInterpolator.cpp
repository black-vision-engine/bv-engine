#include "CompositeBezierInterpolator.h"

namespace bv {

template< class TimeValueT, class ValueT >
class ConstEvaluator : public IInterpolator< TimeValueT, ValueT >
{
    ValueT value;
public:
    ConstEvaluator( ValueT v ) : value( v ) {}
    ValueT Evaluate( TimeValueT ) const override { return value; }
};

template< class TimeValueT, class ValueT >
class LinearEvaluator : public IInterpolator< TimeValueT, ValueT >
{
    typedef Key< TimeValueT, ValueT > Key;

    Key key1, key2;
public:
    LinearEvaluator( Key k1, Key k2 ) : key1( k1 ), key2( k2 ) {}
    ValueT Evaluate( TimeValueT ) const override { return value; }
};

void CompositeBezierInterpolator::AddKey             ( TimeValueT t, const ValueT & v ) 
{ 
    assert( keys.size() == 0 || keys[ keys.size()-1 ].t <= t ); // FIXME don't assume that for God's sake!
    keys.push_back( Key<TimeValueT, ValueT>( t, v ) ); // FIXME sortme
    if( keys.size() > 1 )
        interpolators.push_back( new ConstEvaluator< TimeValueT, ValueT >( v ) );
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

    if( keys.size() == 1 )
        return keys[ 0 ].val;

    if( t < keys[0].t )
        return PreEvaluate( t );

    int i = 0;
    while( t > keys[ i+1 ].t && i < keys.size()-2 )
        i++;

    if( t > keys[ i+1 ].t )
        return PostEvaluate( t );

    return interpolators[ i ]->Evaluate( t );
}

} // bv