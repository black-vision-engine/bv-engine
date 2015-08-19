#pragma once

#include "BasicInterpolator.h"

namespace bv
{

template< class TimeValueT, class ValueT >
class IInterpolator
{
public:
    virtual ValueT Evaluate( TimeValueT t ) const = 0;
};

class CompositeBezierInterpolator
{
public:
    enum CurveType { POINT, LINEAR, BEZIER };

private:
    typedef float TimeValueT;
    typedef float ValueT;

    std::vector< Key<TimeValueT, ValueT> >    keys;
    std::vector< IInterpolator<TimeValueT, ValueT >* > interpolators; // FIXME: ptr-ize
    TimeValueT                              tolerance;

    CurveType                               m_type;

public:
    typedef TimeValueT  TimeType;
    typedef ValueT      ValueType;

    typedef TimeValueT                  TimeT;
    typedef ValueT                      ValT;

public:
    void AddKey             ( TimeValueT t, const ValueT & v );
    ValueT Evaluate         ( TimeValueT t ) const;

    void                    SetCurveType( CurveType type );

// FIXME: below is to remove
    const std::vector<Key<TimeValueT, ValueT>> & AccessKeys() const { static std::vector<Key<TimeValueT, ValueT>> ret; return ret; };
    void                    SetInterpolationMethod ( model::IParameter::InterpolationMethod method ) { SetCurveType( (CurveType)method ); } // FIXME: co pan ja nie jestem rolnikiem!!!!!!!!!!!!!!!!!!!!!!!!!
    model::IParameter::InterpolationMethod     GetInterpolationMethod () const { return model::IParameter::InterpolationMethod::LINEAR; } // WTF
    void SetWrapPostMethod  ( WrapMethod ) {}
    void SetWrapPreMethod   ( WrapMethod ) {}

private:
    float PreEvaluate( float t ) const;
    float PostEvaluate( float t ) const;
};

}