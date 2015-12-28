#include "XMLSerializer.h"

#include <rapidxml/RapidXml.hpp>
#include <rapidxml/RapidXml_Print.hpp>
#include <fstream>
#include <stack>
#include "Serialization/BVSerializeContext.h"

#include "System/Path.h"

namespace bv {

// *******************************
//
XMLSerializer::XMLSerializer()
    : m_context( std::unique_ptr< SerializeContext >( new BVSerializeContext() ) )
{
    m_roots.push( &m_doc );
}

// *******************************
//
XMLSerializer::~XMLSerializer()
{
}

// *******************************
//
void XMLSerializer::Save( const std::string & filename )
{
    std::ofstream file( filename );
    //file << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
    file << m_doc;
    file.close();
    m_doc.clear();
}

// *******************************
//
void XMLSerializer::Save( std::ostream & out )
{
    out << m_doc;
}

// *******************************
//
SerializeContext* XMLSerializer::GetSerializeContext() const
{
    return m_context.get();
}

// *******************************
//
void                                                    XMLSerializer::SetName( const std::string & name )
{
    char *node_name = m_doc.allocate_string( name.c_str() );
    rapidxml::xml_node<>* node = m_doc.allocate_node( rapidxml::node_element, node_name );

    m_roots.top()->append_node( node );
    m_roots.push( node );
}

// *******************************
//
void                                                    XMLSerializer::SetValue( const std::string & name, const std::string & value )
{
    auto attr = m_doc.allocate_attribute( m_doc.allocate_string( name.c_str() ), m_doc.allocate_string( value.c_str() ) );
    m_roots.top()->append_attribute( attr );
}

// *******************************
//
void                                                    XMLSerializer::SetContent( const std::string & value )
{
    char * node_name = m_doc.allocate_string( value.c_str() );
    m_roots.top()->value( node_name, value.size() );
}


// *******************************
//
void                                                    XMLSerializer::Pop()
{
    m_roots.pop();
}

void				XMLSerializer::SetAttribute        ( const std::string& name, const std::string& value )
{
    SetValue( name, value );
}

std::string			XMLSerializer::GetAttribute        ( const std::string& name )
{
    return GetAttribute( name );
}

void                XMLSerializer::EnterChild          ( const std::string& name )
{
    SetName( name );
    return ;
}

bool                XMLSerializer::ExitChild           ()
{
    Pop();
    return true;
}

// ***********************
//
void                XMLSerializer::SetAttribute        ( const std::wstring& /*name*/, const std::wstring& /*value*/ )
{    assert( !"This serializer doesn't supports wstrings" );    }

// ***********************
//
std::wstring        XMLSerializer::GetAttribute        ( const std::wstring& /*name*/ )
{    assert( !"This serializer doesn't supports wstrings" ); return L"";    }

// ***********************
//
void                XMLSerializer::EnterChild          ( const std::wstring& /*name*/ )
{    assert( !"This serializer doesn't supports wstrings" );    }

// ***********************
//
void                XMLSerializer::EnterArray          ( const std::wstring& /*name*/ )
{    assert( !"This serializer doesn't supports wstrings" );    }

// ***********************
//
void                XMLSerializer::EnterArray          ( const std::string& name )
{
    SetName( name );
}

} // bv
