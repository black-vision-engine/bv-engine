#pragma once

#include "BasicInterpolator.h"
#include "InterpolatorBasicTypes.h"

namespace bv
{

template< class TimeValueT, class ValueT >
class IEvaluator : public ISerializable // FIXME perhaps not every evaluator has to be serializable
{
public:
    virtual void                                Deserialize( const IDeserializer& deser ) = 0;
    
    virtual ValueT                              Evaluate( TimeValueT t ) const                      = 0;
    virtual void                                SetValue( TimeValueT t, ValueT v )                  = 0;
    virtual EvaluatorType                       GetType()                                           = 0;
};

template< class TimeValueT, class ValueT >
class CompositeBezierInterpolator : public ISerializable
{
private:
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
    CompositeBezierInterpolator( float tolerance = 0.0001f );
    CompositeBezierInterpolator( const CompositeBezierInterpolator& that );

    virtual void                                        Serialize       ( ISerializer& ) const override;
    static ISerializablePtr                             Create          ( const IDeserializer& );

    void                                                AddKey          ( TimeValueT t, const ValueT & v );
    ValueT                                              Evaluate        ( TimeValueT t ) const;

    int                                                 GetNumKeys      ();
    const std::vector< Key > &                          GetKeys         ();
    const std::vector< IEvaluator* > &                  GetInterpolators();

    void                                                SetCurveType    ( CurveType type );
    CurveType                                           GetCurveType    ();

    void                                                SetWrapPostMethod  ( WrapMethod method );
    void                                                SetWrapPreMethod   ( WrapMethod method );
    WrapMethod                                          GetWrapPostMethod  ();
    WrapMethod                                          GetWrapPreMethod   ();

    void                                                SetKey1             ( int i, Key key );
    void                                                SetKey2             ( int i, Key key );
    void                                                SetV1               ( int i, Key v );
    void                                                SetV2               ( int i, Key v );

// FIXME: below is to remove
    //const std::vector<Key> & AccessKeys() const { static std::vector<Key> ret; return ret; };

private:
    ValueT                                              PreEvaluate( TimeValueT t ) const;
    ValueT                                              PostEvaluate( TimeValueT t ) const;
};

}