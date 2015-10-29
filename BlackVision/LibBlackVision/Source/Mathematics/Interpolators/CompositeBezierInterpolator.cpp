#include "CompositeBezierInterpolator.h"
#include "Mathematics/Core/mathfuncs.h"

#include "Serialization/SerializationHelper.h"

#include <vector>
#include <array>
//#include <initializer_list>

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

    virtual void                                        Serialize       ( ISerializer& ser ) const override
    {
    ser.EnterChild( "interpolation" );
        ser.SetAttribute( "type", "point" );
    ser.ExitChild();
    }
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
        TimeValueT alpha = ( t - key1.t ) / ( key2.t - key1.t );
        return ValueT( alpha * key2.val + (1-alpha) * key1.val );
    }

    virtual void                                        Serialize       ( ISerializer& ser ) const override
    {
    ser.EnterChild( "interpolation" );
        ser.SetAttribute( "type", "linear" );
    ser.ExitChild();
    }
};

Key< bv::TimeType, bool > Key< bv::TimeType, bool >::operator+( const Key< bv::TimeType, bool > &/*that*/ ) const { assert( false ); return Key< bv::TimeType, bool >( 0, false ); }
Key< bv::TimeType, bool > Key< bv::TimeType, bool >::operator-( const Key< bv::TimeType, bool > &/*that*/ ) const { assert( false ); return Key< bv::TimeType, bool >( 0, false ); }

template<>
Key< bv::TimeType, bool > operator*( const bv::TimeType & /*a*/, const Key< bv::TimeType, bool > &/*that*/ ) { assert( false ); return Key< bv::TimeType, bool >( 0, false ); }

template<>
bool LinearEvaluator< bv::TimeType, bool >::Evaluate( bv::TimeType t ) const
{
    bv::TimeType alpha = ( t - key1.t ) / ( key2.t - key1.t );
    return int( alpha * key2.val + (1-alpha) * key1.val ) > 0;
}

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
            Key middle = 1.f/8 * A + 3.f/8 * B + 3.f/8 * C + 1.f/8 * D;
            
            if( fabs( middle.t - t ) < m_tolerance )
                return middle.val;
            else
            {
                if( t < middle.t )
                {
                    A = A;
                    B = 1.f/2 * A + 1.f/2 * B;
                    C = 1.f/4 * A + 1.f/2 * B + 1.f/4 * C;
                    D = middle;
                }
                else
                {
                    A = middle;
                    B = 1.f/4 * B + 1.f/2 * C + 1.f/4 * D;
                    C = 1.f/2 * C + 1.f/2 * D;
                    D = D;               
                }
            }
        }
    }

    virtual void                                        Serialize       ( ISerializer& ser ) const override
    {
    ser.EnterChild( "interpolation" );
        ser.SetAttribute( "type", "bezier" );
        ser.SetAttribute( "v1", std::to_string( v1.t ) + ", " + std::to_string( v1.val ) );
        ser.SetAttribute( "v2", std::to_string( v2.t ) + ", " + std::to_string( v2.val ) );
    ser.ExitChild();
    }
};

// *******************************
//
template< class TimeValueT, class ValueT >
CompositeBezierInterpolator< TimeValueT, ValueT >::CompositeBezierInterpolator( float tolerance )
    //: m_type( CurveType::LINEAR )
    //: m_type( CurveType::COSINE_LIKE )
    //: m_type( CurveType::POINT )
    : m_type( CurveType::BEZIER )
    , m_tolerance( tolerance )
    , m_preMethod( WrapMethod::clamp ), m_postMethod( WrapMethod::clamp )
{
}

// *******************************
//
template< class TimeValueT, class ValueT >
CompositeBezierInterpolator< TimeValueT, ValueT >::CompositeBezierInterpolator( const CompositeBezierInterpolator& that )
{ 
    keys = that.keys; 
    interpolators = that.interpolators; 
    m_tolerance = that.m_tolerance; 
    m_type = that.m_type; 
    m_preMethod = that.m_preMethod;
    m_postMethod = that.m_postMethod;
}

std::pair< CurveType, const char* > ct2s[] = 
{ std::make_pair( CurveType::BEZIER, "bezier" )
    , std::make_pair( CurveType::COSINE_LIKE, "cosine" ) 
    , std::make_pair( CurveType::LINEAR, "linear" ) 
    , std::make_pair( CurveType::POINT, "point" ) 
};

// *************************************
//
template< class TimeValueT, class ValueT >
void                                        CompositeBezierInterpolator< TimeValueT, ValueT >::Serialize       ( ISerializer& ser ) const
{
ser.EnterChild( "interpolator" );
    ser.SetAttribute( "curve_type", SerializationHelper::T2String< CurveType >( ct2s, m_type ) );
    for( size_t i = 0; i < interpolators.size(); i++ )
    {
        keys[ i ].Serialize( ser );
        interpolators[ i ]->Serialize( ser );
    }
    ( keys.end()-1 )->Serialize( ser );
ser.ExitChild();
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
        return new BezierEvaluator< TimeValueT, ValueT >( k1, k2, Key< TimeValueT, ValueT >( 0, ValueT() ), Key< TimeValueT, ValueT >( 0, ValueT() ), tolerance );
    else if( type == CurveType::COSINE_LIKE )
        return new BezierEvaluator< TimeValueT, ValueT >( k1, k2, Key< TimeValueT, ValueT >( 0, ValueT() ), Key< TimeValueT, ValueT >( 0, ValueT() ), tolerance );
    else
    {
        assert( false );
        return nullptr;
    }
}

class Interpolation : ISerializable
{
public:
    virtual void                Serialize       ( ISerializer& ) const { assert( false ); }
    static ISerializablePtr     Create          ( const IDeserializer& deser )
    {
        auto type = deser.GetAttribute( "type" );
        if( type == "point" )
            return ISerializablePtr( CreateDummyInterpolator<float,float>( CurveType::POINT, Key<float,float>(0,0), Key<float,float>(0,0), 0 ) );
        else if( type == "linear" )
            return ISerializablePtr( CreateDummyInterpolator<float,float>( CurveType::LINEAR, Key<float,float>(0,0), Key<float,float>(0,0), 0 ) );
        else if( type == "bezier" )
            return ISerializablePtr( CreateDummyInterpolator<float,float>( CurveType::BEZIER, Key<float,float>(0,0), Key<float,float>(0,0), 0 ) );
        return nullptr;
    }
};

// *************************************
//
template< class TimeValueT, class ValueT >
ISerializablePtr     CompositeBezierInterpolator< TimeValueT, ValueT >::Create          ( const IDeserializer& deser )
{
    auto interpolator = std::make_shared< CompositeBezierInterpolator< TimeValueT, ValueT > >();

    interpolator->SetCurveType( SerializationHelper::String2T< CurveType >( ct2s, deser.GetAttribute( "curve_type" ) ) );

    auto keys = SerializationHelper::DeserializeObjectLoadPropertiesImpl< Key >( deser, "key" );
    auto interpolators = SerializationHelper::DeserializeObjectLoadPropertiesImpl< Interpolation >( deser, "interpolation" );
    for( auto key : keys )
        interpolator->AddKey( key->t, key->val );

    return interpolator;
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
        TimeValueT length = be->key2.t - be->key1.t;
        be->v1 = Key( scale * length, ValueT() );
        be->v2 = Key( -scale * length, ValueT() );
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
template< class TimeValueT, class ValueT >
void CompositeBezierInterpolator< TimeValueT, ValueT >::AddKey             ( TimeValueT t, const ValueT & v ) 
{ 
    if( keys.empty() )
    {
        keys.push_back( Key( t, v ) );
        return;
    }

// find the proper key
    static_assert(std::numeric_limits<float>::is_iec559, "IEEE 754 required");
    Key left( -std::numeric_limits< float >::infinity(), ValueT() );
    Key right = keys.front();
    size_t i = 0, last = keys.size()-1;

    while( t > right.t+m_tolerance )
    {
        left = right;
        if( i++ == last )
        {
            right = Key( std::numeric_limits< float >::infinity(), ValueT() );
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
    for( int j = int( i-2 ); j <= int( i+1 ); j++ )
        if( j >= 0 && j < interpolators.size() )
            UpdateInterpolator( interpolators, j, m_type );
}


// *******************************
//
template< class TimeValueT, class ValueT >
ValueT CompositeBezierInterpolator< TimeValueT, ValueT >::PreEvaluate( TimeValueT t ) const 
{ 
    TimeValueT tStart = keys.front().t;
    TimeValueT tEnd = keys.back().t;

    auto interval = tEnd - tStart;
    if( interval <= m_tolerance )
        return Evaluate( tStart );

    t = t - tStart;

    if( m_preMethod == WrapMethod::clamp )
        return Evaluate( tStart );
    else if( m_preMethod == WrapMethod::repeat )
    {
        TimeValueT q = interval;
        TimeValueT r = std::modf( t, &q );
        return Evaluate( tStart + r );
    }
    else if ( m_preMethod == WrapMethod::pingPong )
    {
        TimeValueT q = interval;
        TimeValueT r = std::modf( t, &q );

        if( round( q ) % 2 == 0 )
        {
            return Evaluate( tStart + r );
        }
        else
        {
            return Evaluate( tStart + interval - r );
        }
    }

    return Evaluate( t );
}

// *******************************
//
template< class TimeValueT, class ValueT >
ValueT CompositeBezierInterpolator< TimeValueT, ValueT >::PostEvaluate( TimeValueT t ) const 
{ 
    TimeValueT tStart = keys.front().t;
    TimeValueT tEnd = keys.back().t;

    auto interval = tEnd - tStart;
    if( interval <= m_tolerance )
        return Evaluate( tEnd );

    t = t - tStart;

    if( m_postMethod == WrapMethod::clamp )
        //return Evaluate( tEnd );
        return keys.back().val; // FIXME(?)
    else if( m_postMethod == WrapMethod::repeat )
    {
        TimeValueT q = interval;
        TimeValueT r = divmod( t, &q );
        return Evaluate( tStart + r );
    }
    else if( m_postMethod == WrapMethod::pingPong )
    {
        TimeValueT q = interval;
        TimeValueT r = divmod(t, &q);

        if( round( q ) % 2 == 0 )
        {
            return Evaluate( tStart + r );
        }
        else
        {
            return Evaluate( tStart + interval - r );
        }
    }

    return Evaluate( t );
}

// *******************************
//
template< class TimeValueT, class ValueT >
ValueT CompositeBezierInterpolator< TimeValueT, ValueT >::Evaluate         ( TimeValueT t ) const 
{ 
    if( keys.size() == 0 )
    {
        assert( false ); // FIXME: error handling FTW
        return ValueT();
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
template< class TimeValueT, class ValueT >
const std::vector< Key< TimeValueT, ValueT > > &                          CompositeBezierInterpolator< TimeValueT, ValueT >::GetKeys()
{
    return keys;
}

// *******************************
//
template< class TimeValueT, class ValueT >
const std::vector< IEvaluator< TimeValueT, ValueT >* > &                  CompositeBezierInterpolator< TimeValueT, ValueT >::GetInterpolators()
{
    return interpolators;
}

// *******************************
//
template< class TimeValueT, class ValueT >
void                                                CompositeBezierInterpolator< TimeValueT, ValueT >::SetCurveType( CurveType type )
{
    m_type = type;
}

// *******************************
//
template< class TimeValueT, class ValueT >
void                                                CompositeBezierInterpolator< TimeValueT, ValueT >::SetWrapPostMethod  ( WrapMethod method )
{
    m_postMethod = method;
}

// *******************************
//
template< class TimeValueT, class ValueT >
void                                                CompositeBezierInterpolator< TimeValueT, ValueT >::SetWrapPreMethod   ( WrapMethod method )
{
    m_preMethod = method;
}

// *******************************
//
template< class TimeValueT, class ValueT >
void                                                CompositeBezierInterpolator< TimeValueT, ValueT >::SetKey1( int i, Key key )
{
    assert( interpolators[ i ]->GetType() == EvaluatorType::BEZIER );
    ( ( BezierEvaluator< TimeValueT, ValueT >* ) interpolators[ i ] )->key1 = key;
}

// *******************************
//
template< class TimeValueT, class ValueT >
void                                                CompositeBezierInterpolator< TimeValueT, ValueT >::SetKey2( int i, Key key )
{
    assert( interpolators[ i ]->GetType() == EvaluatorType::BEZIER );
    ( ( BezierEvaluator< TimeValueT, ValueT >* ) interpolators[ i ] )->key2 = key;
}

// *******************************
//
template< class TimeValueT, class ValueT >
void                                                CompositeBezierInterpolator< TimeValueT, ValueT >::SetV1( int i, Key v )
{
    assert( interpolators[ i ]->GetType() == EvaluatorType::BEZIER );
    ( ( BezierEvaluator< TimeValueT, ValueT >* ) interpolators[ i ] )->v1 = v;
}

// *******************************
//
template< class TimeValueT, class ValueT >
void                                                CompositeBezierInterpolator< TimeValueT, ValueT >::SetV2( int i, Key v )
{
    assert( interpolators[ i ]->GetType() == EvaluatorType::BEZIER );
    ( ( BezierEvaluator< TimeValueT, ValueT >* ) interpolators[ i ] )->v2 = v;
}

template class CompositeBezierInterpolator<TimeType, TimeType>;
template class CompositeBezierInterpolator<TimeType, bool>;
template class CompositeBezierInterpolator<TimeType, int>;
template class CompositeBezierInterpolator<TimeType, float>;

template class CompositeBezierInterpolator<TimeType, glm::vec2>;
template class CompositeBezierInterpolator<TimeType, glm::vec3>;
template class CompositeBezierInterpolator<TimeType, glm::vec4>;

} // bv
