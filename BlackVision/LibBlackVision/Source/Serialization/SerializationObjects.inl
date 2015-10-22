#include "DeserializeObjectImpl.h"

namespace bv {

// *************************************
//
template< typename T >
std::shared_ptr< T >                                        DeserializeObjectLoadImpl( DeserializeObjectImpl* pimpl, std::string name )
{
    auto& m_doc = pimpl->m_doc;

    auto node = m_doc->first_node( name.c_str() );
    assert( node ); // FIXME: error handling
    return pimpl->Load< T >( node );
}

// *************************************
//
template< typename T >
std::vector< std::shared_ptr< T > >                         DeserializeObjectLoadArrayImpl( DeserializeObjectImpl* pimpl, std::string name )
{
    auto& m_doc = pimpl->m_doc;

    std::vector< std::shared_ptr< T > > ret;

    auto children = m_doc->first_node( name.c_str() );

    if( children )
        for( auto child = children->first_node(); child; child = child->next_sibling() )
        {
            auto childNode = pimpl->Load< T >( child );
            ret.push_back( childNode );
        }

    return ret;
}

// *************************************
//
template< typename T >
std::vector< std::shared_ptr< T > >                         DeserializeObjectLoadPropertiesImpl( DeserializeObjectImpl* pimpl, std::string name )
{
    auto& m_doc = pimpl->m_doc;

    std::vector< std::shared_ptr< T > > ret;

    for( auto child = m_doc->first_node(); child; child = child->next_sibling() )
    {
        if( !strcmp( child->name(), name.c_str() ) )
        {
            auto childNode = pimpl->Load< T >( child );
            ret.push_back( childNode );
        }
    }

    return ret;
}

}