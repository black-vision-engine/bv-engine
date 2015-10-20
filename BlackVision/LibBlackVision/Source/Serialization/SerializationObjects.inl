#include "DeserializeObjectImpl.h"
#include "ISerializer.h"

namespace bv {

// *************************************
//
template< typename T >
std::shared_ptr< T >                                        DeserializeObjectLoadImpl( const ISerializer& sob_, std::string name )
{
    auto& sob = const_cast< ISerializer& >( sob_ ); // FIXME OMFG

    auto sucess = sob.EnterChild( name );
    assert( sucess ); // FIXME error handling
    auto obj = T::Create( sob );
    sob.ExitChild();
    return std::static_pointer_cast< T >( obj );
}

// *************************************
//
template< typename T >
std::vector< std::shared_ptr< T > >                         DeserializeObjectLoadArrayImpl( const ISerializer& sob_, std::string name )
{
    auto& sob = const_cast< ISerializer& >( sob_ ); // FIXME OMFG

    std::vector< std::shared_ptr< T > > ret;

    bool success = sob.EnterChild( name );
    if( !success )
        return ret;

// remove trailing 's'
    assert( name[ name.size()-1 ] == 's' );
    name = name.substr( 0, name.size()-1 );

    int i = 0;
    while( sob.EnterChild( name, i++ ) )
    {
        auto childNode = DeserializeObjectLoadImpl< T >( sob, name );
        ret.push_back( childNode );
    sob.ExitChild();
    }

   sob.ExitChild();

    return ret;
}

// *************************************
//
template< typename T >
std::vector< std::shared_ptr< T > >                         DeserializeObjectLoadPropertiesImpl( const ISerializer& sob_, std::string name )
{
    auto& sob = const_cast< ISerializer& >( sob_ ); // FIXME OMFG

    std::vector< std::shared_ptr< T > > ret;

    int i = 0;
    while( sob.EnterChild( name, i++ ) )
    {
        auto child = DeserializeObjectLoadImpl< T >( sob, name );
        ret.push_back( child );
    sob.ExitChild();
    }

    return ret;
}

}