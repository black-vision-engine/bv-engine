#include "XMLDeserializer.h"

#include "System/Path.h"
#include <fstream>
#include <sstream>

#include "Serialization/BVDeserializeContext.h"

namespace bv {

// *******************************
//
XMLDeserializer::XMLDeserializer( std::string filename )
    : m_rootDoc( new rapidxml::xml_document<> )
    , m_context( std::unique_ptr< DeserializeContext >( new BVDeserializeContext() ) ) 
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
    , m_context( std::unique_ptr< DeserializeContext >( new BVDeserializeContext() ) ) 
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
DeserializeContext* XMLDeserializer::GetDeserializeContext() const
{
    return m_context.get();
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

std::string			XMLDeserializer::GetParentAttribute	  ( const std::string& /*parentName*/, const std::string& /*attName*/ ) const
{ 
    assert( !"FIXME" ); return "";
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


    if( m_doc )
    {
        m_nodes.pop();
        m_nodes.push( m_doc );
        return true;
    }
    else
        return false;
}


std::wstring        XMLDeserializer::GetAttribute        ( const std::wstring& /*name*/ ) const
{    assert( !"This serializer doesn't supports wstrings" ); return L"";    }

std::wstring        XMLDeserializer::GetParentAttribute	( const std::wstring& /*parentName*/, const std::wstring& /*attName*/ ) const
{    assert( !"This serializer doesn't supports wstrings" ); return L"";    }

bool                XMLDeserializer::EnterChild          ( const std::wstring& /*name*/ ) const
{    assert( !"This serializer doesn't supports wstrings" ); return false;    }


} // bv
