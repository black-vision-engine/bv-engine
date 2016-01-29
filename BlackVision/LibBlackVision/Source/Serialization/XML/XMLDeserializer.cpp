#include "stdafx.h"

#include "XMLDeserializer.h"

#include "System/Path.h"
#include "IO/FileIO.h"

#include "Serialization/BVDeserializeContext.h"

#include "rapidxml/RapidXml.hpp"

namespace bv {

// *******************************
//
XMLDeserializer::XMLDeserializer        ( const std::string & fileName, DeserializeContext * context )
    : m_rootDoc( new rapidxml::xml_document<> )
    , m_context( std::unique_ptr< DeserializeContext >( context ) ) 
{ 
    assert( Path::Exists( fileName ) );

    auto size = File::Size( fileName );

    m_buf = new char[ size + 1 ];

    auto bytesRead = File::Read( m_buf, fileName );
    { bytesRead; }

    m_buf[ size ] = '\0';

    m_rootDoc->parse<0>( m_buf );
    m_doc = m_rootDoc;

    m_nodes.push( m_doc );
}

// *******************************
//
XMLDeserializer::XMLDeserializer        ( std::istream & in, SizeType numBytes, DeserializeContext * context )
    : m_rootDoc( new rapidxml::xml_document<> )
    , m_context( std::unique_ptr< DeserializeContext >( context ) ) 
{
    m_buf = new char[ numBytes + 1 ];

    m_buf[ numBytes ] = '\0';

    in.read( m_buf, numBytes );

    m_rootDoc->parse<0>( m_buf );
    m_doc = m_rootDoc;

    m_nodes.push( m_doc );
}

// *******************************
//
XMLDeserializer::~XMLDeserializer()
{
    delete m_rootDoc;
    delete [] m_buf;
}

// *******************************
//
DeserializeContext *                                    XMLDeserializer::GetDeserializeContext  () const
{
    return m_context.get();
}

// *******************************
//
std::string                                             XMLDeserializer::GetName                () const
{
    return GetDoc()->name();
}

// *******************************
//
XMLDeserializer::XmlNode *                              XMLDeserializer::GetDoc                  () const 
{
    return m_nodes.top();
}

// *******************************
//
std::string                                             XMLDeserializer::GetValue               ( std::string name ) const
{
    auto node = GetDoc()->first_attribute( name.c_str() );
    if( node == nullptr )
    {
        return "";
    }

    return node->value();
}

// *******************************
//
std::string			                                    XMLDeserializer::GetAttribute           ( const std::string & name ) const
{ 
    return GetValue( name );
}

// *******************************
//
std::string			                                    XMLDeserializer::GetParentAttribute	    ( const std::string & /*parentName*/, const std::string & /*attName*/ ) const
{ 
    assert( !"FIXME" );
    return "";
}

// *******************************
//
bool                                                    XMLDeserializer::EnterChild             ( const std::string & name ) const
{ 
    auto child = m_doc->first_node( name.c_str() );
    if( child == nullptr )
    {
        return false;
    }

    m_nodes.push( child );
    m_doc = child;

    return true;
}

// *******************************
//
bool                                                    XMLDeserializer::ExitChild              () const
{ 
    if( m_nodes.size() > 0 )
    {
        m_nodes.pop();
        m_doc = m_nodes.top();
        return true;
    }
    else
    {
        return false;
    }
}

// *******************************
//
bool                                                    XMLDeserializer::NextChild              () const
{
    auto name = m_doc->name();
    
    do
    {
        m_doc = m_doc->next_sibling();
    } 
    while( m_doc && strcmp( name, m_doc->name() ) );

    if( m_doc )
    {
        m_nodes.pop();
        m_nodes.push( m_doc );
        return true;
    }
    else
    {
        return false;
    }
}

// *******************************
//
std::wstring                                            XMLDeserializer::GetAttribute           ( const std::wstring & /*name*/ ) const
{    
    assert( !"This serializer doesn't supports wstrings" );
    return L"";
}

// *******************************
//
std::wstring                                            XMLDeserializer::GetParentAttribute	    ( const std::wstring & /*parentName*/, const std::wstring & /*attName*/ ) const
{   
    assert( !"This serializer doesn't supports wstrings" ); 
    return L"";
}

// *******************************
//
bool                                                    XMLDeserializer::EnterChild             ( const std::wstring & /*name*/ ) const
{    
    assert( !"This serializer doesn't supports wstrings" ); 
    return false;
}


} // bv
