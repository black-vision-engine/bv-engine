#pragma once

#include "Mathematics/Interpolators/Key.h"

#include "Serialization/SerializationHelper.h"


namespace bv
{


// *******************************
//
template< class TimeValueT, class ValueT >
class LinearEvaluator : public IEvaluator< TimeValueT, ValueT >
{
    typedef Key< TimeValueT, ValueT > Key;
    FRIEND_TEST_ACCESSOR( LinearEvaluator )

private:

    Key key1, key2;
    TimeValueT m_tolerance;

public:
    LinearEvaluator( Key k1, Key k2, TimeValueT tolerance ) : key1( k1 ), key2( k2 ), m_tolerance( tolerance ) {}

    virtual EvaluatorType           GetType         () const override { return EvaluatorType::ET_LINEAR; }
    virtual CurveType               GetCurveType    () const override { return CurveType::CT_LINEAR; }

    virtual void SetValue( TimeValueT t, ValueT v ) override
    {
        if( fabs( key1.t - t ) < m_tolerance )
            key1.val = v;
        else if( fabs( key2.t - t ) < m_tolerance )
            key2.val = v;
        else
            assert( false );
    }

    ValueT Evaluate( TimeValueT t ) const override 
    { 
        TimeValueT alpha = ( t - key1.t ) / ( key2.t - key1.t );
        return ValueT( alpha * key2.val + (1-alpha) * key1.val );
    }

    virtual void                                        Serialize       ( ISerializer& ser ) const override
    {
        ser.EnterChild( "interpolation" );
            ser.SetAttribute( "type", SerializationHelper::T2String( CurveType::CT_LINEAR ) );
        ser.ExitChild();
    }

    virtual void                                Deserialize( const IDeserializer& deser )
    {
        if( deser.GetAttribute( "type" ) != SerializationHelper::T2String( CurveType::CT_LINEAR ) )
            assert( false );
    }
};

template<>
bool LinearEvaluator< bv::TimeType, bool >::Evaluate( bv::TimeType t ) const
{
    bv::TimeType alpha = ( t - key1.t ) / ( key2.t - key1.t );
    return int( alpha * key2.val + (1-alpha) * key1.val ) > 0;
}

}