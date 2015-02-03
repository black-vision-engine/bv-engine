#include "AttributeChannelTyped.h"

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"


namespace bv { namespace model
{

// *********************************
//
Float3AttributeChannel::Float3AttributeChannel  ( const AttributeChannelDescriptor * desc, const std::string & name, bool readOnly )
    : AttributeChannel( desc, name, readOnly )
{
    //FIXME: typed instances should be created via factories
    assert( desc->GetType() == AttributeType::AT_FLOAT3 );
    assert( sizeof( glm::vec3 ) == desc->GetEntrySize() );
}

// *********************************
//
Float3AttributeChannel::~Float3AttributeChannel   ()
{
}

// *********************************
//
bool                    Float3AttributeChannel::IsReadOnly   () const
{
    return false;
}

// *********************************
//
unsigned int            Float3AttributeChannel::GetNumEntries()   const
{
    return (unsigned int) m_attributes.size();
}

// *********************************
//FIXME: this suxxxxx - of course vector is guaranteed to work this way (and reintepreted cast works) but this is a shitty practice and a sign of poorly designed code
const char *            Float3AttributeChannel::GetData()         const
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
std::vector< glm::vec3 > &      Float3AttributeChannel::GetVertices   ()
{
    return m_attributes;
}

// *********************************
//
void                    Float3AttributeChannel::AddAttribute( const glm::vec3 & v )
{
    m_attributes.push_back( v );
}



// *********************************
//
Float2AttributeChannel::Float2AttributeChannel  ( const AttributeChannelDescriptor * desc, const std::string & name, bool readOnly )
    : AttributeChannel( desc, name, readOnly )
{
    //FIXME: typed instances should be created via factories
    assert( desc->GetType() == AttributeType::AT_FLOAT2 );
    assert( sizeof( glm::vec2 ) == desc->GetEntrySize() );
}

// *********************************
//
bool                    Float2AttributeChannel::IsReadOnly   () const
{
    return false;
}

// *********************************
//
unsigned int            Float2AttributeChannel::GetNumEntries()   const
{
    return (unsigned int) m_attributes.size();
}

// *********************************
//
const char *            Float2AttributeChannel::GetData()         const
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
std::vector< glm::vec2 > &  Float2AttributeChannel::GetVertices()
{
    return m_attributes;
}

// *********************************
//
void                    Float2AttributeChannel::AddAttribute ( const glm::vec2 & v )
{
    m_attributes.push_back(v);
}

} // model
} // bv
