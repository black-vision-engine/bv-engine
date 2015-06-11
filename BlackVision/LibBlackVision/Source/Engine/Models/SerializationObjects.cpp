#include "SerializationObjects.h"

#include <rapidxml/RapidXml_Print.hpp>


namespace bv {

// *******************************
//
void SerializeObject::Save( std::string filename )
{
    std::ofstream file( filename );

    file << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
    file << m_doc;
    file.close();
    
	m_doc.clear();
}

// *******************************
//
void	SerializeObject::SetName( std::string name )
{
    char * node_name = m_doc.allocate_string( name.c_str() );
    
	rapidxml::xml_node<> * node = m_doc.allocate_node( rapidxml::node_element, node_name );
    
	m_doc.append_node( node );
}

// *******************************
//
void	SerializeObject::SetValue( std::string name, std::string value )
{
}

} // bv
