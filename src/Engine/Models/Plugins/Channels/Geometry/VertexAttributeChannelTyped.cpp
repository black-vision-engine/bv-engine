#include "VertexAttributeChannelTyped.h"

#include "Engine\Models\Plugins\Channels\Geometry\VertexAttributeChannelDescriptor.h"

namespace bv { namespace model
{

// *********************************
//
Float3VertexAttributeChannel::Float3VertexAttributeChannel  ( const VertexAttributeChannelDescriptor * desc, const std::string & name, bool readOnly )
    : VertexAttributeChannel( desc, name, readOnly )
{
    //FIXME: typed instances should be created via factories
    assert( desc->GetType() == AttributeType::AT_FLOAT3 );
    assert( sizeof( glm::vec3 ) == desc->GetEntrySize() );
}

// *********************************
//
Float3VertexAttributeChannel::~Float3VertexAttributeChannel   ()
{
}

// *********************************
//
bool                    Float3VertexAttributeChannel::IsReadOnly   () const
{
    return false;
}

// *********************************
//
unsigned int            Float3VertexAttributeChannel::GetNumEntries()   const
{
    return m_attributes.size();
}

// *********************************
//
const char *            Float3VertexAttributeChannel::GetData()         const
{
    if( m_attributes.empty() )
    {
        return nullptr;
    }
    else
    {
        return reinterpret_cast< const char * >( &m_attributes[ 0 ] );
    }
}

// *********************************
//
std::vector< glm::vec3 > &      Float3VertexAttributeChannel::GetVertices   ()
{
    return m_attributes;
}

// *********************************
//
void                    Float3VertexAttributeChannel::AddVertexAttribute( const glm::vec3 & v )
{
    m_attributes.push_back( v );
}



// *********************************
//
Float2VertexAttributeChannel::Float2VertexAttributeChannel  ( const VertexAttributeChannelDescriptor * desc, const std::string & name, bool readOnly )
    : VertexAttributeChannel( desc, name, readOnly )
{
    //FIXME: typed instances should be created via factories
    assert( desc->GetType() == AttributeType::AT_FLOAT2 );
    assert( sizeof( glm::vec2 ) == desc->GetEntrySize() );
}

// *********************************
//
bool                    Float2VertexAttributeChannel::IsReadOnly   () const
{
    return false;
}

// *********************************
//
unsigned int            Float2VertexAttributeChannel::GetNumEntries()   const
{
    return m_attributes.size();
}

// *********************************
//
const char *            Float2VertexAttributeChannel::GetData()         const
{
    if( m_attributes.empty() )
    {
        return nullptr;
    }
    else
    {
        return reinterpret_cast< const char * >( &m_attributes[ 0 ] );
    }
}

// *********************************
//
std::vector< glm::vec2 > &  Float2VertexAttributeChannel::GetVertices()
{
    return m_attributes;
}

// *********************************
//
void                    Float2VertexAttributeChannel::AddVertexAttribute ( const glm::vec2 & v )
{
    m_attributes.push_back(v);
}

} // model
} // bv
