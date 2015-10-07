#pragma once

#include "BasicInterpolator.h"

namespace bv
{

enum CurveType 
{ 
    POINT, 
    LINEAR, 
    BEZIER,
    COSINE_LIKE
};

enum class EvaluatorType : int
{ 
    CONSTANT, 
    LINEAR, 
    BEZIER 
};

template< class TimeValueT, class ValueT >
class IEvaluator
{
public:
    virtual ValueT                              Evaluate( TimeValueT t ) const                      = 0;
    virtual void                                SetValue( TimeValueT t, ValueT v )                  = 0;
    virtual EvaluatorType                       GetType()                                           = 0;
};

class CompositeBezierInterpolator
{
private:
    typedef float TimeValueT;
    typedef float ValueT;

    typedef Key< TimeValueT, ValueT >                   Key;
    typedef IEvaluator< TimeValueT, ValueT >            IEvaluator;

    std::vector< Key >                                  keys;
    std::vector< IEvaluator* >                          interpolators; // FIXME: ptr-ize
    TimeValueT                                          m_tolerance;

    CurveType                                           m_type;
    WrapMethod                                          m_preMethod, m_postMethod;

public:
    typedef TimeValueT  TimeType;
    typedef ValueT      ValueType;

    typedef TimeValueT                  TimeT;
    typedef ValueT                      ValT;

public:
    CompositeBezierInterpolator( float tolerance = 0.000001 );
    CompositeBezierInterpolator( const CompositeBezierInterpolator& that );

    void                                                AddKey             ( TimeValueT t, const ValueT & v );
    ValueT                                              Evaluate         ( TimeValueT t ) const;

    const std::vector< Key > &                          GetKeys();
    const std::vector< IEvaluator* > &                  GetInterpolators();

    void                                                SetCurveType( CurveType type );
    void                                                SetWrapPostMethod  ( WrapMethod method );
    void                                                SetWrapPreMethod   ( WrapMethod method );

    void                                                SetKey1( int i, Key key );
    void                                                SetKey2( int i, Key key );
    void                                                SetV1( int i, Key v );
    void                                                SetV2( int i, Key v );

// FIXME: below is to remove
    const std::vector<Key> & AccessKeys() const { static std::vector<Key> ret; return ret; };

private:
    float                                               PreEvaluate( float t ) const;
    float                                               PostEvaluate( float t ) const;
};

}