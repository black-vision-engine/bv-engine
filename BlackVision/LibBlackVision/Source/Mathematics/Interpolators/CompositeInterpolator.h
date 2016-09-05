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
    std::vector< IEvaluator* >                          interpolators; // FIXME: ptr-ize
    TimeValueT                                          m_tolerance;

    CurveType                                           m_type;
    WrapMethod                                          m_preMethod, m_postMethod;

public:
    typedef TimeValueT                  TimeType;
    typedef ValueT                      ValueType;

    typedef TimeValueT                  TimeT;
    typedef ValueT                      ValT;

public:
                                                        CompositeInterpolator( const CompositeInterpolator & that );
    explicit                                            CompositeInterpolator( float tolerance = 0.0001f );

    static std::shared_ptr< CompositeInterpolator< TimeValueT, ValueT > >
                                                        Create              ( float tolerance = 0.0001f );

    virtual void                                        Serialize           ( ISerializer & ) const override;

    static std::shared_ptr< CompositeInterpolator< TimeValueT, ValueT > > 
                                                        Create              ( const IDeserializer & );

    void                                                AddKey              ( TimeValueT t, const ValueT & v );
    bool                                                RemoveKey           ( TimeValueT t );
    bool                                                MoveKey             ( TimeValueT t, TimeValueT newTime );
    ValueT                                              Evaluate            ( TimeValueT t ) const;
    
    void                                                RemoveAllKeys       ();

    int                                                 GetNumKeys          ();
    const std::vector< Key > &                          GetKeys             () const;
    std::vector< Key > &                                GetKeys             ();
    const std::vector< IEvaluator* > &                  GetInterpolators    ();

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

} // bv