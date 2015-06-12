#include "SerializationObjects.h"

#include <rapidxml/RapidXml_Print.hpp>
#include <iostream>

namespace bv {

SerializeObject::SerializeObject()
{
    m_roots.push( &m_doc );
    m_curRoot = m_roots.top();
}

    void SerializeObject::Save( std::string filename )
{
    std::ofstream file( filename );
    //file << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
    file << m_doc;
    file.close();
    m_doc.clear();
}

void                                                    SerializeObject::SetName( std::string name )
{
    char *node_name = m_doc.allocate_string( name.c_str() );
    rapidxml::xml_node<>* node = m_doc.allocate_node( rapidxml::node_element, node_name );

    m_curRoot->append_node( node );
    m_roots.push( node );
    m_curRoot = m_roots.top();
}

void                                                    SerializeObject::SetValue( std::string name, std::string value )
{
    auto attr = m_doc.allocate_attribute( m_doc.allocate_string( name.c_str() ), m_doc.allocate_string( value.c_str() ) );
    m_curRoot->append_attribute( attr );
}

void                                                    SerializeObject::Push()
{
}


void                                                    SerializeObject::Pop()
{
    m_roots.pop();
    m_curRoot = m_roots.top();
}

}
