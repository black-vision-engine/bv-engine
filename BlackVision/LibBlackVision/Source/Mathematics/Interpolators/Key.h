#pragma once

#include "Serialization/ISerializable.h"



namespace bv
{



// ***********************
//
template<class TimeValueT/* = bv::TimeType*/, class ValueT>
class Key : public ISerializable
{
public:

    TimeValueT  t;
    ValueT      val;

public:

    explicit                    Key         ( TimeValueT t, ValueT val );

    virtual void                Serialize   ( ISerializer& doc ) const override;
    static ISerializablePtr     Create      ( const IDeserializer& doc );

    Key< TimeValueT, ValueT >   operator+   ( const Key< TimeValueT, ValueT > &that ) const;
    Key< TimeValueT, ValueT >   operator-   ( const Key< TimeValueT, ValueT > &that ) const;
    void                        operator=   ( const std::pair< TimeValueT, ValueT >& p ) { t = p.first; val = p.second; }
};

// ***************************
//
template< class TimeValueT, class ValueT >
Key< TimeValueT, ValueT > operator*( const TimeValueT & a, const Key< TimeValueT, ValueT > & that )
{
    return Key< TimeValueT, ValueT >( TimeValueT( a * that.t ), ValueT( a * that.val ) );
}

// *************************************
//
template<>
Key< bv::TimeType, bool > operator*( const bv::TimeType & /*a*/, const Key< bv::TimeType, bool > &/*that*/ );

// *************************************
//
template<>
Key< bv::TimeType, bool > Key< bv::TimeType, bool >::operator+( const Key< bv::TimeType, bool > & ) const;

// *************************************
//
template<>
Key< bv::TimeType, bool > Key< bv::TimeType, bool >::operator-( const Key< bv::TimeType, bool > & ) const;

// *************************************
//
template<>
Key< bv::TimeType, std::string > Key< bv::TimeType, std::string >::operator+( const Key< bv::TimeType, std::string > & ) const;

// *************************************
//
template<>
Key< bv::TimeType, std::string > Key< bv::TimeType, std::string >::operator-( const Key< bv::TimeType, std::string > & ) const;

// *************************************
//
template<>
Key< bv::TimeType, std::wstring > Key< bv::TimeType, std::wstring >::operator+( const Key< bv::TimeType, std::wstring > & ) const;

// *************************************
//
template<>
Key< bv::TimeType, std::wstring > Key< bv::TimeType, std::wstring >::operator-( const Key< bv::TimeType, std::wstring > & ) const;




} // bv

