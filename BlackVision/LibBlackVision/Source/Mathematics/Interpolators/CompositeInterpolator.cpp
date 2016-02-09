#include "stdafx.h"

#include "CompositeInterpolator.h"

#include "Functions/ConstFunction.h"
#include "Functions/LinearFunction.h"
#include "Functions/BezierFunction.h"
#include "Functions/PolynomialFunction.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"

#include "Mathematics/Core/mathfuncs.h"
#include <vector>
#include <array>
//#include <initializer_list>

namespace bv {

namespace SerializationHelper {

std::pair< WrapMethod, const char* > wm2s[] =
{ std::make_pair( WrapMethod::clamp, "clamp" )
, std::make_pair( WrapMethod::pingPong, "pingPong" )
, std::make_pair( WrapMethod::repeat, "repeat" )
, std::make_pair( WrapMethod::clamp, "" ) };

template<> std::string T2String< WrapMethod >( const WrapMethod& wm )       { return Enum2String( wm2s, wm ); }
template<> WrapMethod String2T( const std::string & s, const WrapMethod& default )
{
    if( s == "" ) 
        return default; 
    else 
        return String2Enum( wm2s, s );
}

std::pair< CurveType, const char* > ct2s[] = 
    { std::make_pair( CurveType::CT_BEZIER, "bezier" )
    , std::make_pair( CurveType::CT_COSINE_LIKE, "cosine" ) 
    , std::make_pair( CurveType::CT_LINEAR, "linear" ) 
    , std::make_pair( CurveType::CT_POINT, "point" ) 
    , std::make_pair( CurveType::CT_CUBIC_IN, "cubic_in" )
    , std::make_pair( CurveType::CT_CUBIC_OUT, "cubic_out" )
    , std::make_pair( CurveType::CT_ELASTIC_IN, "elastic_in" )
    , std::make_pair( CurveType::CT_ELASTIC_OUT, "elastic_out" )
    , std::make_pair( CurveType::CT_ELASTIC_IN_BOUNCE, "elastic_in_bounce" )
    , std::make_pair( CurveType::CT_ELASTIC_OUT_BOUNCE, "elastic_out_bounce" )
    , std::make_pair( CurveType::CT_QUARTIC_INOUT, "quadric_inout" )
    , std::make_pair( CurveType::CT_CUBIC_INTOUT, "cubic_inout" )
    , std::make_pair( CurveType::CT_BEZIER, "" )        // default
};


template<> std::string T2String< CurveType >( const CurveType& ct )
{
    return Enum2String( ct2s, ct );
}

template<> CurveType String2T( const std::string & s, const CurveType& default )
{
    if( s == "" ) 
        return default; 
    else 
        return String2Enum( ct2s, s );
}

}

// *******************************
//
template< class TimeValueT, class ValueT >
CompositeInterpolator< TimeValueT, ValueT >::CompositeInterpolator( float tolerance )
    : m_type( CurveType::CT_LINEAR )
    , m_tolerance( tolerance )
    , m_preMethod( WrapMethod::clamp ), m_postMethod( WrapMethod::clamp )
{
}

// *******************************
//
template< class TimeValueT, class ValueT >
CompositeInterpolator< TimeValueT, ValueT >::CompositeInterpolator( const CompositeInterpolator& that )
{ 
    keys = that.keys; 
    interpolators = that.interpolators; 
    m_tolerance = that.m_tolerance; 
    m_type = that.m_type; 
    m_preMethod = that.m_preMethod;
    m_postMethod = that.m_postMethod;
}

// *************************************
//
template< class TimeValueT, class ValueT >
void                                        CompositeInterpolator< TimeValueT, ValueT >::Serialize       ( ISerializer& ser ) const
{
    ser.EnterChild( "interpolator" );

        ser.SetAttribute( "curve_type", SerializationHelper::Enum2String< CurveType >( SerializationHelper::ct2s, m_type ) );
        SerializationHelper::SerializeAttribute( ser, m_preMethod, "preMethod" );
        SerializationHelper::SerializeAttribute( ser, m_postMethod, "postMethod" );

        ser.EnterArray( "keys" );
            for( size_t i = 0; i < keys.size(); i++ )
                keys[ i ].Serialize( ser );
        ser.ExitChild();

        ser.EnterArray( "interpolations" );
            for( size_t i = 0; i < interpolators.size(); i++ )
                interpolators[ i ]->Serialize( ser );
        ser.ExitChild();

    ser.ExitChild();
}

// *************************************
//
template< class TimeValueT, class ValueT >
CompositeInterpolator< TimeValueT, ValueT >*     CompositeInterpolator< TimeValueT, ValueT >::Create          ( const IDeserializer& deser )
{
    auto interpolator = new CompositeInterpolator< TimeValueT, ValueT >();

    auto keys = SerializationHelper::DeserializeArray< Key >( deser, "keys" );

    if( keys.size() == 1 || deser.EnterChild( "interpolations" ) == false )
        for( auto key : keys ) // no interpolation types
            interpolator->AddKey( key->t, key->val );
    else
    {
        if( deser.EnterChild( "interpolation" ) )
		{
			for( auto key : keys )
			{
				interpolator->AddKey( key->t, key->val );
				if( key != keys.back() )
				{
					interpolator->SetAddedKeyCurveType( SerializationHelper::String2Enum< CurveType >( SerializationHelper::ct2s, deser.GetAttribute( "type" ) ) );
					if( deser.NextChild() == false )
						if( key == keys.end()[-2] ) // everything is OK, this is the end, we need to go out
							deser.ExitChild();
						else // we've got malformed XML
						{
							assert( false ); // FIXME: error handling
							return nullptr;
						}
				}
			}

			deser.EnterChild( "interpolation" );
			size_t i = 0;
			do
			{
				auto interpolators = interpolator->GetInterpolators();
				interpolators[ i++ ]->Deserialize( deser );
			} while( deser.NextChild() );
			deser.ExitChild(); // exit "interpolation"
		}
		else
		{
			for( auto key : keys ) // no interpolation types
				interpolator->AddKey( key->t, key->val );
		}
        deser.ExitChild(); // exit "interpolations"
    }

    interpolator->SetAddedKeyCurveType( SerializationHelper::String2Enum< CurveType >( SerializationHelper::ct2s, deser.GetAttribute( "curve_type" ) ) );
    interpolator->SetWrapPreMethod( SerializationHelper::String2Enum< WrapMethod >( SerializationHelper::wm2s, deser.GetAttribute( "preMethod" ) ) );
    interpolator->SetWrapPostMethod( SerializationHelper::String2Enum< WrapMethod >( SerializationHelper::wm2s, deser.GetAttribute( "postMethod" ) ) );

    assert( interpolator->GetNumKeys() > 0 );

    return interpolator;
}

// *******************************
//
template< class TimeValueT, class ValueT >
void UpdateInterpolator( std::vector< IEvaluator<TimeValueT, ValueT > * > & interpolators, size_t i, CurveType cType )
{
    typedef Key< TimeValueT, ValueT > Key;

    const float scale = 0.2f;

    auto iType = interpolators[ i ]->GetType();

    if( iType == EvaluatorType::ET_CONSTANT || iType == EvaluatorType::ET_LINEAR || iType == EvaluatorType::ET_POLYNOMIAL )
        return;

    assert( iType == EvaluatorType::ET_BEZIER );
    auto be = ( BezierEvaluator< TimeValueT, ValueT >* ) interpolators[ i ];

    if( cType == CurveType::CT_COSINE_LIKE )
    {
        TimeValueT length = be->key2.t - be->key1.t;
        be->v1 = Key( scale * length, ValueT() );
        be->v2 = Key( -scale * length, ValueT() );
    }
    else if( cType == CurveType::CT_BEZIER )
    {
        auto prevE = ( i == 0 ) ? interpolators[ 0 ] : interpolators[ i-1 ];
        auto nextE = ( i == interpolators.size()-1 ) ? interpolators.back() : interpolators[ i+1 ];

        auto prevBE = ( BezierEvaluator< TimeValueT, ValueT >* ) prevE;
        auto nextBE = ( BezierEvaluator< TimeValueT, ValueT >* ) nextE;

        be->v1 = scale * ( be->key2 - prevBE->key1 );
        be->v2 = -scale * ( nextBE->key2 - be->key1 );
    }
    else
    {
        assert( false );
    }
}

// *******************************
//
template<>
void UpdateInterpolator< TimeType, std::string >( std::vector< IEvaluator< TimeType, std::string > * > & , size_t, CurveType )
{}

// *******************************
//
template<>
void UpdateInterpolator< TimeType, std::wstring >( std::vector< IEvaluator< TimeType, std::wstring > * > &, size_t, CurveType )
{}

// *******************************
//
template< class TimeValueT, class ValueT >
IEvaluator<TimeValueT, ValueT > * CreateDummyInterpolator( CurveType type, Key< TimeValueT, ValueT > k1, Key< TimeValueT, ValueT > k2, TimeValueT tolerance ) // FIXME maybe
{
    if( type == CurveType::CT_POINT )
        return new ConstEvaluator< TimeValueT, ValueT >( k1.val );
    else if( type == CurveType::CT_LINEAR )
        return new LinearEvaluator< TimeValueT, ValueT >( k1, k2, tolerance );
    else if( type == CurveType::CT_BEZIER )
        return new BezierEvaluator< TimeValueT, ValueT >( k1, k2, Key< TimeValueT, ValueT >( 0, ValueT() ), Key< TimeValueT, ValueT >( 0, ValueT() ), tolerance );
    else if( type == CurveType::CT_COSINE_LIKE )
        return new BezierEvaluator< TimeValueT, ValueT >( k1, k2, Key< TimeValueT, ValueT >( 0, ValueT() ), Key< TimeValueT, ValueT >( 0, ValueT() ), tolerance );
    else if( type == CurveType::CT_CUBIC_IN )
        return new PolynomialEvaluator< TimeValueT, ValueT >( k1, k2, 0, 0, 1, 0, 0, tolerance );
    else if( type == CurveType::CT_CUBIC_OUT )
        return new PolynomialEvaluator< TimeValueT, ValueT >( k1, k2, 0, 0, 1, -3, 3, tolerance );
    else if( type == CurveType::CT_ELASTIC_IN )
        return new PolynomialEvaluator< TimeValueT, ValueT >( k1, k2, 33, -59, 32, -5, 0, tolerance );
    else if( type == CurveType::CT_ELASTIC_OUT )
        return new PolynomialEvaluator< TimeValueT, ValueT >( k1, k2, 33, -106, 126, -67, 15, tolerance );
    //else if( type == CT_CUBIC_IN_BOUNCE )
    //    return new PolynomialEvaluator< TimeValueT, ValueT >( k1, k2, 0, 0, 1, 0, 0, true );
    //else if( type == CT_CUBIC_OUT_BOUNCE )
    //    return new PolynomialEvaluator< TimeValueT, ValueT >( k1, k2, 0, 0, 1, -3, 3, true );
    else if( type == CT_ELASTIC_IN_BOUNCE )
        return new PolynomialEvaluator< TimeValueT, ValueT >( k1, k2, 33, -59, 32, -5, 0, tolerance, true );
    else if( type == CT_ELASTIC_OUT_BOUNCE )
        return new PolynomialEvaluator< TimeValueT, ValueT >( k1, k2, 33, -106, 126, -67, 15, tolerance, true );
    else if( type == CT_QUARTIC_INOUT )
        return new PolynomialEvaluator< TimeValueT, ValueT >( k1, k2, 6, -15, 10, 0, 0, tolerance );
    else if( type == CT_CUBIC_INTOUT )
        return new PolynomialEvaluator< TimeValueT, ValueT >( k1, k2, 0, 0, -2, 3, 0, tolerance );
    else
    {
        assert( false );
        return nullptr;
    }
}

// *******************************
//
template<>
IEvaluator< TimeType, std::wstring > * CreateDummyInterpolator( CurveType type, Key< TimeType, std::wstring  > k1, Key< TimeType, std::wstring  >, TimeType ) // FIXME maybe
{
    if( type == CurveType::CT_POINT )
    {
        return new ConstEvaluator< TimeType, std::wstring >( k1.val );
    }
    else
    {
        assert( false );
        return nullptr;
    }
}

// *******************************
//
template<>
IEvaluator< TimeType, std::string > * CreateDummyInterpolator( CurveType type, Key< TimeType, std::string  > k1, Key< TimeType, std::string  >, TimeType ) // FIXME maybe
{
    if( type == CurveType::CT_POINT )
    {
        return new ConstEvaluator< TimeType, std::string >( k1.val );
    }
    else
    {
        assert( false );
        return nullptr;
    }
}
// *******************************
//
template< class TimeValueT, class ValueT >
void CompositeInterpolator< TimeValueT, ValueT >::AddKey             ( TimeValueT t, const ValueT & v ) 
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
        {
            right = keys[ i ];
        }
    }

    assert( left.t <= t && t <= right.t );
    bool isSame = fabs( t - right.t ) < m_tolerance;

    if( isSame )
    {
        keys[ i ].val = v;

        if( i != 0 )
        {
            interpolators[ i-1 ]->SetValue( t, v );
        }
        if( i != last )
        {
            interpolators[ i ]->SetValue( t, v );
        }
    }
    else
    {
        auto key = Key( t, v );
        keys.insert( keys.begin() + i, key );

        if( i == 0 )
        {
            interpolators.insert( interpolators.begin(), CreateDummyInterpolator( m_type, key, right, m_tolerance ) );
        }
        else if( i == last+1 )
        {
            interpolators.push_back( CreateDummyInterpolator( m_type, left, key, m_tolerance ) );
        }
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

// ***********************
//
template< class TimeValueT, class ValueT >
bool CompositeInterpolator< TimeValueT, ValueT >::RemoveKey       ( TimeValueT t )
{
    if( keys.size() <= 1 )
        return false;

    // Find the proper key
    SizeType i = 0;
    SizeType keysSize = keys.size();
    for( ; i < keysSize; ++i )
    {
        if( fabs( keys[ i ].t - t ) < m_tolerance )
            break;
    }
    if( i == keysSize )
        return false;       // Key not found.


    if( i == 0 )
        interpolators.erase( interpolators.begin() );
    else if( i == keysSize - 1 )
        interpolators.erase( interpolators.begin() + ( i - 1 ) );
    else
    {
        // Erase pair of interpolators touching keys
        interpolators.erase( interpolators.begin() + ( i - 1 ) );
        interpolators.erase( interpolators.begin() + ( i - 1 ) );

        // Insert new interpolator instead
        interpolators.insert( interpolators.begin() + ( i - 1 ), CreateDummyInterpolator( m_type, keys[ i - 1 ], keys[ i + 1 ], m_tolerance ) );
    }

    keys.erase( keys.begin() + i );

// update interpolators
    for( SizeType j = int( i-2 ); j <= int( i+1 ); j++ )
        if( j >= 0 && j < interpolators.size() )
            UpdateInterpolator( interpolators, j, m_type );

    return true;
}


// ***********************
//
template< class TimeValueT, class ValueT >
bool CompositeInterpolator< TimeValueT, ValueT >::MoveKey             ( TimeValueT t, TimeValueT newTime )
{
    // Find key to move
    SizeType index = std::numeric_limits<SizeType>::max();
    for( SizeType i = 0; i < keys.size(); ++i )
    {
        if( std::fabs( keys[ i ].t - t ) <= m_tolerance )
        {
            index = i;
            break;
        }
    }

    // Key to move not found.
    if( index == std::numeric_limits<SizeType>::max() )
        return false;

    // Check newTime. If key under this time exists, return.
    for( SizeType i = 0; i < keys.size(); ++i )
    {
        if( std::fabs( keys[ i ].t - newTime ) <= m_tolerance )
        {// Key already exists. Don't move.
            return false;
        }
    }

    AddKey( newTime, keys[ index ].val );
    RemoveKey( t );
    
    return true;
}


// *******************************
//
template< class TimeValueT, class ValueT >
ValueT CompositeInterpolator< TimeValueT, ValueT >::PreEvaluate( TimeValueT t ) const 
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
ValueT CompositeInterpolator< TimeValueT, ValueT >::PostEvaluate( TimeValueT t ) const 
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
ValueT CompositeInterpolator< TimeValueT, ValueT >::Evaluate         ( TimeValueT t ) const 
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

    SizeType i = 0;
    while( t > keys[ i+1 ].t && i < keys.size()-2 )
        i++;

    if( t > keys[ i+1 ].t )
        return PostEvaluate( t );

    return interpolators[ i ]->Evaluate( t );

}

// *******************************
//
template< class TimeValueT, class ValueT >
int                          CompositeInterpolator< TimeValueT, ValueT >::GetNumKeys()
{
    return (int) keys.size();
}

// *******************************
//
template< class TimeValueT, class ValueT >
const std::vector< Key< TimeValueT, ValueT > > &                          CompositeInterpolator< TimeValueT, ValueT >::GetKeys() const
{
    return keys;
}

// *******************************
//
template< class TimeValueT, class ValueT >
std::vector< Key< TimeValueT, ValueT > > &                                CompositeInterpolator< TimeValueT, ValueT >::GetKeys()
{
    return keys;
}

// *******************************
//
template< class TimeValueT, class ValueT >
const std::vector< IEvaluator< TimeValueT, ValueT >* > &                  CompositeInterpolator< TimeValueT, ValueT >::GetInterpolators()
{
    return interpolators;
}

// *******************************
//
template< class TimeValueT, class ValueT >
void                                                CompositeInterpolator< TimeValueT, ValueT >::SetGlobalCurveType( CurveType type )
{
    if( keys.size() == 0 )
    {
        assert( false && "Interpolators with no keys are evil" );
        return;
    }

    interpolators.clear();
    auto prevKey = keys.begin();
    auto nextKey = prevKey + 1;
    while( nextKey != keys.end() )
    {
        interpolators.push_back( CreateDummyInterpolator( type, *prevKey, *nextKey, m_tolerance ) );
        prevKey++;
        nextKey++;
    }

    for( int i = 0; i < interpolators.size(); i++ )
        UpdateInterpolator( interpolators, i, type );

    SetAddedKeyCurveType( type );
}

// *******************************
//
template< class TimeValueT, class ValueT >
void                                                CompositeInterpolator< TimeValueT, ValueT >::SetAddedKeyCurveType( CurveType type )
{
    m_type = type;
}

// *******************************
//
template< class TimeValueT, class ValueT >
CurveType                                           CompositeInterpolator< TimeValueT, ValueT >::GetCurveType    ()
{
    return m_type;
}

// *******************************
//
template< class TimeValueT, class ValueT >
void                                                CompositeInterpolator< TimeValueT, ValueT >::SetWrapPostMethod  ( WrapMethod method )
{
    m_postMethod = method;
}

// *******************************
//
template< class TimeValueT, class ValueT >
void                                                CompositeInterpolator< TimeValueT, ValueT >::SetWrapPreMethod   ( WrapMethod method )
{
    m_preMethod = method;
}

// *******************************
//
template< class TimeValueT, class ValueT >
WrapMethod                                          CompositeInterpolator< TimeValueT, ValueT >::GetWrapPostMethod  ()
{
    return m_postMethod;
}

// *******************************
//
template< class TimeValueT, class ValueT >
WrapMethod                                          CompositeInterpolator< TimeValueT, ValueT >::GetWrapPreMethod   ()
{
    return m_preMethod;
}

// *******************************
//
template<>
CompositeInterpolator< bv::TimeType, std::string >::CompositeInterpolator( float tolerance )
    : m_type( CurveType::CT_POINT )
    , m_tolerance( tolerance )
    , m_preMethod( WrapMethod::clamp ), m_postMethod( WrapMethod::clamp )
{
}

// *******************************
//
template<>
CompositeInterpolator< bv::TimeType, std::wstring >::CompositeInterpolator( float tolerance )
    : m_type( CurveType::CT_POINT )
    , m_tolerance( tolerance )
    , m_preMethod( WrapMethod::clamp ), m_postMethod( WrapMethod::clamp )
{
}

template class CompositeInterpolator<TimeType, TimeType>;
template class CompositeInterpolator<TimeType, bool>;
template class CompositeInterpolator<TimeType, int>;
template class CompositeInterpolator<TimeType, float>;

template class CompositeInterpolator<TimeType, glm::vec2>;
template class CompositeInterpolator<TimeType, glm::vec3>;
template class CompositeInterpolator<TimeType, glm::vec4>;

template class CompositeInterpolator< TimeType, std::string >;
template class CompositeInterpolator< TimeType, std::wstring >;

} // bv
