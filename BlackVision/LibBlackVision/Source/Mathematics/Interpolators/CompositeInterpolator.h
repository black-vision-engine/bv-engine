#pragma once

#include "BasicInterpolator.h"
#include "InterpolatorBasicTypes.h"

namespace bv
{

template< class TimeValueT, class ValueT >
class IEvaluator : public ISerializable // FIXME perhaps not every evaluator has to be serializable
{
public:
    virtual void                                Deserialize( const IDeserializer& deser )   = 0;
    
    virtual ValueT                              Evaluate( TimeValueT t ) const              = 0;
    virtual void                                SetValue( TimeValueT t, ValueT v )          = 0;
    virtual EvaluatorType                       GetType()                                   = 0;
};


template< class TimeValueT, class ValueT >
class CompositeInterpolator : public ISerializable
{
private:
    typedef Key< TimeValueT, ValueT >                   Key;
    typedef IEvaluator< TimeValueT, ValueT >            IEvaluator;

    std::vector< Key >                                  keys;
    std::vector< std::shared_ptr< IEvaluator > >        interpolators; // FIXME: ptr-ize
    TimeValueT                                          m_tolerance;

    CurveType                                           m_type;
    WrapMethod                                          m_preMethod, m_postMethod;

public:
    typedef TimeValueT                  TimeType;
    typedef ValueT                      ValueType;

    typedef TimeValueT                  TimeT;
    typedef ValueT                      ValT;

public:
                                                        CompositeInterpolator   ( const CompositeInterpolator & that );
    explicit                                            CompositeInterpolator   ( float tolerance = 0.0001f );
     

    static std::shared_ptr< CompositeInterpolator< TimeValueT, ValueT > >
                                                        Create              ( float tolerance = 0.0001f );

    virtual void                                        Serialize           ( ISerializer & ) const override;

    static std::shared_ptr< CompositeInterpolator< TimeValueT, ValueT > > 
                                                        Create              ( const IDeserializer & );

    void                                                AddKey              ( TimeValueT t, const ValueT & v );
    bool                                                RemoveKey           ( TimeValueT t );
    bool                                                MoveKey             ( TimeValueT t, TimeValueT newTime );
    inline ValueT                                       Evaluate            ( TimeValueT t ) const;
    
    void                                                RemoveAllKeys       ();

    int                                                 GetNumKeys          ();
    const std::vector< Key > &                          GetKeys             () const;
    std::vector< Key > &                                GetKeys             ();

    void                                                SetGlobalCurveType  ( CurveType type );
    void                                                SetAddedKeyCurveType ( CurveType type );
    CurveType                                           GetCurveType        ();

    void                                                SetWrapPostMethod   ( WrapMethod method );
    void                                                SetWrapPreMethod    ( WrapMethod method );
    WrapMethod                                          GetWrapPostMethod   ();
    WrapMethod                                          GetWrapPreMethod    ();

private:
    ValueT                                              PreEvaluate         ( TimeValueT t ) const;
    ValueT                                              PostEvaluate        ( TimeValueT t ) const;
};


// *******************************
//
template<>
CompositeInterpolator< bv::TimeType, std::string >::CompositeInterpolator( float tolerance );

// *******************************
//
template<>
CompositeInterpolator< bv::TimeType, std::wstring >::CompositeInterpolator( float tolerance );


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
		TimeValueT r = divmod( t, &q );
		return Evaluate( tStart + r );
	}
	else if( m_postMethod == WrapMethod::pingPong )
	{
		TimeValueT q = interval;
		TimeValueT r = divmod( t, &q );

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
inline ValueT CompositeInterpolator< TimeValueT, ValueT >::Evaluate         ( TimeValueT t ) const
{
	auto size = keys.size();
	if( size == 0 )
	{
		assert( false ); // FIXME: error handling FTW
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


} // bv