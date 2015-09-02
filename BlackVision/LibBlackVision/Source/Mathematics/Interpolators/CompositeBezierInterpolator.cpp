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
    ValueT Evaluate( TimeValueT t ) const override 
    { 
        float alpha = ( t - key1.t ) / ( key2.t - key1.t );
        return alpha * key2.val + (1-alpha) * key1.val;
    }
};

template< class TimeValueT, class ValueT >
class BezierEvaluator : public IInterpolator< TimeValueT, ValueT >
{
    typedef Key< TimeValueT, ValueT > Key;

    Key key1, key2;
    Key v1, v2;
    TimeValueT m_tolerance;
public:
    BezierEvaluator( Key k1, Key k2, Key v1_, Key v2_, TimeValueT tolerance ) : key1( k1 ), key2( k2 ), v1( v1_ ), v2( v2_ ), m_tolerance( tolerance ) {}
    
    void SetV2( Key v2 ) { this->v2 = v2; }
    
    ValueT Evaluate( TimeValueT t ) const override 
    {
        Key A = key1,
            B = key1 + v1,
            C = key2 + v2,
            D = key2;

        for( ; ; ) 
        {
            Key middle = 1./8 * A + 3./8 * B + 3./8 * C + 1./8 * D;
            
            if( fabs( middle.t - t ) < m_tolerance )
                return middle.val;
            else
            {
                if( t < middle.t )
                {
                    A = A;
                    B = 1./2 * A + 1./2 * B;
                    C = 1./4 * A + 1./2 * B + 1./4 * C;
                    D = middle;
                }
                else
                {
                    A = middle;
                    B = 1./4 * B + 1./2 * C + 1./4 * D;
                    C = 1./2 * C + 1./2 * D;
                    D = D;               
                }
            }
        }
    }
};

CompositeBezierInterpolator::CompositeBezierInterpolator( float tolerance )
    : m_type( CurveType::LINEAR )
    , m_tolerance( tolerance )
{
}

CompositeBezierInterpolator::CompositeBezierInterpolator( const CompositeBezierInterpolator& that )
{ 
    keys = that.keys; 
    interpolators = that.interpolators; 
    m_tolerance = that.m_tolerance; 
    m_type = that.m_type; 
}

void CompositeBezierInterpolator::AddKey             ( TimeValueT t, const ValueT & v ) 
{ 
    typedef Key< TimeValueT, ValueT > Key;

    //if( keys.size() > 0 && keys[ keys.size()-1 ].t == t )
    //    keys.pop_back();
    //assert( keys.size() == 0 || keys[ keys.size()-1 ].t < t ); // FIXME don't assume that for God's sake!

    if( keys.empty() )
    {
        keys.push_back( Key( t, v ) );
        return;
    }

// find the proper key
    static_assert(std::numeric_limits<float>::is_iec559, "IEEE 754 required");
    Key left( -std::numeric_limits<float>::infinity(), 0 );
    Key right = keys.front();
    auto it = keys.begin();

    while( t > right.t)
    {
        left = right;
        it++;
        if( it == keys.end() )
            right = Key( std::numeric_limits< float >::infinity(), 0 );
        else
            right = *it; // ;)
    }

    assert( left.t <= t && t <= right.t );
    keys.insert( it, Key( t, v ) );
    //keys.push_back( Key( t, v ) ); // FIXME sortme

    if( keys.size() > 1 )
    {
        size_t last = keys.size()-1;
        const float scale = 0.3f;

        if( m_type == CurveType::POINT )
        {
            interpolators.push_back( new ConstEvaluator< TimeValueT, ValueT >( v ) );
        }
        else if( m_type == CurveType::LINEAR )
        {
            interpolators.push_back( new LinearEvaluator< TimeValueT, ValueT >( keys[ last-1 ], keys[ last ] ) );
        }
        else if( m_type == CurveType::COSINE_LIKE )
        {
            float length = keys[ last ].t - keys[ last-1 ].t;

            interpolators.push_back( new BezierEvaluator< TimeValueT, ValueT >( keys[ last-1 ], keys[ last ], Key( scale * length, 0 ), Key( -scale * length, 0 ), m_tolerance ) );
        }
        else if( m_type == CurveType::BEZIER )
        {
            Key left = ( last > 1 ) ? scale * ( keys[ last ] - keys[ last-2 ] ) : scale * ( keys[ last ] - keys[ last-1 ] );
            Key right = scale * ( keys[ last ] - keys[ last-1 ] );

            interpolators.push_back( new BezierEvaluator< TimeValueT, ValueT >( keys[ last-1 ], keys[ last ], left, right, m_tolerance ) );

            if( last > 1 )
            {
                auto bi = ( BezierEvaluator< TimeValueT, ValueT >* )interpolators[ last-2 ];
                bi->SetV2( -1 * left );
            }
        }
        else
            assert( false );
    }
}

void                    CompositeBezierInterpolator::SetCurveType( CurveType type )
{
    m_type = type;
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
