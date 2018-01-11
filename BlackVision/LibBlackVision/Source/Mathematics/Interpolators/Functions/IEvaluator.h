#pragma once

#include "Mathematics/Interpolators/InterpolatorBasicTypes.h"
#include "Exceptions/Serialization/SerializationLogicError.h"



namespace bv
{

// ***********************
//
template< class TimeValueT, class ValueT >
class IEvaluator : public ISerializable // FIXME perhaps not every evaluator has to be serializable
{
public:
    virtual void                                Deserialize     ( const IDeserializer& deser )  = 0;

    virtual ValueT                              Evaluate        ( TimeValueT t ) const          = 0;
    virtual void                                SetValue        ( TimeValueT t, ValueT v )      = 0;
    virtual EvaluatorType                       GetType         () const                        = 0;
    virtual CurveType                           GetCurveType    () const                        = 0;

public:

    bool                    ValidateCurveType   ( const IDeserializer & deser, CurveType curve );
};

// ========================================================================= //
// Type traits for curves
// ========================================================================= //


#define SPECIALIZE_TYPE_DEFAULT_CURVE( typeName, curveType )    \
template<>                                                      \
inline CurveType        DefaultCurveType< typeName >() { return curveType; }



// ***********************
// Specialize template for other types.
template< typename ValueType >
inline CurveType        DefaultCurveType    ()
{
    return CurveType::CT_LINEAR;
}

SPECIALIZE_TYPE_DEFAULT_CURVE( std::string, CurveType::CT_POINT )
SPECIALIZE_TYPE_DEFAULT_CURVE( std::wstring, CurveType::CT_POINT )
SPECIALIZE_TYPE_DEFAULT_CURVE( int, CurveType::CT_POINT )
SPECIALIZE_TYPE_DEFAULT_CURVE( bool, CurveType::CT_POINT )


// ========================================================================= //
// Validation helper functions
// ========================================================================= //

// ***********************
//
template< class TimeValueT, class ValueT >
inline bool             IEvaluator< TimeValueT, ValueT >::ValidateCurveType     ( const IDeserializer & deser, CurveType curve )
{
    if( deser.GetAttribute( "type" ) != Convert::T2String( curve ) )
    {
        Warn< SerializationLogicError >( deser, "Created interpolator of other type ["
            + Convert::T2String( curve )
            + "] then serialized in file ["
            + deser.GetAttribute( "type" ) + "]." );

        assert( false );
        return false;
    }

    return true;
}


}	// bv


