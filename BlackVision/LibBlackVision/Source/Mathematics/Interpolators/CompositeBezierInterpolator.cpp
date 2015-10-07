#include "CompositeBezierInterpolator.h"
#include "Mathematics/Core/mathfuncs.h"

namespace bv {

// *******************************
//
template< class TimeValueT, class ValueT >
class ConstEvaluator : public IEvaluator< TimeValueT, ValueT >
{
    ValueT value;
public:
    ConstEvaluator( ValueT v ) : value( v ) {}

    virtual EvaluatorType GetType() override { return EvaluatorType::CONSTANT; }

    virtual void SetValue( TimeValueT /*t*/, ValueT v ) override
    {
        value = v;
    }

    ValueT Evaluate( TimeValueT ) const override { return value; }
};

// *******************************
//
template< class TimeValueT, class ValueT >
class LinearEvaluator : public IEvaluator< TimeValueT, ValueT >
{
    typedef Key< TimeValueT, ValueT > Key;

    Key key1, key2;
public:
    LinearEvaluator( Key k1, Key k2 ) : key1( k1 ), key2( k2 ) {}

    virtual EvaluatorType GetType() override { return EvaluatorType::LINEAR; }

    virtual void SetValue( TimeValueT t, ValueT v ) override
    {
        if( key1.t == t )
            key1.val = v;
        else if( key2.t == t )
            key2.val = v;
        else
            assert( false );
    }

    ValueT Evaluate( TimeValueT t ) const override 
    { 
        float alpha = ( t - key1.t ) / ( key2.t - key1.t );
        return alpha * key2.val + (1-alpha) * key1.val;
    }
};

// *******************************
//
template< class TimeValueT, class ValueT >
class BezierEvaluator : public IEvaluator< TimeValueT, ValueT >
{
    typedef Key< TimeValueT, ValueT > Key;

public: // FIXME
    Key key1, key2;
    Key v1, v2;
    TimeValueT m_tolerance;
public:
    virtual EvaluatorType GetType() override { return EvaluatorType::BEZIER; }

    BezierEvaluator( Key k1, Key k2, Key v1_, Key v2_, TimeValueT tolerance ) : key1( k1 ), key2( k2 ), v1( v1_ ), v2( v2_ ), m_tolerance( tolerance ) {}
    
    void SetV2( Key v2 ) { this->v2 = v2; }
    
    virtual void SetValue( TimeValueT t, ValueT v ) override
    {
        if( key1.t == t )
            key1.val = v;
        else if( key2.t == t )
            key2.val = v;
        else
            assert( false );
    }

    ValueT Evaluate( TimeValueT t ) const override 
    {
        assert( key1.t <= t && t <= key2.t );

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

// *******************************
//
CompositeBezierInterpolator::CompositeBezierInterpolator( float tolerance )
    : m_type( CurveType::LINEAR )
    , m_tolerance( tolerance )
    , m_preMethod( WrapMethod::clamp ), m_postMethod( WrapMethod::clamp )
{
}

// *******************************
//
CompositeBezierInterpolator::CompositeBezierInterpolator( const CompositeBezierInterpolator& that )
{ 
    keys = that.keys; 
    interpolators = that.interpolators; 
    m_tolerance = that.m_tolerance; 
    m_type = that.m_type; 
    m_preMethod = that.m_preMethod;
    m_postMethod = that.m_postMethod;
}

// *******************************
//
template< class TimeValueT, class ValueT >
IEvaluator<TimeValueT, ValueT >* CreateDummyInterpolator( CurveType type, Key< TimeValueT, ValueT > k1, Key< TimeValueT, ValueT > k2, TimeValueT tolerance ) // FIXME maybe
{
    if( type == CurveType::POINT )
        return new ConstEvaluator< TimeValueT, ValueT >( k1.val );
    else if( type == CurveType::LINEAR )
        return new LinearEvaluator< TimeValueT, ValueT >( k1, k2 );
    else if( type == CurveType::BEZIER )
        return new BezierEvaluator< TimeValueT, ValueT >( k1, k2, Key< TimeValueT, ValueT >( 0, 0 ), Key< TimeValueT, ValueT >( 0, 0 ), tolerance );
    else if( type == CurveType::COSINE_LIKE )
        return new BezierEvaluator< TimeValueT, ValueT >( k1, k2, Key< TimeValueT, ValueT >( 0, 0 ), Key< TimeValueT, ValueT >( 0, 0 ), tolerance );
    else
    {
        assert( false );
        return nullptr;
    }
}

// *******************************
//
template< class TimeValueT, class ValueT >
void UpdateInterpolator( std::vector< IEvaluator<TimeValueT, ValueT >* >& interpolators, size_t i, CurveType cType )
{
    typedef Key< TimeValueT, ValueT > Key;

    const float scale = 0.3f;

    auto iType = interpolators[ i ]->GetType();

    if( iType == EvaluatorType::CONSTANT || iType == EvaluatorType::LINEAR )
        return;

    assert( iType == EvaluatorType::BEZIER );
    auto be = ( BezierEvaluator< TimeValueT, ValueT >* ) interpolators[ i ];

    if( cType == CurveType::COSINE_LIKE )
    {
        float length = be->key2.t - be->key1.t;
        be->v1 = Key( scale * length, 0 );
        be->v2 = Key( -scale * length, 0 );
    }
    else if( cType == CurveType::BEZIER )
    {
        auto prevE = ( i == 0 ) ? interpolators[ 0 ] : interpolators[ i-1 ];
        auto nextE = ( i == interpolators.size()-1 ) ? interpolators.back() : interpolators[ i+1 ];

        auto prevBE = ( BezierEvaluator< TimeValueT, ValueT >* ) prevE;
        auto nextBE = ( BezierEvaluator< TimeValueT, ValueT >* ) nextE;

        be->v1 = scale * ( be->key2 - prevBE->key1 );
        be->v2 = -scale * ( nextBE->key2 - be->key1 );
    }
    else
        assert( false );
}

// *******************************
//
void CompositeBezierInterpolator::AddKey             ( TimeValueT t, const ValueT & v ) 
{ 
    if( keys.empty() )
    {
        keys.push_back( Key( t, v ) );
        return;
    }

// find the proper key
    static_assert(std::numeric_limits<float>::is_iec559, "IEEE 754 required");
    Key left( -std::numeric_limits< float >::infinity(), 444.f );
    Key right = keys.front();
    size_t i = 0, last = keys.size()-1;

    while( t > right.t+m_tolerance )
    {
        left = right;
        if( i++ == last )
        {
            right = Key( std::numeric_limits< float >::infinity(), 444.f );
            break;
        }
        else
            right = keys[ i ];
    }

    assert( left.t <= t && t <= right.t );
    bool isSame = fabs( t - right.t ) < m_tolerance;

    if( isSame )
    {
        keys[ i ].val = v;

        if( i != 0 )
            interpolators[ i-1 ]->SetValue( t, v );
        if( i != last )
            interpolators[ i ]->SetValue( t, v );
    }
    else
    {
        auto key = Key( t, v );
        keys.insert( keys.begin() + i, key );

        if( i == 0 )
            interpolators.insert( interpolators.begin(), CreateDummyInterpolator( m_type, key, right, m_tolerance ) );
        else if( i == last+1 )
            interpolators.push_back( CreateDummyInterpolator( m_type, left, key, m_tolerance ) );
        else
        {
            interpolators.erase( interpolators.begin() + (i-1) );
            interpolators.insert( interpolators.begin() + (i-1), CreateDummyInterpolator( m_type, left, key, m_tolerance ) );
            interpolators.insert( interpolators.begin() + i, CreateDummyInterpolator( m_type, key, right, m_tolerance ) );
        }
    }

// update interpolators
    for( size_t j = i-2; j <= i+1; j++ )
        if( j >= 0 && j < interpolators.size() )
            UpdateInterpolator( interpolators, j, m_type );
}


// *******************************
//
float CompositeBezierInterpolator::PreEvaluate( float t ) const 
{ 
    TimeValueT tStart = keys.front().t;
    TimeValueT tEnd = keys.back().t;

    auto interval = tEnd - tStart;
    if( interval <= m_tolerance )
        return tStart;

    t = t - tStart;

    if( m_preMethod == WrapMethod::clamp )
        return tStart;
    else if( m_preMethod == WrapMethod::repeat )
    {
        TimeValueT q = interval;
        TimeValueT r = std::modf( t, &q );
        return tStart + r;
    }
    else if ( m_preMethod == WrapMethod::pingPong )
    {
        TimeValueT q = interval;
        TimeValueT r = std::modf( t, &q );

        if( round( q ) % 2 == 0 )
        {
            return tStart + r;
        }
        else
        {
            return tStart + interval - r;
        }
    }

    return Evaluate( t );
}

// *******************************
//
float CompositeBezierInterpolator::PostEvaluate( float t ) const 
{ 
    TimeValueT tStart = keys.front().t;
    TimeValueT tEnd = keys.back().t;

    auto interval = tEnd - tStart;
    if( interval <= m_tolerance )
        return tEnd;

    t = t - tStart;

    if( m_postMethod == WrapMethod::clamp )
        return tEnd;
    else if( m_postMethod == WrapMethod::repeat )
    {
        TimeValueT q = interval;
        TimeValueT r = divmod( t, &q );
        return tStart + r;
    }
    else if( m_postMethod == WrapMethod::pingPong )
    {
        TimeValueT q = interval;
        TimeValueT r = divmod(t, &q);

        if( round( q ) % 2 == 0 )
        {
            return tStart + r;
        }
        else
        {
            return tStart + interval - r;
        }
    }

    return Evaluate( t );
}

// *******************************
//
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

// *******************************
//
const std::vector< CompositeBezierInterpolator::Key > &                          CompositeBezierInterpolator::GetKeys()
{
    return keys;
}

// *******************************
//
const std::vector< CompositeBezierInterpolator::IEvaluator* > &                  CompositeBezierInterpolator::GetInterpolators()
{
    return interpolators;
}

// *******************************
//
void                                                CompositeBezierInterpolator::SetCurveType( CurveType type )
{
    m_type = type;
}

// *******************************
//
void                                                CompositeBezierInterpolator::SetWrapPostMethod  ( WrapMethod method )
{
    m_postMethod = method;
}

// *******************************
//
void                                                CompositeBezierInterpolator::SetWrapPreMethod   ( WrapMethod method )
{
    m_preMethod = method;
}

// *******************************
//
void                                                CompositeBezierInterpolator::SetKey1( int i, Key key )
{
    assert( interpolators[ i ]->GetType() == EvaluatorType::BEZIER );
    ( ( BezierEvaluator< TimeValueT, ValueT >* ) interpolators[ i ] )->key1 = key;
}

// *******************************
//
void                                                CompositeBezierInterpolator::SetKey2( int i, Key key )
{
    assert( interpolators[ i ]->GetType() == EvaluatorType::BEZIER );
    ( ( BezierEvaluator< TimeValueT, ValueT >* ) interpolators[ i ] )->key2 = key;
}

// *******************************
//
void                                                CompositeBezierInterpolator::SetV1( int i, Key v )
{
    assert( interpolators[ i ]->GetType() == EvaluatorType::BEZIER );
    ( ( BezierEvaluator< TimeValueT, ValueT >* ) interpolators[ i ] )->v1 = v;
}

// *******************************
//
void                                                CompositeBezierInterpolator::SetV2( int i, Key v )
{
    assert( interpolators[ i ]->GetType() == EvaluatorType::BEZIER );
    ( ( BezierEvaluator< TimeValueT, ValueT >* ) interpolators[ i ] )->v2 = v;
}


} // bv
