#include "SerializationObjects.h"

#include <rapidxml/RapidXml_Print.hpp>
#include <iostream>

namespace bv {

SerializeObject::SerializeObject()
{
    m_roots.push( &m_doc );
}

void SerializeObject::Save( const std::string & filename )
{
    std::ofstream file( filename );
    //file << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
    file << m_doc;
    file.close();
    m_doc.clear();
}

void SerializeObject::Save( std::ostream & out )
{
    out << m_doc;
}

//void                                                    SerializeObject::SetAssetsWithUIDs( AssetDescsWithUIDs* assets ) // FIXME singletonize
//{
//    m_assets = assets;
//}
//
//AssetDescsWithUIDs*                                     SerializeObject::GetAssetsWithUIDs() // FIXME singletonize
//{
//    return m_assets;
//}


void                                                    SerializeObject::SetName( const std::string & name )
{
    char *node_name = m_doc.allocate_string( name.c_str() );
    rapidxml::xml_node<>* node = m_doc.allocate_node( rapidxml::node_element, node_name );

    m_roots.top()->append_node( node );
    m_roots.push( node );
}

void                                                    SerializeObject::SetValue( const std::string & name, const std::string & value )
{
    auto attr = m_doc.allocate_attribute( m_doc.allocate_string( name.c_str() ), m_doc.allocate_string( value.c_str() ) );
    m_roots.top()->append_attribute( attr );
}

void                                                    SerializeObject::SetContent( const std::string & value )
{
    char * node_name = m_doc.allocate_string( value.c_str() );
    m_roots.top()->value( node_name, value.size() );
}


void                                                    SerializeObject::Pop()
{
    m_roots.pop();
}

//void                                                    DeserializeObject::SetAssetsWithUIDs( AssetDescsWithUIDs* assets ) // FIXME singletonize
//{
//    m_assets = assets;
//}
//
//AssetDescsWithUIDs*                                     DeserializeObject::GetAssetsWithUIDs() // FIXME singletonize
//{
//    return m_assets;
//}

}
