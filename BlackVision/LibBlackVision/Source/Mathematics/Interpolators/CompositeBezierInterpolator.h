#pragma once

#include "BasicInterpolator.h"

namespace bv
{

class CompositeBezierInterpolator
{
    typedef model::IParameter::CurveType CurveType;

private:
    typedef float TimeValueT;
    typedef float ValueT;

    std::vector< Key<TimeValueT, ValueT> >    keys;
    std::vector< IInterpolator<TimeValueT, ValueT >* > interpolators; // FIXME: ptr-ize
    TimeValueT                              tolerance;

    model::IParameter::CurveType                   m_type;

public:
    typedef TimeValueT  TimeType;
    typedef ValueT      ValueType;

    typedef TimeValueT                  TimeT;
    typedef ValueT                      ValT;

public:
    CompositeBezierInterpolator();
    CompositeBezierInterpolator( const CompositeBezierInterpolator& that ); //{ keys = that.keys; interpolators = that.interpolators; tolerance = that.tolerance; m_type = that.m_type; }

    void AddKey             ( TimeValueT t, const ValueT & v );
    ValueT Evaluate         ( TimeValueT t ) const;

    void                    SetCurveType( CurveType type );

// FIXME: below is to remove
    const std::vector<Key<TimeValueT, ValueT>> & AccessKeys() const { static std::vector<Key<TimeValueT, ValueT>> ret; return ret; };
    //void                    SetInterpolationMethod ( model::IParameter::InterpolationMethod method ) { SetCurveType( (CurveType)method ); } // FIXME: co pan ja nie jestem rolnikiem!!!!!!!!!!!!!!!!!!!!!!!!!
    //model::IParameter::InterpolationMethod     GetInterpolationMethod () const { return model::IParameter::InterpolationMethod::LINEAR; } // WTF
    void SetWrapPostMethod  ( WrapMethod ) {}
    void SetWrapPreMethod   ( WrapMethod ) {}

private:
    float PreEvaluate( float t ) const;
    float PostEvaluate( float t ) const;
};

}