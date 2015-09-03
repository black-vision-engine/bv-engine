#pragma once

#include <vector>

#include "Engine/Models/Plugins/Interfaces/IParameter.h"

namespace bv
{

enum class WrapMethod : int
{
    clamp = 0,
    repeat = 1,
    pingPong = 2,
};

template<class TimeValueT, class ValueT>
class Key
{
public:

    TimeValueT  t;
    ValueT      val;

public:

    explicit Key( TimeValueT t, ValueT val );

    Key< TimeValueT, ValueT > operator+( const Key< TimeValueT, ValueT > &that ) const { return Key< TimeValueT, ValueT >( t + that.t, val + that.val ); }
    Key< TimeValueT, ValueT > operator-( const Key< TimeValueT, ValueT > &that ) const { return Key< TimeValueT, ValueT >( t - that.t, val - that.val ); }
};

template<class TimeValueT, class ValueT>
Key< TimeValueT, ValueT > operator*( const float a, const Key< TimeValueT, ValueT > &that ) { return Key< TimeValueT, ValueT >( a * that.t, a * that.val ); }

template<class TimeValueT>
class Interpolator
{
    typedef model::IParameter::CurveType CurveType; // FIXME
public:

    //typedef TimeValueT TimeType;

public:
    virtual void                    SetCurveType    ( CurveType type )                      = 0;

    virtual int EvalToCBuffer( TimeValueT time, char * buf ) const = 0;

};

template<class TimeValueT, class ValueT, class FloatT = float >
class BasicInterpolator : public Interpolator<TimeValueT> // FIXME: this class will be removed
{
public:

    typedef TimeValueT  TimeType;
    typedef ValueT      ValueType;

private:
    std::vector<Key<TimeValueT, ValueT>>    keys;
    TimeValueT                              tolerance;

    WrapMethod wrapPre;
    WrapMethod wrapPost;

public:

    typedef Key<TimeValueT, ValueT> KeyType;

    typedef TimeValueT                  TimeT;
    typedef ValueT                      ValT;
    typedef Interpolator<TimeValueT>    ParentType;

    static const int value_size = sizeof( ValueT );

public:

    explicit BasicInterpolator  ( TimeValueT tolerance = 0.0001 );
    virtual ~BasicInterpolator  () {};

    void AddKey             ( TimeValueT t, const ValueT & v );
    void AddKey             ( const Key<TimeValueT, ValueT> & key );

    ValueT Evaluate         ( TimeValueT t ) const;
    ValueT EvaluatePoint    ( TimeValueT t ) const;

    TimeValueT CalcPreT     ( TimeValueT t ) const;
    TimeValueT CalcPostT    ( TimeValueT t ) const;

    void SetWrapPostMethod  ( WrapMethod );
    void SetWrapPreMethod   ( WrapMethod );

    WrapMethod GetWrapPostMethod() const;
    WrapMethod GetWrapPreMethod () const;

    void SetWrapMethod( WrapMethod pre, WrapMethod post );

    virtual int EvalToCBuffer( TimeValueT time, char * buf ) const;
    virtual void                SetCurveType    ( model::IParameter::CurveType /*type*/ ) override { } // remember, this class will be removed ;)

	const std::vector<Key<TimeValueT, ValueT>> & AccessKeys() const;

    const KeyType &     FirstKey    () const;
    const KeyType &     LastKey     () const;

};

} //bv
