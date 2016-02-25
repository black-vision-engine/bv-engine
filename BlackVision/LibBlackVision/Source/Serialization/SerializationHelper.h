#pragma once

#include "Serialization/ISerializable.h"
#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"

#include "Expected.h"

namespace bv {

namespace SerializationHelper {

// *************************************
//
template< typename T >
std::shared_ptr< T >                                        DeserializeObject( const IDeserializer& deser, std::string name )
{
    auto sucess = deser.EnterChild( name );

    if( sucess )
    {
        auto obj = T::Create( deser );
        deser.ExitChild();
        return std::static_pointer_cast< T >( obj );
    }
    else
    {
        return nullptr;
    }
}

//// *************************************
////
//template< typename T >
//std::shared_ptr< T >                                        DeserializeObjectPtr( const IDeserializer& deser, std::string name )
//{
//    return std::shared_ptr< T >( DeserializeObject< T >( deser, name ) );
//}

// *************************************
//
template< typename T >
std::vector< std::shared_ptr< T > >                         DeserializeArray( const IDeserializer& deser, std::string nameParent, std::string nameChild="" )
{
    std::vector< std::shared_ptr< T > > ret;

    bool success = deser.EnterChild( nameParent );
    if( !success )
        return ret;

// create nameChild if necessary
    if( nameChild == "" )
    {
        assert( nameParent[ nameParent.size()-1 ] == 's' );
        nameChild = nameParent.substr( 0, nameParent.size()-1 );
    }

    if( deser.EnterChild( nameChild ) )
    {
        do
        {
            auto obj = ISerializablePtr( T::Create( deser ) );
            ret.push_back( std::static_pointer_cast< T >( obj ) );
        }while( deser.NextChild() );
        deser.ExitChild(); // nameChild
    }

    deser.ExitChild(); // nameParent
    return ret;
}

// *************************************
//
template< typename T >
std::vector< std::shared_ptr< T > >                         DeserializeProperties( const IDeserializer& deser, std::string name )
{
    std::vector< std::shared_ptr< T > > ret;

    if( deser.EnterChild( name ) )
    {
        do
        {
            auto obj = ISerializablePtr( T::Create( deser ) );

            ret.push_back( std::static_pointer_cast< T >( obj ) );
        }while( deser.NextChild( ) );
        deser.ExitChild();
    }

    return ret;
}

template< typename T >
void SerializeObjectImpl( const T& o, ISerializer& ser );

template< typename T >
std::shared_ptr< T > Create( const IDeserializer& deser )
{
    auto obj = T::Create( deser );
    return std::static_pointer_cast< T >( obj );
}

//template< typename T >
//T* DeserializeObjectImpl( ISerializer& dob );
//
//template< typename T >
//class SerializedObject : public ISerializable
//{
//    T o;
//
//public:
//    SerializedObject( const T& o_ ) : o( o_ ) { }
//
//    virtual void                Serialize       ( ISerializer& sob ) const { SerializeObjectImpl< T >( o, sob ); }
//
//    static ISerializablePtr     Create          ( ISerializer& dob ) { return ISerializablePtr( DeserializeObjectImpl< T >( dob ); }
//};

// glm stuff

template< typename T >
std::string T2String( const T & t );

template< typename T >
Expected<T> String2T( const std::string & s );

template< typename T >
T String2T( const std::string & s, const T & defaultVal );

} // SerializationHelper
} // bv
