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
    virtual CurveType                           GetCurveType()                              = 0;
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

    inline void                                         AddKey              ( TimeValueT t, const ValueT & v );
    inline bool                                         RemoveKey           ( TimeValueT t );
    inline bool                                         MoveKey             ( TimeValueT t, TimeValueT newTime );
    inline ValueT                                       Evaluate            ( TimeValueT t ) const;
    
	inline void                                         RemoveAllKeys       ();

	inline int                                          GetNumKeys          ();
	inline const std::vector< Key > &                   GetKeys             () const;
	inline std::vector< Key > &                         GetKeys             ();

	inline void                                         SetGlobalCurveType  ( CurveType type );
	inline void                                         SetAddedKeyCurveType ( CurveType type );
	inline CurveType                                    GetCurveType        ();

	inline void                                         SetWrapPostMethod   ( WrapMethod method );
	inline void                                         SetWrapPreMethod    ( WrapMethod method );
	inline WrapMethod                                   GetWrapPostMethod   ();
	inline WrapMethod                                   GetWrapPreMethod    ();

private:
	inline ValueT                                       PreEvaluate         ( TimeValueT t ) const;
	inline ValueT                                       PostEvaluate        ( TimeValueT t ) const;
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
