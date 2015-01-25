#pragma once

#include <vector>


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
};

template<class TimeValueT>
class Interpolator
{
public:

    typedef TimeValueT TimeType;

public:

    virtual int EvalToCBuffer( TimeValueT time, char * buf ) const = 0;

};

template<class TimeValueT, class ValueT>
class BasicInterpolator : public Interpolator<TimeValueT>
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

    const KeyType &     FirstKey    () const;
    const KeyType &     LastKey     () const;

};

} //bv
