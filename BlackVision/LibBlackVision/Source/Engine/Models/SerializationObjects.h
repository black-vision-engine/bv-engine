#pragma once

#include "CoreDEF.h"

#include "rapidxml/RapidXml.hpp" // FIXME

#include <fstream>
#include <stack>

namespace bv
{

//class AssetDescsWithUIDs;
namespace model { class TimelineManager; }

class SerializeObject
{
    rapidxml::xml_document<>                                m_doc;
    std::stack< rapidxml::xml_node<>* >                     m_roots; // FIXME: Move it to implementation class to prevent including RapidXml.hpp in every header using this class.
    //AssetDescsWithUIDs*                                     m_assets;

public:
    SerializeObject();
    void										            Save( const std::string & filename );
	void										            Save( std::ostream & out );

    //void                                                    SetAssetsWithUIDs( AssetDescsWithUIDs* );
    //AssetDescsWithUIDs*                                     GetAssetsWithUIDs();

    void                                                    SetName( const std::string & name );
    void                                                    SetValue( const std::string & name, const std::string & value );
	void                                                    SetContent( const std::string & value );
    void                                                    Pop();
};


class DeserializeObject
{
    rapidxml::xml_node<>*                                   m_doc;

    template< typename T >
    std::shared_ptr< T >                                    Load( rapidxml::xml_node<>* node ) const
    {
        auto dob = DeserializeObject( node, this->m_tm ); // FIXME for God's sake!!!
        auto obj = T::Create( dob );
        return std::static_pointer_cast< T >( obj );
    }

    model::TimelineManager* m_tm;
    //AssetDescsWithUIDs*                                     m_assets;

public:
    DeserializeObject( rapidxml::xml_node<>* doc, model::TimelineManager* tm ) : m_doc( doc ), m_tm( tm ) { }

    model::TimelineManager*                                 GetTimelineManager() { return m_tm; }
    //void                                                    SetAssetsWithUIDs( AssetDescsWithUIDs* );
    //AssetDescsWithUIDs*                                     GetAssetsWithUIDs();

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
