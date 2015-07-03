#pragma once

#include "rapidxml/RapidXml.hpp"
#include "Engine/Models/Timeline/TimelineManager.h"
#include <fstream>

#include <stack>

namespace bv
{

namespace model { class PluginsManager; }

class SerializeObject
{
    rapidxml::xml_document<>                                m_doc;
    std::stack< rapidxml::xml_node<>* >                     m_roots;

public:
    SerializeObject();
    void Save( const std::string & filename );

    void                                                    SetName( const std::string & name );
    void                                                    SetValue( const std::string & name, const std::string & value );
    void                                                    Pop();
};


class DeserializeObject
{
    rapidxml::xml_node<>* m_doc;

    template< typename T >
    std::shared_ptr< T >                                    Load( rapidxml::xml_node<>* node ) const
    {
        auto dob = DeserializeObject( *node, *this->m_tm, *this->m_pm ); // FIXME for God's sake!!!
        auto obj = T::Create( dob );
        return std::static_pointer_cast< T >( obj );
    }
public:
    model::TimelineManager* m_tm; // FIXME(?)
    const model::PluginsManager* m_pm; // FIXME(?)

public:
    DeserializeObject( rapidxml::xml_node<>& doc, model::TimelineManager& tm, const model::PluginsManager& pm ) : m_doc( &doc ), m_tm( &tm ), m_pm( &pm ) { }

    std::string                                             GetName()
    {
        return m_doc->name();
    }

    std::string                                             GetValue( std::string name ) const
    {
        auto node = m_doc->first_attribute( name.c_str() );
        assert( node ); // FIXME: error handling
        return node->value();
    }

    template< typename T >
    std::shared_ptr< T >                                    Load( std::string name ) const
    {
        auto node = m_doc->first_node( name.c_str() );
        assert( node ); // FIXME: error handling
        return Load< T >( node );
    }

    template< typename T >
    std::vector< std::shared_ptr< T > >                     LoadArray( std::string name ) const
    {
        std::vector< std::shared_ptr< T > > ret;

        auto children = m_doc->first_node( name.c_str() );

        if( children )
            for( auto child = children->first_node(); child; child = child->next_sibling() )
            {
                auto childNode = Load< T >( child );
                ret.push_back( childNode );
            }

        return ret;
    }

    template< typename T >
    std::vector< std::shared_ptr< T > >                     LoadProperties( std::string name ) const
    {
        std::vector< std::shared_ptr< T > > ret;

        for( auto child = m_doc->first_node(); child; child = child->next_sibling() )
        {
            if( !strcmp( child->name(), name.c_str() ) )
            {
                auto childNode = Load< T >( child );
                ret.push_back( childNode );
            }
        }

        return ret;
    }
};

}
