#pragma once

#include "CompositeInterpolator.h"

#include "InterpolatorBasicTypes.h"

#include "Functions/ConstFunction.h"
#include "Functions/LinearFunction.h"
#include "Functions/BezierFunction.h"
#include "Functions/PolynomialFunction.h"

#include "CompositeInterpolatorSerializationHelper.h"

#include "Serialization/BV/BVSerializeContext.h"
#include "Exceptions/SerializationException.h"

#include "Mathematics/Core/MathFuncs.h"

#include <vector>
#include <array>
#include <algorithm>
//#include <initializer_list>

#include "Mathematics/glm_inc.h"

#include "Memory/MemoryLeaks.h"



namespace bv {

// *******************************
//
template< class TimeValueT, class ValueT >
inline CompositeInterpolator< TimeValueT, ValueT >::CompositeInterpolator( float tolerance )
    : m_type( CurveType::CT_LINEAR )
    , m_tolerance( tolerance )
    , m_preMethod( WrapMethod::clamp ), m_postMethod( WrapMethod::clamp )
{
}

// *******************************
//
template< class TimeValueT, class ValueT >
inline CompositeInterpolator< TimeValueT, ValueT >::CompositeInterpolator( const CompositeInterpolator& that )
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
inline void                                        CompositeInterpolator< TimeValueT, ValueT >::Serialize       ( ISerializer& ser ) const
{
    auto context = static_cast<BVSerializeContext*>( ser.GetSerializeContext() );

    ser.EnterChild( "interpolator" );

        if( context->detailedInfo )
        {
            ser.SetAttribute( "curve_type", SerializationHelper::T2String< CurveType >( m_type ) );
            SerializationHelper::SerializeAttribute( ser, m_preMethod, "preMethod" );
            SerializationHelper::SerializeAttribute( ser, m_postMethod, "postMethod" );
        }

        ser.EnterArray( "keys" );
            for( size_t i = 0; i < keys.size(); i++ )
                keys[ i ].Serialize( ser );
        ser.ExitChild();

        if( context->detailedInfo )
        {
            ser.EnterArray( "interpolations" );
                for( size_t i = 0; i < interpolators.size(); i++ )
                    interpolators[ i ]->Serialize( ser );
            ser.ExitChild();
        }

    ser.ExitChild();
}

// *************************************
//
template< class TimeValueT, class ValueT >
std::shared_ptr< CompositeInterpolator< TimeValueT, ValueT > >
inline CompositeInterpolator< TimeValueT, ValueT >::Create              ( float tolerance )
{
    return std::shared_ptr< CompositeInterpolator< TimeValueT, ValueT > >( new CompositeInterpolator< TimeValueT, ValueT >( tolerance ) );
}

// *************************************
//
template< class TimeValueT, class ValueT >
inline std::shared_ptr< CompositeInterpolator< TimeValueT, ValueT > >     CompositeInterpolator< TimeValueT, ValueT >::Create          ( const IDeserializer& deser )
{
    //auto ctx = Cast< BVDeserializeContext * >( deser.GetDeserializeContext() );

    auto interpolator = CompositeInterpolator< TimeValueT, ValueT >::Create();
    auto keys = SerializationHelper::DeserializeArray< Key >( deser, "keys" );

    //std::stable_sort( keys.negin(), keys.end() );

    //if( keys.size() > 0 )
    //{
    //    interpolator->AddKey( keys[ 0 ]->t, keys[ 0 ]->val );

    //    if( keys.size() > 1 )
    //    {
    //        if( deser.EnterChild( "interpolations" ) )
    //        {
    //            bool existsEvaluator = true;

    //            if( deser.EnterChild( "interpolation" ) )
    //            {
    //                for( SizeType i = 1; i < keys.size(); ++i )
    //                {
    //                    CurveType curveType = SerializationHelper::String2T< CurveType >( deser.GetAttribute( "type" ), CurveType::CT_LINEAR );

    //                    interpolator->SetAddedKeyCurveType( curveType );
    //                    interpolator->AddKey( keys[ i ]->t, keys[ i ]->val );
    //                    
    //                    auto & addedKeys = interpolator->GetKeys();
    //                    auto posIter = std::find( addedKeys.begin(), addedKeys.end(), keys[ i ] );
    //                    auto evalIdx = std::distance( addedKeys.begin(), posIter );

    //                    interpolator->interpolators[ evalIdx ]->Deserialize( deser );

    //                    deser.NextChild();
    //                }

    //                deser.ExitChild();  // interpolation
    //            }

    //            deser.ExitChild();  // interpolations
    //        }
    //        else
    //        {
    //            ctx->AddWarning< SerializationException >( "No interpolation marker found in xml. Evaluators for keys will have default types." );


    //        }
    //    }
    //}

    //interpolator->SetAddedKeyCurveType( SerializationHelper::String2T< CurveType >( deser.GetAttribute( "curve_type" ), CurveType::CT_LINEAR ) );
    //interpolator->SetWrapPreMethod( SerializationHelper::String2T< WrapMethod >( deser.GetAttribute( "preMethod" ), WrapMethod::clamp ) );
    //interpolator->SetWrapPostMethod( SerializationHelper::String2T< WrapMethod >( deser.GetAttribute( "postMethod" ), WrapMethod::clamp ) );

    //return interpolator;

    // ***********************
    // Old code

    if( keys.size() == 1 || deser.EnterChild( "interpolations" ) == false )
    {
        for( auto key : keys ) // no interpolation types
            interpolator->AddKey( key->t, key->val );
    }
    else
    {
        if( deser.EnterChild( "interpolation" ) )
        {
            for( auto key : keys )
            {
                interpolator->AddKey( key->t, key->val );
                if( key != keys.back() )
                {
                    interpolator->SetAddedKeyCurveType( SerializationHelper::String2T< CurveType >( deser.GetAttribute( "type" ), CurveType::CT_LINEAR ) );
                    if( deser.NextChild() == false )
                    {
                        if( key == keys.end()[ -2 ] ) // everything is OK, this is the end, we need to go out
                            deser.ExitChild();
                        else // we've got malformed XML
                        {
                            // DOUBLE FIXME: error handling
                            // assert( false ); // FIXME: error handling
                            return nullptr;
                        }
                    }
                }
            }

            deser.EnterChild( "interpolation" );
            size_t i = 0;
            do
            {
				if( interpolator->interpolators.size() > i )
				{
					interpolator->interpolators[ i++ ]->Deserialize( deser );
				}
				else
				{
					LOG_MESSAGE( SeverityLevel::error ) << "Wrong number of interpolators for keys.";
				}
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

    interpolator->SetAddedKeyCurveType( SerializationHelper::String2T< CurveType >( deser.GetAttribute( "curve_type" ), CurveType::CT_LINEAR ) );
    interpolator->SetWrapPreMethod( SerializationHelper::String2T< WrapMethod >( deser.GetAttribute( "preMethod" ), WrapMethod::clamp ) );
    interpolator->SetWrapPostMethod( SerializationHelper::String2T< WrapMethod >( deser.GetAttribute( "postMethod" ), WrapMethod::clamp ) );
    
    // FIXME: This code is written instead of assert here. Make proper error handling.
    //if( interpolator->GetNumKeys() == 0 )
    //    return nullptr;

    return interpolator;
}

// ***********************
//
template< class TimeValueT, class ValueT >
inline std::vector< CurveType >         CompositeInterpolator< TimeValueT, ValueT >::DeserializeCurves      ( const IDeserializer & deser )
{
    auto ctx = Cast< BVDeserializeContext * >( deser.GetDeserializeContext() );

    std::vector< CurveType > curves;

    SizeType numEvaluators = 0;

    if( deser.EnterChild( "interpolations" ) )
    {
        if( deser.EnterChild( "interpolation" ) )
        {
            do
            {
                auto curveType = SerializationHelper::String2T< CurveType >( deser.GetAttribute( "type" ) );

                if( curveType.IsValid() )
                    curves.push_back( curveType.GetVal() );
                else
                    ctx->AddWarning< SerializationException >( "Cannot deserialize interpolation under index [" + SerializationHelper::T2String( numEvaluators ) + "]. Interpolation ignored." );

                numEvaluators++;
            }
            while( deser.NextChild() );

            deser.ExitChild();  // interpolation
        }
        deser.ExitChild();  // interpolations
    }

    return curves;
}

// *******************************
//
template< class TimeValueT, class ValueT >
inline void UpdateInterpolator( std::vector< std::shared_ptr< IEvaluator<TimeValueT, ValueT > > > & interpolators, size_t i )
{
    typedef Key< TimeValueT, ValueT > Key;

    const float scale = 0.2f;

    auto iType = interpolators[ i ]->GetType();
    auto cType = interpolators[ i ]->GetCurveType();

    if( iType == EvaluatorType::ET_CONSTANT || iType == EvaluatorType::ET_LINEAR || iType == EvaluatorType::ET_POLYNOMIAL )
        return;

    assert( iType == EvaluatorType::ET_BEZIER );
    auto be = std::static_pointer_cast< BezierEvaluator< TimeValueT, ValueT > >( interpolators[ i ] );

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

        auto prevBE = std::static_pointer_cast< BezierEvaluator< TimeValueT, ValueT > >( prevE );
        auto nextBE = std::static_pointer_cast< BezierEvaluator< TimeValueT, ValueT > >( nextE );

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
inline void UpdateInterpolator< TimeType, std::string >( std::vector< std::shared_ptr< IEvaluator< TimeType, std::string > > > & , size_t )
{}

// *******************************
//
template<>
inline void UpdateInterpolator< TimeType, std::wstring >( std::vector< std::shared_ptr< IEvaluator< TimeType, std::wstring > > > &, size_t )
{}

// *******************************
//
template< class TimeValueT, class ValueT >
inline std::shared_ptr< IEvaluator< TimeValueT, ValueT > > CreateDummyInterpolator( CurveType type, Key< TimeValueT, ValueT > k1, Key< TimeValueT, ValueT > k2, TimeValueT tolerance ) // FIXME maybe
{
    if( type == CurveType::CT_POINT )
        return std::make_shared< ConstEvaluator< TimeValueT, ValueT > >(  k1.val );
    else if( type == CurveType::CT_LINEAR )
        return std::make_shared< LinearEvaluator< TimeValueT, ValueT > >( k1, k2, tolerance );
    else if( type == CurveType::CT_BEZIER )
        return std::make_shared< BezierEvaluator< TimeValueT, ValueT > >( type, k1, k2, Key< TimeValueT, ValueT >( 0, ValueT() ), Key< TimeValueT, ValueT >( 0, ValueT() ), tolerance );
    else if( type == CurveType::CT_COSINE_LIKE )
        return std::make_shared< BezierEvaluator< TimeValueT, ValueT > >( type, k1, k2, Key< TimeValueT, ValueT >( 0, ValueT() ), Key< TimeValueT, ValueT >( 0, ValueT() ), tolerance );
    else if( type == CurveType::CT_CUBIC_IN )
        return std::make_shared< PolynomialEvaluator< TimeValueT, ValueT > >( type, k1, k2, 0.f, 0.f, 1.f, 0.f, 0.f, tolerance, false );
    else if( type == CurveType::CT_CUBIC_OUT )
        return std::make_shared< PolynomialEvaluator< TimeValueT, ValueT > >( type, k1, k2, 0.f, 0.f, 1.f, -3.f, 3.f, tolerance, false );
    else if( type == CurveType::CT_ELASTIC_IN )
        return std::make_shared< PolynomialEvaluator< TimeValueT, ValueT > >( type, k1, k2, 33.f, -59.f, 32.f, -5.f, 0.f, tolerance, false );
    else if( type == CurveType::CT_ELASTIC_OUT )
        return std::make_shared< PolynomialEvaluator< TimeValueT, ValueT > >( type, k1, k2, 33.f, -106.f, 126.f, -67.f, 15.f, tolerance, false );
    //else if( type == CT_CUBIC_IN_BOUNCE )
    //    return new PolynomialEvaluator< TimeValueT, ValueT >( k1, k2, 0, 0, 1, 0, 0, true );
    //else if( type == CT_CUBIC_OUT_BOUNCE )
    //    return new PolynomialEvaluator< TimeValueT, ValueT >( k1, k2, 0, 0, 1, -3, 3, true );
    else if( type == CT_ELASTIC_IN_BOUNCE )
        return std::make_shared< PolynomialEvaluator< TimeValueT, ValueT > >( type, k1, k2, 33.f, -59.f, 32.f, -5.f, 0.f, tolerance, true );
    else if( type == CT_ELASTIC_OUT_BOUNCE )
        return std::make_shared< PolynomialEvaluator< TimeValueT, ValueT > >( type, k1, k2, 33.f, -106.f, 126.f, -67.f, 15.f, tolerance, true );
    else if( type == CT_QUARTIC_INOUT )
        return std::make_shared< PolynomialEvaluator< TimeValueT, ValueT > >( type, k1, k2, 6.f, -15.f, 10.f, 0.f, 0.f, tolerance, false );
    else if( type == CT_CUBIC_INTOUT )
        return std::make_shared< PolynomialEvaluator< TimeValueT, ValueT > >( type, k1, k2, 0.f, 0.f, -2.f, 3.f, 0.f, tolerance, false );
    else
    {
        assert( false );
        return nullptr;
    }
}

// *******************************
//
template<>
inline std::shared_ptr< IEvaluator< TimeType, std::wstring > > CreateDummyInterpolator( CurveType type, Key< TimeType, std::wstring  > k1, Key< TimeType, std::wstring  >, TimeType ) // FIXME maybe
{
    if( type == CurveType::CT_POINT )
    {
        return std::make_shared< ConstEvaluator< TimeType, std::wstring > >( k1.val );
    }
    else
    {
        //assert( false );
        return nullptr;
    }
}

// *******************************
//
template<>
inline std::shared_ptr< IEvaluator< TimeType, std::string > > CreateDummyInterpolator( CurveType type, Key< TimeType, std::string  > k1, Key< TimeType, std::string  >, TimeType ) // FIXME maybe
{
    if( type == CurveType::CT_POINT )
    {
        return std::make_shared< ConstEvaluator< TimeType, std::string > >( k1.val );
    }
    else
    {
        //assert( false );
        return nullptr;
    }
}
// *******************************
//
template< class TimeValueT, class ValueT >
inline void CompositeInterpolator< TimeValueT, ValueT >::AddKey             ( TimeValueT t, const ValueT & v ) 
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

    assert( left.t <= t && t <= right.t + m_tolerance );
    bool isSame = fabs( t - right.t ) < m_tolerance;

    if( isSame )
    {
        keys[ i ].val = v;

        if( i != 0 && interpolators[ i-1 ]->GetType() != EvaluatorType::ET_CONSTANT ) // FIXME: think about cleaner solution (perhaps inside of SetValue)
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
    for( auto j = ( UInt32 )( i-2 ); j <= ( UInt32 )( i+1 ); j++ )
        if( j >= 0 && j < ( UInt32 )interpolators.size() )
            UpdateInterpolator( interpolators, j );
}

// ***********************
//
template< class TimeValueT, class ValueT >
inline bool CompositeInterpolator< TimeValueT, ValueT >::RemoveKey       ( TimeValueT t )
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
            UpdateInterpolator( interpolators, j );

    return true;
}

// ***********************
//
template< class TimeValueT, class ValueT >
inline void CompositeInterpolator< TimeValueT, ValueT >::RemoveAllKeys     ()
{
    //FIXME: is this correct?
    interpolators.clear();
    keys.clear();
}

// ***********************
//
template< class TimeValueT, class ValueT >
inline bool CompositeInterpolator< TimeValueT, ValueT >::MoveKey             ( TimeValueT t, TimeValueT newTime )
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
inline int                          CompositeInterpolator< TimeValueT, ValueT >::GetNumKeys()
{
    return (int) keys.size();
}

// *******************************
//
template< class TimeValueT, class ValueT >
inline const std::vector< Key< TimeValueT, ValueT > > &                          CompositeInterpolator< TimeValueT, ValueT >::GetKeys() const
{
    return keys;
}

// *******************************
//
template< class TimeValueT, class ValueT >
inline std::vector< Key< TimeValueT, ValueT > > &                                CompositeInterpolator< TimeValueT, ValueT >::GetKeys()
{
    return keys;
}

// *******************************
//
template< class TimeValueT, class ValueT >
inline void                                                CompositeInterpolator< TimeValueT, ValueT >::SetGlobalCurveType( CurveType type )
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

    for( UInt32 i = 0; i < ( UInt32 )interpolators.size(); i++ )
        UpdateInterpolator( interpolators, i );

    SetAddedKeyCurveType( type );
}

// *******************************
//
template< class TimeValueT, class ValueT >
inline void                                                CompositeInterpolator< TimeValueT, ValueT >::SetAddedKeyCurveType( CurveType type )
{
    m_type = type;
}

// *******************************
//
template< class TimeValueT, class ValueT >
inline CurveType                                           CompositeInterpolator< TimeValueT, ValueT >::GetCurveType    ()
{
    return m_type;
}

// *******************************
//
template< class TimeValueT, class ValueT >
inline void                                                CompositeInterpolator< TimeValueT, ValueT >::SetWrapPostMethod  ( WrapMethod method )
{
    m_postMethod = method;
}

// *******************************
//
template< class TimeValueT, class ValueT >
inline void                                                CompositeInterpolator< TimeValueT, ValueT >::SetWrapPreMethod   ( WrapMethod method )
{
    m_preMethod = method;
}

// *******************************
//
template< class TimeValueT, class ValueT >
inline WrapMethod                                          CompositeInterpolator< TimeValueT, ValueT >::GetWrapPostMethod  ()
{
    return m_postMethod;
}

// *******************************
//
template< class TimeValueT, class ValueT >
inline WrapMethod                                          CompositeInterpolator< TimeValueT, ValueT >::GetWrapPreMethod   ()
{
    return m_preMethod;
}



// *******************************
//
template< class TimeValueT, class ValueT >
inline ValueT CompositeInterpolator< TimeValueT, ValueT >::PreEvaluate( TimeValueT t ) const
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
	else if( m_preMethod == WrapMethod::pingPong )
	{
		TimeValueT q = interval;
		TimeValueT r = std::modf( t, &q );

		if( mathematics::round( q ) % 2 == 0 )
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
inline ValueT CompositeInterpolator< TimeValueT, ValueT >::PostEvaluate( TimeValueT t ) const
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
		TimeValueT r = mathematics::divmod( t, &q );
		return Evaluate( tStart + r );
	}
	else if( m_postMethod == WrapMethod::pingPong )
	{
		TimeValueT q = interval;
		TimeValueT r = mathematics::divmod( t, &q );

		if( mathematics::round( q ) % 2 == 0 )
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
inline ValueT CompositeInterpolator< TimeValueT, ValueT >::Evaluate         ( TimeValueT t ) const
{
	auto size = keys.size();
	if( size == 0 )
	{
		return ValueT();
	}

	if( size == 1 )
		return keys[ 0 ].val;

	if( t < keys[ 0 ].t )
		return PreEvaluate( t );

	SizeType i = 0;
	while( t > keys[ i + 1 ].t && i < size - 2 )
		i++;

	if( t > keys[ i + 1 ].t )
		return PostEvaluate( t );

	return interpolators[ i ]->Evaluate( t );

}

// *******************************
//
template<>
inline CompositeInterpolator< bv::TimeType, std::string >::CompositeInterpolator( float tolerance )
	: m_type( CurveType::CT_POINT )
	, m_tolerance( tolerance )
	, m_preMethod( WrapMethod::clamp ), m_postMethod( WrapMethod::clamp )
{}

// *******************************
//
template<>
inline CompositeInterpolator< bv::TimeType, std::wstring >::CompositeInterpolator( float tolerance )
	: m_type( CurveType::CT_POINT )
	, m_tolerance( tolerance )
	, m_preMethod( WrapMethod::clamp ), m_postMethod( WrapMethod::clamp )
{}

// *******************************
//
template<>
inline CompositeInterpolator< bv::TimeType, bool >::CompositeInterpolator( float tolerance )
    : m_type( CurveType::CT_POINT )
    , m_tolerance( tolerance )
    , m_preMethod( WrapMethod::clamp ), m_postMethod( WrapMethod::clamp )
{}

} // bv
