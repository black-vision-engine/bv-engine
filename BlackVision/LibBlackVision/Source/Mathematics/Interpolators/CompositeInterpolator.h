#pragma once

#include "BasicInterpolator.h"
#include "InterpolatorBasicTypes.h"

#include "Mathematics/Interpolators/Functions/IEvaluator.h"


namespace bv
{


// ***********************
//
template< class TimeValueT, class ValueT >
class CompositeInterpolator : public ISerializable
{
    FRIEND_TEST_ACCESSOR( CompositeInterpolator )
private:
    typedef Key< TimeValueT, ValueT >                   Key;
    typedef std::shared_ptr< Key >                      KeyPtr;
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

    inline bool                                         AddKey              ( TimeValueT t, const ValueT & v );
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

    static inline std::vector< CurveType >              DeserializeCurves           ( const IDeserializer & deser );
    static inline void                                  DeserializeInterpolators    ( const IDeserializer & deser, std::shared_ptr< CompositeInterpolator< TimeValueT, ValueT > > & interpolator, const std::vector< CurveType > & curves );

    static inline bool                                  ValidateMatching    ( const IDeserializer & deser, const std::vector< CurveType > & curves, const std::vector< std::shared_ptr< Key > > & keys );
};




} // bv
