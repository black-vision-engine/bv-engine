namespace bv {

// *******************************
//
template< class TimeValueT, class ValueT >
class LinearEvaluator : public IEvaluator< TimeValueT, ValueT >
{
    typedef Key< TimeValueT, ValueT > Key;

    Key key1, key2;
public:
    LinearEvaluator( Key k1, Key k2 ) : key1( k1 ), key2( k2 ) {}

    virtual EvaluatorType GetType() override { return EvaluatorType::ET_LINEAR; }

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
        TimeValueT alpha = ( t - key1.t ) / ( key2.t - key1.t );
        return ValueT( alpha * key2.val + (1-alpha) * key1.val );
    }

    virtual void                                        Serialize       ( ISerializer& ser ) const override
    {
    ser.EnterChild( "interpolation" );
        ser.SetAttribute( "type", "linear" );
    ser.ExitChild();
    }

    virtual void                                Deserialize( const IDeserializer& deser )
    {
        if( deser.GetAttribute( "type" ) != "linear" )
            assert( false );
    }
};

Key< bv::TimeType, bool > Key< bv::TimeType, bool >::operator+( const Key< bv::TimeType, bool > &/*that*/ ) const { assert( false ); return Key< bv::TimeType, bool >( 0, false ); }
Key< bv::TimeType, bool > Key< bv::TimeType, bool >::operator-( const Key< bv::TimeType, bool > &/*that*/ ) const { assert( false ); return Key< bv::TimeType, bool >( 0, false ); }

template<>
Key< bv::TimeType, bool > operator*( const bv::TimeType & /*a*/, const Key< bv::TimeType, bool > &/*that*/ ) { assert( false ); return Key< bv::TimeType, bool >( 0, false ); }

template<>
bool LinearEvaluator< bv::TimeType, bool >::Evaluate( bv::TimeType t ) const
{
    bv::TimeType alpha = ( t - key1.t ) / ( key2.t - key1.t );
    return int( alpha * key2.val + (1-alpha) * key1.val ) > 0;
}

}