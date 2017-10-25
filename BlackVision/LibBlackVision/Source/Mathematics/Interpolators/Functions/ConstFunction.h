#pragma once

#include "Mathematics/Interpolators/CompositeInterpolator.h"
#include "IEvaluator.h"


namespace bv
{

// *******************************
//
template< class TimeValueT, class ValueT >
class ConstEvaluator : public IEvaluator< TimeValueT, ValueT >
{
    FRIEND_TEST_ACCESSOR( ConstEvaluator )
private:

    ValueT value;

public:
    ConstEvaluator( ValueT v ) : value( v ) {}

    virtual EvaluatorType           GetType         () const override { return EvaluatorType::ET_CONSTANT; }
    virtual CurveType               GetCurveType    () const override { return CurveType::CT_POINT; }

    virtual void SetValue( TimeValueT /*t*/, ValueT v ) override
    {
        value = v;
    }

    ValueT Evaluate( TimeValueT ) const override { return value; }

    virtual void                                        Serialize       ( ISerializer& ser ) const override
    {
    ser.EnterChild( "interpolation" );
        ser.SetAttribute( "type", SerializationHelper::T2String( CurveType::CT_POINT ) );
    ser.ExitChild();
    }

    virtual void                                Deserialize( const IDeserializer& deser )
    {
        if( deser.GetAttribute( "type" ) != SerializationHelper::T2String( CurveType::CT_POINT ) )
            assert( false );
    }

};



}
