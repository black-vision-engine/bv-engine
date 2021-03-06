#pragma once

#include "Key.h"

#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"

#include "Exceptions/Serialization/SerializationException.h"

#include "Tools/Utils.h"

#include "CoreDEF.h"

#include "UseLoggerLibBlackVision.h"

namespace bv
{

// *************************************
//
template<class TimeValueT, class ValueT >
Key<TimeValueT, ValueT>::Key(TimeValueT t, ValueT val)
    : t(t), val(val)
{
}

// *************************************
//
template<class TimeValueT, class ValueT >
void                Key<TimeValueT, ValueT>::Serialize       ( ISerializer& ser ) const
{
    ser.EnterChild( "key" );
	ser.SetAttribute( "time", Convert::T2String( t ) );
    ser.SetAttribute( "val", Convert::T2String( val ) );
    ser.ExitChild();
}

// *************************************
//
template<class TimeValueT, class ValueT >
ISerializablePtr    Key<TimeValueT, ValueT>::Create          ( const IDeserializer & deser )
{
    auto time = Convert::String2T< TimeValueT >( deser.GetAttribute( "time" ) );
    auto val = Convert::String2T< ValueT >( deser.GetAttribute( "val" ) );

    if( !time.IsValid() )
    {
        // Invalid time. Don't add key, it has no value for us.
        Warn< SerializationException >( deser, "Cannot deserialize key. Invalid time: [" + deser.GetAttribute( "time" ) + "], value: [" + deser.GetAttribute( "val" ) + "]." );
        return nullptr;
    }
    else if( !val.IsValid() )
    {
        // Invalid value. We can add key with default value. This way we don't break interpolations array.
        Warn< SerializationException >( deser, "Cannot deserialize key, time: [" + deser.GetAttribute( "time" ) + "], value: [" + deser.GetAttribute( "val" ) + "]. Replacing with defaults." );
        return std::make_shared< Key< TimeValueT, ValueT > >( time.IsValid() ? time : TimeValueT(), ValueT() );
    }
    else
    {
        return std::make_shared< Key< TimeValueT, ValueT > >( time, val );
    }
}

// *************************************
//
template< class TimeValueT, class ValueT >
Key< TimeValueT, ValueT >   Key<TimeValueT, ValueT>::operator+   ( const Key< TimeValueT, ValueT > &that ) const
{
    return Key< TimeValueT, ValueT >( t + that.t, ValueT( val + that.val ) );
}

// *************************************
//
template< class TimeValueT, class ValueT >
Key< TimeValueT, ValueT >   Key<TimeValueT, ValueT>::operator-   ( const Key< TimeValueT, ValueT > &that ) const 
{
    return Key< TimeValueT, ValueT >( t - that.t, ValueT( val - that.val ) );
}

// *************************************
//
template<>
Key< bv::TimeType, bool > Key< bv::TimeType, bool >::operator+( const Key< bv::TimeType, bool > &/*that*/ ) const
{
    assert( false ); return Key< bv::TimeType, bool >( 0, false );
}

// *************************************
//
template<>
Key< bv::TimeType, bool > Key< bv::TimeType, bool >::operator-( const Key< bv::TimeType, bool > &/*that*/ ) const
{
    assert( false ); return Key< bv::TimeType, bool >( 0, false );
}

// *************************************
//
template<>
Key< bv::TimeType, std::string > Key< bv::TimeType, std::string >::operator+( const Key< bv::TimeType, std::string > & ) const
{
    assert( false ); return Key< bv::TimeType, std::string >( 0, "" );
}

// *************************************
//
template<>
Key< bv::TimeType, std::string > Key< bv::TimeType, std::string >::operator-( const Key< bv::TimeType, std::string > & ) const
{
    assert( false ); return Key< bv::TimeType, std::string >( 0, "" );
}

// *************************************
//
template<>
Key< bv::TimeType, std::wstring > Key< bv::TimeType, std::wstring >::operator+( const Key< bv::TimeType, std::wstring > & ) const
{
    assert( false ); return Key< bv::TimeType, std::wstring >( 0, L"" );
}

// *************************************
//
template<>
Key< bv::TimeType, std::wstring > Key< bv::TimeType, std::wstring >::operator-( const Key< bv::TimeType, std::wstring > & ) const
{
    assert( false ); return Key< bv::TimeType, std::wstring >( 0, L"" );
}

// *************************************
//
template<>
Key< bv::TimeType, bool > operator*( const bv::TimeType & /*a*/, const Key< bv::TimeType, bool > &/*that*/ )
{
    assert( false ); return Key< bv::TimeType, bool >( 0, false );
}

} // bv