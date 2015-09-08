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

    std::vector< Key<TimeValueT, ValueT> >              keys;
    std::vector< IEvaluator<TimeValueT, ValueT >* >     interpolators; // FIXME: ptr-ize
    TimeValueT                                          m_tolerance;

    CurveType                                           m_type;

public:
    typedef TimeValueT  TimeType;
    typedef ValueT      ValueType;

    typedef TimeValueT                  TimeT;
    typedef ValueT                      ValT;

public:
    CompositeBezierInterpolator( float tolerance = 0.000001 );
    CompositeBezierInterpolator( const CompositeBezierInterpolator& that );

    void AddKey             ( TimeValueT t, const ValueT & v );
    ValueT Evaluate         ( TimeValueT t ) const;

    void                    SetCurveType( CurveType type );

// FIXME: below is to remove
    const std::vector<Key<TimeValueT, ValueT>> & AccessKeys() const { static std::vector<Key<TimeValueT, ValueT>> ret; return ret; };
    void SetWrapPostMethod  ( WrapMethod ) {}
    void SetWrapPreMethod   ( WrapMethod ) {}

private:
    float PreEvaluate( float t ) const;
    float PostEvaluate( float t ) const;
};

}