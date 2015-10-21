#pragma once

#include "DeserializeObjectImpl.h"
#include "IDeserializer.h"

namespace bv {

// *************************************
//
template< typename T >
std::shared_ptr< T >                                        DeserializeObjectLoadImpl( const IDeserializer& sob_, std::string name )
{
    auto& sob = const_cast< IDeserializer& >( sob_ ); // FIXME OMFG

    auto sucess = sob.EnterChild( name );
    assert( sucess ); // FIXME error handling
    auto obj = T::Create( sob );
    sob.ExitChild();
    return std::static_pointer_cast< T >( obj );
}

// *************************************
//
template< typename T >
std::vector< std::shared_ptr< T > >                         DeserializeObjectLoadArrayImpl( const IDeserializer& sob_, std::string nameParent, std::string nameChild="" )
{
    auto& sob = const_cast< IDeserializer& >( sob_ ); // FIXME OMFG

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

    //int i = 0;
    sob.EnterChild( name );
    while( sob.NextChild() )
    {
        auto obj = T::Create( sob );
        ret.push_back( std::static_pointer_cast< T >( obj ) );
        //sob.ExitChild(); // return to parent
    }
    sob.ExitChild(); // nameChild

    sob.ExitChild(); // nameParent
    return ret;
}

// *************************************
//
template< typename T >
std::vector< std::shared_ptr< T > >                         DeserializeObjectLoadPropertiesImpl( const IDeserializer& sob_, std::string name )
{
    auto& sob = const_cast< IDeserializer& >( sob_ ); // FIXME OMFG

    std::vector< std::shared_ptr< T > > ret;

    //int i = 0;
    sob.EnterChild( name );
    while( sob.NextChild( ) )
    {
        auto obj = T::Create( sob );

        ret.push_back( std::static_pointer_cast< T >( obj ) );
        //sob.ExitChild();
    }
    sob.ExitChild();

    return ret;
}

}