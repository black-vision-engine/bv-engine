#include "SerializationObjects.h"
#include "DeserializeObjectImpl.h"

#include "System/Path.h"

#include <fstream>

namespace bv {

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

} // bv
