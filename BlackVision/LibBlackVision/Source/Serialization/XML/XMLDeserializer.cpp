#include "XMLDeserializer.h"

#include "System/Path.h"
#include <fstream>
#include <sstream>

namespace bv {

// *******************************
//
DeserializeObject::DeserializeObject( std::string filename )
    : m_rootDoc( new rapidxml::xml_document<> )
{ 
    assert( Path::Exists( filename ) );

    std::ifstream file( filename );
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    static std::string content( buffer.str() ); // FIXME
    m_rootDoc->parse<0>( &content[0] );
    m_doc = m_rootDoc->first_node();

    m_nodes.push( m_doc );
}

// *******************************
//
DeserializeObject::DeserializeObject( std::istream & in, SizeType numBytes )
    : m_rootDoc( new rapidxml::xml_document<> )
{
    std::stringstream buffer;

    auto buf = new char[ numBytes + 1 ];

    buf[ numBytes ] = '\0';

    in.read( buf, numBytes );

    m_rootDoc->parse<0>( buf );
    m_doc = m_rootDoc->first_node();

    m_nodes.push( m_doc );
}

//// *******************************
////
//DeserializeObject::DeserializeObject( DeserializeObjectImpl *pimpl )
//    //: pimpl_( pimpl )
//{
//}

// *******************************
//
DeserializeObject::~DeserializeObject()
{
    //delete pimpl_;
}


// *******************************
//
std::string                                             DeserializeObject::GetName()
{
    return GetDoc()->name();
}

// *******************************
//
std::string                                             DeserializeObject::GetValue( std::string name ) const
{
    auto node = GetDoc()->first_attribute( name.c_str() );
    assert( node ); // FIXME: error handling
    return node->value();
}


//void				DeserializeObject::SetAttribute        ( const std::string& /*name*/, const std::string& /*value*/ ) 
//{
//    assert( false );
//}

std::string			DeserializeObject::GetAttribute        ( const std::string& name ) const
{ 
    return GetValue( name );
}

bool                DeserializeObject::EnterChild          ( const std::string& name ) const
{ 
// FIXME this implementation sucks so much
    auto child = m_doc->first_node( name.c_str() );
    if( child == nullptr )
        return false;

    m_nodes.push( child );
    m_doc = child;

    return true;
}

bool                DeserializeObject::ExitChild           () const
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

bool                DeserializeObject::NextChild           () const
{
    m_doc = m_doc->next_sibling();

    m_nodes.pop();
    m_nodes.push( m_doc );

    if( m_doc )
        return true;
    else
        return false;
}


} // bv
