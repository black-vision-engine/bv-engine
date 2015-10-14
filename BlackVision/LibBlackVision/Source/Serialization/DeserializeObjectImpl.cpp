#include "DeserializeObjectImpl.h"

#include "System/Path.h"

#include <fstream>
#include <cassert>

namespace bv {

DeserializeObjectImpl::DeserializeObjectImpl( std::string filename  )
    : m_rootDoc( new rapidxml::xml_document<>() )
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

DeserializeObjectImpl::DeserializeObjectImpl( std::istream & in, SizeType numBytes )
    : m_rootDoc( new rapidxml::xml_document<>() )
{
    std::stringstream buffer;

    auto buf = new char[ numBytes + 1 ];

    buf[ numBytes ] = '\0';

    in.read( buf, numBytes );

    m_rootDoc->parse<0>( buf );
    m_doc = m_rootDoc->first_node();
}

DeserializeObjectImpl::DeserializeObjectImpl( rapidxml::xml_node<> * node )
    : m_doc( node )
    , m_rootDoc( nullptr )
{
}

DeserializeObjectImpl::~DeserializeObjectImpl()
{
    delete m_rootDoc;
}


}
