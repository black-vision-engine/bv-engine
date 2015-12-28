#include "../CompositeBezierInterpolator.h"

namespace bv {

// *******************************
//
template< class TimeValueT, class ValueT >
class ConstEvaluator : public IEvaluator< TimeValueT, ValueT >
{
    ValueT value;
public:
    ConstEvaluator( ValueT v ) : value( v ) {}

    virtual EvaluatorType GetType() override { return EvaluatorType::ET_CONSTANT; }

    virtual void SetValue( TimeValueT /*t*/, ValueT v ) override
    {
        value = v;
    }

    ValueT Evaluate( TimeValueT ) const override { return value; }

    virtual void                                        Serialize       ( ISerializer& ser ) const override
    {
    ser.EnterChild( "interpolation" );
        ser.SetAttribute( "type", "point" );
    ser.ExitChild();
    }

    virtual void                                Deserialize( const IDeserializer& deser )
    {
        if( deser.GetAttribute( "type" ) != "point" )
            assert( false );
    }

};



}
