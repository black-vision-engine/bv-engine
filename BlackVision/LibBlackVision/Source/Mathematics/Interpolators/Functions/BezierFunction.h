#pragma once

namespace bv
{



// *******************************
//
template< class TimeValueT, class ValueT >
class BezierEvaluator : public IEvaluator< TimeValueT, ValueT >
{
    typedef Key< TimeValueT, ValueT > Key;

public: // FIXME
    
    Key key1, key2;
    Key v1, v2;
    TimeValueT  m_tolerance;
    CurveType   m_curveType;

public:
    virtual EvaluatorType           GetType         () override { return EvaluatorType::ET_BEZIER; }
    virtual CurveType               GetCurveType    () override { return m_curveType; }

    BezierEvaluator( CurveType curve, Key k1, Key k2, Key v1_, Key v2_, TimeValueT tolerance ) : m_curveType( curve ), key1( k1 ), key2( k2 ), v1( v1_ ), v2( v2_ ), m_tolerance( tolerance ) {}
    
    void SetV2( Key v2 ) { this->v2 = v2; }
    
    virtual void SetValue( TimeValueT t, ValueT v ) override
    {
        if( key1.t == t )
            key1.val = v;
        else if( key2.t == t )
            key2.val = v;
        else
            assert( false );
    }

    ValueT Evaluate( TimeValueT t ) const override 
    {
        assert( key1.t <= t && t <= key2.t );

        Key A = key1,
            B = key1 + v1,
            C = key2 + v2,
            D = key2;

        for( ; ; ) 
        {
            Key middle = 1.f/8 * A + 3.f/8 * B + 3.f/8 * C + 1.f/8 * D;
            
            if( fabs( middle.t - t ) < m_tolerance )
                return middle.val;
            else
            {
                if( t < middle.t )
                {
                    A = A;
                    B = 1.f/2 * A + 1.f/2 * B;
                    C = 1.f/4 * A + 1.f/2 * B + 1.f/4 * C;
                    D = middle;
                }
                else
                {
                    A = middle;
                    B = 1.f/4 * B + 1.f/2 * C + 1.f/4 * D;
                    C = 1.f/2 * C + 1.f/2 * D;
                    D = D;               
                }
            }
        }
    }

    virtual void                                        Serialize       ( ISerializer& ser ) const override
    {
    ser.EnterChild( "interpolation" );
        ser.SetAttribute( "type", SerializationHelper::T2String( m_curveType ) );
        
        ser.EnterChild( "v1" );
            SerializationHelper::SerializeAttribute( ser, v1.t, "dt" );
            SerializationHelper::SerializeAttribute( ser, v1.val, "dval" );
        ser.ExitChild();
        
        ser.EnterChild( "v2" );
            SerializationHelper::SerializeAttribute( ser, v2.t, "dt" );
            SerializationHelper::SerializeAttribute( ser, v2.val, "dval" );
        ser.ExitChild();
        
    ser.ExitChild();
    }

    virtual void                                Deserialize( const IDeserializer& deser )
    {
        if( deser.GetAttribute( "type" ) != SerializationHelper::T2String( m_curveType ) )
            assert( false );

        deser.EnterChild( "v1" );
            v1.t = SerializationHelper::String2T< TimeValueT >( deser.GetAttribute( "dt" ) );
            v1.val = SerializationHelper::String2T< ValueT >( deser.GetAttribute( "dval" ) );
        deser.ExitChild();

        deser.EnterChild( "v2" );
            v2.t = SerializationHelper::String2T< TimeValueT >( deser.GetAttribute( "dt" ) );
            v2.val = SerializationHelper::String2T< ValueT >( deser.GetAttribute( "dval" ) );
        deser.ExitChild();
    }
};

}
