#include "SerializationObjects.h"

#include <rapidxml/RapidXml.hpp>
#include <rapidxml/RapidXml_Print.hpp>
#include <fstream>
#include <stack>

#include "System/Path.h"

namespace bv {

class SerializeObjectImpl
{
public:
    rapidxml::xml_document<>                                m_doc;
    std::stack< rapidxml::xml_node<>* >                     m_roots;
};

// *******************************
//
SerializeObject::SerializeObject()
    : pimpl_( new SerializeObjectImpl() )
{
    auto& m_doc = pimpl_->m_doc;
    auto& m_roots = pimpl_->m_roots;

    m_roots.push( &m_doc );
}

// *******************************
//
SerializeObject::~SerializeObject()
{
    delete pimpl_;
}

// *******************************
//
void SerializeObject::Save( const std::string & filename )
{
    auto& m_doc = pimpl_->m_doc;

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
    auto& m_doc = pimpl_->m_doc;

    out << m_doc;
}

// *******************************
//
//void                                                    SerializeObject::SetName( const std::string & name )
//{
//    auto& m_doc = pimpl_->m_doc;
//    auto& m_roots = pimpl_->m_roots;
//
//    char *node_name = m_doc.allocate_string( name.c_str() );
//    rapidxml::xml_node<>* node = m_doc.allocate_node( rapidxml::node_element, node_name );
//
//    m_roots.top()->append_node( node );
//    m_roots.push( node );
//}

// *******************************
//
void                                                    SerializeObject::SetValue( const std::string & name, const std::string & value )
{
    auto& m_doc = pimpl_->m_doc;
    auto& m_roots = pimpl_->m_roots;

    auto attr = m_doc.allocate_attribute( m_doc.allocate_string( name.c_str() ), m_doc.allocate_string( value.c_str() ) );
    m_roots.top()->append_attribute( attr );
}

// *******************************
//
void                                                    SerializeObject::SetContent( const std::string & value )
{
    auto& m_doc = pimpl_->m_doc;
    auto& m_roots = pimpl_->m_roots;

    char * node_name = m_doc.allocate_string( value.c_str() );
    m_roots.top()->value( node_name, value.size() );
}


// *******************************
//
void                                                    SerializeObject::Pop()
{
    auto& m_roots = pimpl_->m_roots;

    m_roots.pop();
}

} // bv
