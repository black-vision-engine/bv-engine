#pragma once

#include "ISerializable.h"
#include "ISerializer.h"
#include "IDeserializer.h"

#include "Mathematics/glm_inc.h"

namespace bv {

namespace SerializationHelper {

// *************************************
//
template< typename T >
std::shared_ptr< T >                                        DeserializeObjectLoadImpl( const IDeserializer& sob, std::string name )
{
    auto sucess = sob.EnterChild( name );
    assert( sucess ); // FIXME error handling
    auto obj = T::Create( sob );
    sob.ExitChild();
    return std::static_pointer_cast< T >( obj );
}

// *************************************
//
template< typename T >
std::vector< std::shared_ptr< T > >                         DeserializeObjectLoadArrayImpl( const IDeserializer& sob, std::string nameParent, std::string nameChild="" )
{
    std::vector< std::shared_ptr< T > > ret;

    bool success = sob.EnterChild( nameParent );
    if( !success )
        return ret;

// create nameChild if necessary
    if( nameChild == "" )
    {
        assert( nameParent[ nameParent.size()-1 ] == 's' );
        nameChild = nameParent.substr( 0, nameParent.size()-1 );
    }

    if( sob.EnterChild( nameChild ) )
    {
        do
        {
            auto obj = T::Create( sob );
            ret.push_back( std::static_pointer_cast< T >( obj ) );
        }while( sob.NextChild() );
        sob.ExitChild(); // nameChild
    }

    sob.ExitChild(); // nameParent
    return ret;
}

// *************************************
//
template< typename T >
std::vector< std::shared_ptr< T > >                         DeserializeObjectLoadPropertiesImpl( const IDeserializer& sob, std::string name )
{
    std::vector< std::shared_ptr< T > > ret;

    if( sob.EnterChild( name ) )
    {
        do
        {
            auto obj = T::Create( sob );

            ret.push_back( std::static_pointer_cast< T >( obj ) );
        }while( sob.NextChild( ) );
        sob.ExitChild();
    }

    return ret;
}

template< typename T >
void SerializeObjectImpl( const T& o, ISerializer& sob );

template< typename T >
std::shared_ptr< T > Create( const IDeserializer& sob )
{
    auto obj = T::Create( sob );
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

glm::vec4               String2Vec4( std::string s );
glm::vec3               String2Vec3( std::string s );
glm::vec2               String2Vec2( std::string s );

}

}