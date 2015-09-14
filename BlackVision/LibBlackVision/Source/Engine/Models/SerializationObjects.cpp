#include "SerializationObjects.h"
#include "DeserializeObjectImpl.h"

#include <rapidxml/RapidXml.hpp>
#include <rapidxml/RapidXml_Print.hpp>
#include <iostream>
#include <sstream>

#include "System/Path.h"

namespace bv {

class SerializeObjectImpl
{
public:
    rapidxml::xml_document<>                                m_doc;
    std::stack< rapidxml::xml_node<>* >                     m_roots;
};

SerializeObject::SerializeObject()
    : pimpl_( new SerializeObjectImpl() )
{
    auto& m_doc = pimpl_->m_doc;
    auto& m_roots = pimpl_->m_roots;

    m_roots.push( &m_doc );
}

SerializeObject::~SerializeObject()
{
    delete pimpl_;
}

void SerializeObject::Save( const std::string & filename )
{
    auto& m_doc = pimpl_->m_doc;

    std::ofstream file( filename );
    //file << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
    file << m_doc;
    file.close();
    m_doc.clear();
}

void SerializeObject::Save( std::ostream & out )
{
    auto& m_doc = pimpl_->m_doc;

    out << m_doc;
}

void                                                    SerializeObject::SetName( const std::string & name )
{
    auto& m_doc = pimpl_->m_doc;
    auto& m_roots = pimpl_->m_roots;

    char *node_name = m_doc.allocate_string( name.c_str() );
    rapidxml::xml_node<>* node = m_doc.allocate_node( rapidxml::node_element, node_name );

    m_roots.top()->append_node( node );
    m_roots.push( node );
}

void                                                    SerializeObject::SetValue( const std::string & name, const std::string & value )
{
    auto& m_doc = pimpl_->m_doc;
    auto& m_roots = pimpl_->m_roots;

    auto attr = m_doc.allocate_attribute( m_doc.allocate_string( name.c_str() ), m_doc.allocate_string( value.c_str() ) );
    m_roots.top()->append_attribute( attr );
}

void                                                    SerializeObject::SetContent( const std::string & value )
{
    auto& m_doc = pimpl_->m_doc;
    auto& m_roots = pimpl_->m_roots;

    char * node_name = m_doc.allocate_string( value.c_str() );
    m_roots.top()->value( node_name, value.size() );
}


void                                                    SerializeObject::Pop()
{
    auto& m_roots = pimpl_->m_roots;

    m_roots.pop();
}

DeserializeObject::DeserializeObject( std::string filename, model::TimelineManager* tm )
    : pimpl_( new DeserializeObjectImpl( filename, tm ) )
        { }

DeserializeObject::DeserializeObject( std::istream & in, SizeType numBytes, model::TimelineManager* tm )
    : pimpl_( new DeserializeObjectImpl( in, numBytes, tm ) )
{
}

DeserializeObject::DeserializeObject( DeserializeObjectImpl *pimpl )
    : pimpl_( pimpl )
{
}

DeserializeObject::~DeserializeObject()
{
    delete pimpl_;
}

DeserializeObjectImpl::DeserializeObjectImpl( std::string filename, model::TimelineManager* tm  )
    : m_rootDoc( new rapidxml::xml_document<>() )
    , m_tm( tm )
{
    assert( Path::Exists( filename ) );

    std::ifstream file( filename );
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    static std::string content( buffer.str() ); // FIXME
    m_rootDoc->parse<0>( &content[0] );
    m_doc = m_rootDoc->first_node();
}

DeserializeObjectImpl::DeserializeObjectImpl( std::istream & in, SizeType numBytes, model::TimelineManager* tm )
    : m_rootDoc( new rapidxml::xml_document<>() )
    , m_tm( tm )
{
    std::stringstream buffer;

    auto buf = new char[ numBytes + 1 ];

    buf[ numBytes ] = '\0';

    in.read( buf, numBytes );

    m_rootDoc->parse<0>( buf );
    m_doc = m_rootDoc->first_node();
}

DeserializeObjectImpl::DeserializeObjectImpl( rapidxml::xml_node<> * node, model::TimelineManager* tm )
    : m_doc( node )
    , m_tm( tm )
    , m_rootDoc( nullptr )
{
}

DeserializeObjectImpl::~DeserializeObjectImpl()
{
    delete m_rootDoc;
}


model::TimelineManager*                                 DeserializeObject::GetTimelineManager() 
{ 
    return pimpl_->m_tm; 
}

std::string                                             DeserializeObject::GetName()
{
    return pimpl_->m_doc->name();
}

std::string                                             DeserializeObject::GetValue( std::string name ) const
{
    auto node = pimpl_->m_doc->first_attribute( name.c_str() );
    assert( node ); // FIXME: error handling
    return node->value();
}

}
