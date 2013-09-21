#include "VertexAttributeChannel.h"

namespace bv { namespace model
{

// *********************************
//
VertexAttributeChannel::VertexAttributeChannel(AttrType type, AttrSemantic semantic)
    : m_type(type), m_semantic(semantic)
{}

// *********************************
//
AttrType                VertexAttributeChannel::GetType()               const
{
    return m_type;
}

// *********************************
//
AttrSemantic            VertexAttributeChannel::GetSemantic()           const
{
    return m_semantic;
}

// *********************************
//
Float3VertexAttributeChannel::Float3VertexAttributeChannel(AttrSemantic semantic)
    : VertexAttributeChannel( AttrType::AT_FLOAT3, semantic )
{}

// *********************************
//
int                     Float3VertexAttributeChannel::GetNumEntries()   const
{
    return m_f3attritutes.size();
}

// *********************************
//
const char*             Float3VertexAttributeChannel::GetData()         const
{
    if( m_f3attritutes.empty() )
    {
        return nullptr;
    }
    else
    {
        return reinterpret_cast<const char*>( &m_f3attritutes[0] );
    }
}

// *********************************
//
void                    Float3VertexAttributeChannel::AddVertexAttribute(const glm::vec3& v)
{
    m_f3attritutes.push_back(v);
}

// *********************************
//
Float2VertexAttributeChannel::Float2VertexAttributeChannel(AttrSemantic semantic)
    : VertexAttributeChannel( AttrType::AT_FLOAT2, semantic )
{}

// *********************************
//
int                     Float2VertexAttributeChannel::GetNumEntries()   const
{
    return m_f2attritutes.size();
}

// *********************************
//
const char*             Float2VertexAttributeChannel::GetData()         const
{
    if( m_f2attritutes.empty() )
    {
        return nullptr;
    }
    else
    {
        return reinterpret_cast<const char*>( &m_f2attritutes[0] );
    }
}

// *********************************
//
void                    Float2VertexAttributeChannel::AddVertexAttribute(const glm::vec2& v)
{
    m_f2attritutes.push_back(v);
}

} // model
} // bv