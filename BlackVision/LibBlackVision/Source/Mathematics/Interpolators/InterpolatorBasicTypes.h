#pragma once

namespace bv
{

enum class WrapMethod : int
{
    clamp = 0,
    repeat = 1,
    pingPong = 2,
};

template<class TimeValueT/* = bv::TimeType*/, class ValueT>
class Key : public ISerializable
{
public:

    TimeValueT  t;
    ValueT      val;

public:

    explicit Key( TimeValueT t, ValueT val );

    virtual void                Serialize       ( ISerializer& doc ) const override;
    static ISerializablePtr     Create          ( const IDeserializer& doc );

    Key< TimeValueT, ValueT > operator+( const Key< TimeValueT, ValueT > &that ) const { return Key< TimeValueT, ValueT >( t + that.t, ValueT( val + that.val ) ); }
    Key< TimeValueT, ValueT > operator-( const Key< TimeValueT, ValueT > &that ) const { return Key< TimeValueT, ValueT >( t - that.t, ValueT( val - that.val ) ); }
    void operator=( const std::pair< TimeValueT, ValueT >& p ) { t = p.first; val = p.second; }
};

template<class TimeValueT, class ValueT>
Key< TimeValueT, ValueT > operator*( const TimeValueT & a, const Key< TimeValueT, ValueT > &that ) { return Key< TimeValueT, ValueT >( TimeValueT( a * that.t ), ValueT( a * that.val ) ); }

enum CurveType 
{ 
    CT_POINT = 0, 
    CT_LINEAR, 
    CT_BEZIER,
    CT_COSINE_LIKE,

    CT_CUBIC_IN,
    CT_CUBIC_OUT,
    CT_ELASTIC_IN,
    CT_ELASTIC_OUT,

    //CT_CUBIC_IN_BOUNCE,
    //CT_CUBIC_OUT_BOUNCE,
    CT_ELASTIC_IN_BOUNCE,
    CT_ELASTIC_OUT_BOUNCE,

    CT_QUARTIC_INOUT,
    CT_CUBIC_INTOUT,

    CT_TOTAL
};

enum class EvaluatorType : int
{ 
    ET_CONSTANT, 
    ET_LINEAR, 
    ET_BEZIER,
    ET_POLYNOMIAL
};


}
