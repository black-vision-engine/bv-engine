#pragma once

#include <vector>

#include "Engine/Models/Plugins/Interfaces/IParameter.h"

#include "InterpolatorBasicTypes.h"

#include "Serialization/ISerializable.h"

namespace bv
{

template<class TimeValueT>
class Interpolator
{
public:
    virtual int EvalToCBuffer( TimeValueT time, char * buf ) const = 0;
};

template<class TimeValueT, class ValueT, class FloatT = float >
class BasicInterpolator : public Interpolator<TimeValueT>, public ISerializable
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

    virtual void                Serialize       ( ISerializer& doc ) const override;
    static ISerializablePtr     Create          ( const IDeserializer& doc );

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

	const std::vector<Key<TimeValueT, ValueT>> & AccessKeys() const;

    const KeyType &     FirstKey    () const;
    const KeyType &     LastKey     () const;

    int                                                 GetNumKeys      ();

    void                                                SetCurveType    ( CurveType /*type*/ ) { assert( false ); }
    CurveType                                           GetCurveType    () { assert( false ); return CurveType::CT_TOTAL; }
};

} //bv
