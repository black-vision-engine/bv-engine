namespace bv {

template< class TimeValueT, class ValueT >
class PolynomialEvaluator : public IEvaluator< TimeValueT, ValueT >
{
    typedef Key< TimeValueT, ValueT > Key;
    typedef float Coefficient;

    Coefficient a, b, c, d, e; // a*t^5+b*t^4+c*t^3+d*t^2+e*t
    bool bounce;
    Key key1, key2;
    TimeValueT m_tolerance;
public:
    PolynomialEvaluator( Key k1, Key k2, Coefficient a, Coefficient b, Coefficient c, Coefficient d, Coefficient e, TimeValueT tolerance, bool bounce )
        : a( a ), b( b ), c( c ), d( d ), e( e ), bounce( bounce )
        , key1( k1 ), key2( k2 ), m_tolerance( tolerance )
    {
    }

    virtual EvaluatorType                               GetType() override { return EvaluatorType::ET_POLYNOMIAL; }

    virtual void                                        SetValue( TimeValueT t, ValueT v ) override
    {
        if( fabs( key1.t - t ) < m_tolerance )
            key1.val = v;
        else if( fabs( key2.t - t ) < m_tolerance )
            key2.val = v;
        else
            assert( false );
    }

    ValueT Evaluate( TimeValueT t_ ) const override 
    { 
        TimeValueT t = ( t_ - key1.t ) / ( key2.t - key1.t );
        TimeValueT alpha = ( ( ( ( ( a * t ) + b ) * t + c ) * t + d ) * t + e ) * t; // Horner FTW

        if( bounce )
            if( alpha < 0 )
                alpha = -alpha;
            else if( alpha > 1 )
                alpha = 2 - alpha;

        return ValueT( alpha * key2.val + (1-alpha) * key1.val );
    }

    virtual void                                        Serialize       ( ISerializer& /*ser*/ ) const override
    {
    //ser.EnterChild( "interpolation" );
    //    ser.SetAttribute( "type", "point" );
    //ser.ExitChild();
    }

    virtual void                                        Deserialize( const IDeserializer& /*deser*/ )
    {
        //if( deser.GetAttribute( "type" ) != "point" )
        //    assert( false );
    }
};

template<>
bool PolynomialEvaluator< bv::TimeType, bool >::Evaluate( bv::TimeType /*t*/ ) const
{
    assert( false );
    return true;
}

}
