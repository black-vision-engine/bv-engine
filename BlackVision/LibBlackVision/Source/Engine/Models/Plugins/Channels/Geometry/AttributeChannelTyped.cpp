#include "stdafx.h"

#include "AttributeChannelTyped.h"

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model
{


//******************* Float4AttributeChannel *******************


// *********************************
//
Float4AttributeChannel::Float4AttributeChannel      ( AttributeChannelDescriptorConstPtr desc, const std::string & name, bool readOnly )
    : AttributeChannel( desc, name, readOnly )
{
    //FIXME: typed instances should be created via factories
    assert( desc->GetType() == AttributeType::AT_FLOAT4 );
    assert( sizeof( glm::vec4 ) == desc->GetEntrySize() );
}

// *********************************
//
Float4AttributeChannel::~Float4AttributeChannel     ()
{
}

// *********************************
//
void                    Float4AttributeChannel::Update          ( TimeType t )
{
    { t; }
}

// *********************************
//
bool                    Float4AttributeChannel::IsReadOnly      () const
{
    return false;
}

// *********************************
//
unsigned int            Float4AttributeChannel::GetNumEntries   () const
{
    return (unsigned int) m_attributes.size();
}

// *********************************
//FIXME: Of course vector is guaranteed to work this way (and reintepreted cast works) but this is sign of poorly designed code
const char *            Float4AttributeChannel::GetData         () const
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
std::vector< glm::vec4 > &  Float4AttributeChannel::GetVertices ()
{
    return m_attributes;
}

// *********************************
//
void                    Float4AttributeChannel::AddAttribute    ( const glm::vec4 & v )
{
    m_attributes.push_back( v );
}

// *********************************
//
void                    Float4AttributeChannel::AddAttributes   ( const std::vector< glm::vec4 > & v )
{
    m_attributes.insert( m_attributes.end(), v.begin(), v.end() );
}

// ***********************
//
void                    Float4AttributeChannel::ReplaceAttributes( std::vector< glm::vec4 > && v )
{
    m_attributes = std::move( v );
}


//******************* Float3AttributeChannel *******************


// *********************************
//
Float3AttributeChannel::Float3AttributeChannel  ( AttributeChannelDescriptorConstPtr desc, const std::string & name, bool readOnly )
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
//FIXME: Of course vector is guaranteed to work this way (and reintepreted cast works) but this is a sign of poorly designed code
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
void                    Float3AttributeChannel::AddAttributes( const std::vector< glm::vec3 > & v )
{
    m_attributes.insert( m_attributes.end(), v.begin(), v.end() );
}

// ***********************
//
void                    Float3AttributeChannel::ReplaceAttributes( std::vector< glm::vec3 > && v )
{
    m_attributes = std::move( v );
}

//******************* Float2AttributeChannel *******************


// *********************************
//
Float2AttributeChannel::Float2AttributeChannel  ( AttributeChannelDescriptorConstPtr desc, const std::string & name, bool readOnly )
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

// *********************************
//
void                    Float2AttributeChannel::AddAttributes ( const std::vector< glm::vec2 > & v )
{
    m_attributes.insert( m_attributes.end(), v.begin(), v.end() );
}

// ***********************
//
void                    Float2AttributeChannel::ReplaceAttributes   ( std::vector< glm::vec2 >&& v )
{
    m_attributes = std::move( v );
}

} // model
} // bv
