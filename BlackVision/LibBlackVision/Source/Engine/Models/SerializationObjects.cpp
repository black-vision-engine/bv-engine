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

void                                                    SerializeObject::Pop()
{
    m_roots.pop();
}

}
