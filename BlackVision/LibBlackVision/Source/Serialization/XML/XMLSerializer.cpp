#include "XMLSerializer.h"

#include <rapidxml/RapidXml.hpp>
#include <rapidxml/RapidXml_Print.hpp>
#include <fstream>
#include <stack>

#include "System/Path.h"

namespace bv {

// *******************************
//
SerializeObject::SerializeObject()
{
    m_roots.push( &m_doc );
}

// *******************************
//
SerializeObject::~SerializeObject()
{
}

// *******************************
//
void SerializeObject::Save( const std::string & filename )
{
    std::ofstream file( filename );
    //file << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
    file << m_doc;
    file.close();
    m_doc.clear();
}

// *******************************
//
void SerializeObject::Save( std::ostream & out )
{
    out << m_doc;
}

// *******************************
//
void                                                    SerializeObject::SetName( const std::string & name )
{
    char *node_name = m_doc.allocate_string( name.c_str() );
    rapidxml::xml_node<>* node = m_doc.allocate_node( rapidxml::node_element, node_name );

    m_roots.top()->append_node( node );
    m_roots.push( node );
}

// *******************************
//
void                                                    SerializeObject::SetValue( const std::string & name, const std::string & value )
{
    auto attr = m_doc.allocate_attribute( m_doc.allocate_string( name.c_str() ), m_doc.allocate_string( value.c_str() ) );
    m_roots.top()->append_attribute( attr );
}

// *******************************
//
void                                                    SerializeObject::SetContent( const std::string & value )
{
    char * node_name = m_doc.allocate_string( value.c_str() );
    m_roots.top()->value( node_name, value.size() );
}


// *******************************
//
void                                                    SerializeObject::Pop()
{
    m_roots.pop();
}

void				SerializeObject::SetAttribute        ( const std::string& name, const std::string& value )
{
    SetValue( name, value );
}

std::string			SerializeObject::GetAttribute        ( const std::string& name )
{
    return GetAttribute( name );
}

void                SerializeObject::EnterChild          ( const std::string& name )
{
    SetName( name );
    return ;
}

bool                SerializeObject::ExitChild           ()
{
    Pop();
    return true;
}


} // bv
