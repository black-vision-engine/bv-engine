#include "XMLDeserializer.h"

#include "System/Path.h"
#include <fstream>
#include <sstream>

namespace bv {

// *******************************
//
XMLDeserializer::XMLDeserializer( std::string filename )
    : m_rootDoc( new rapidxml::xml_document<> )
{ 
    assert( Path::Exists( filename ) );

    std::ifstream file( filename );
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    static std::string content( buffer.str() ); // FIXME
    m_rootDoc->parse<0>( &content[0] );
    m_doc = m_rootDoc;

    m_nodes.push( m_doc );
}

// *******************************
//
XMLDeserializer::XMLDeserializer( std::istream & in, SizeType numBytes )
    : m_rootDoc( new rapidxml::xml_document<> )
{
    std::stringstream buffer;

    auto buf = new char[ numBytes + 1 ];

    buf[ numBytes ] = '\0';

    in.read( buf, numBytes );

    m_rootDoc->parse<0>( buf );
    m_doc = m_rootDoc;

    m_nodes.push( m_doc );
}

// *******************************
//
XMLDeserializer::~XMLDeserializer()
{
}


// *******************************
//
std::string                                             XMLDeserializer::GetName()
{
    return GetDoc()->name();
}

// *******************************
//
std::string                                             XMLDeserializer::GetValue( std::string name ) const
{
    auto node = GetDoc()->first_attribute( name.c_str() );
    if( node == nullptr )
        return "";
    return node->value();
}


std::string			XMLDeserializer::GetAttribute        ( const std::string& name ) const
{ 
    return GetValue( name );
}

bool                XMLDeserializer::EnterChild          ( const std::string& name ) const
{ 
    auto child = m_doc->first_node( name.c_str() );
    if( child == nullptr )
        return false;

    m_nodes.push( child );
    m_doc = child;

    return true;
}

bool                XMLDeserializer::ExitChild           () const
{ 
    if( m_nodes.size() > 0 )
    {
        m_nodes.pop();
        m_doc = m_nodes.top();
        return true;
    }
    else
        return false;
}

bool                XMLDeserializer::NextChild           () const
{
    auto name = m_doc->name();
    do {
        m_doc = m_doc->next_sibling();
    }while( m_doc && strcmp( name, m_doc->name() ) );

    m_nodes.pop();
    m_nodes.push( m_doc );

    if( m_doc )
        return true;
    else
        return false;
}


std::wstring        XMLDeserializer::GetAttribute        ( const std::wstring& /*name*/ ) const
{    assert( !"This serializer doesn't supports wstrings" ); return L"";    }

bool                XMLDeserializer::EnterChild          ( const std::wstring& /*name*/ ) const
{    assert( !"This serializer doesn't supports wstrings" ); return false;    }


} // bv
