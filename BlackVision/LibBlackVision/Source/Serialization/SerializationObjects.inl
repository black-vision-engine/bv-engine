#include "DeserializeObjectImpl.h"
#include "ISerializer.h"

namespace bv {

// *************************************
//
template< typename T >
std::shared_ptr< T >                                        DeserializeObjectLoadImpl( ISerializer& sob, std::string name )
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
std::vector< std::shared_ptr< T > >                         DeserializeObjectLoadArrayImpl( ISerializer& pimpl, std::string name )
{
    assert( false ); pimpl; name; return std::vector< std::shared_ptr< T > >();
    //auto& m_doc = pimpl->m_doc;

    //std::vector< std::shared_ptr< T > > ret;

    //auto children = m_doc->first_node( name.c_str() );

    //if( children )
    //    for( auto child = children->first_node(); child; child = child->next_sibling() )
    //    {
    //        auto childNode = pimpl->Load< T >( child );
    //        ret.push_back( childNode );
    //    }

    //return ret;
}

// *************************************
//
template< typename T >
std::vector< std::shared_ptr< T > >                         DeserializeObjectLoadPropertiesImpl( ISerializer& pimpl, std::string name )
{
    assert( false ); pimpl; name; return std::vector< std::shared_ptr< T > >();
    //auto& m_doc = pimpl->m_doc;

    //std::vector< std::shared_ptr< T > > ret;

    //for( auto child = m_doc->first_node(); child; child = child->next_sibling() )
    //{
    //    if( !strcmp( child->name(), name.c_str() ) )
    //    {
    //        auto childNode = pimpl->Load< T >( child );
    //        ret.push_back( childNode );
    //    }
    //}

    //return ret;
}

}