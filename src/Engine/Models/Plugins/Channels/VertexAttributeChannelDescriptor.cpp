#include "VertexAttributeChannelDescriptor.h"

namespace bv { namespace model
{

const int VertexAttributeChannelDescriptor::m_sAttrSizea[ (int) AttributeType::AT_TOTAL ] = { sizeof( float ),     //AT_FLOAT1 = 0,
                                                                                              2 * sizeof( float ), //AT_FLOAT2,
                                                                                              3 * sizeof( float ), //AT_FLOAT3,
                                                                                              4 * sizeof( float )  //AT_FLOAT4
                                                                                            };

// ************************************
//
VertexAttributeChannelDescriptor::VertexAttributeChannelDescriptor    ( AttributeType attrType, AttributeSemantic attrSemantic, ChannelRole channelRole )
    : m_attrType( attrType )
    , m_attrSemantic( attrSemantic )
    , m_channelRole( channelRole )
{
}

// ************************************
//
VertexAttributeChannelDescriptor::~VertexAttributeChannelDescriptor   ()
{
}

// ************************************
//
AttributeType        VertexAttributeChannelDescriptor::GetType                              ()  const
{
    return m_attrType;
}

// ************************************
//
AttributeSemantic    VertexAttributeChannelDescriptor::GetSemantic                          ()  const
{
    return m_attrSemantic;
}

// ************************************
//
int             VertexAttributeChannelDescriptor::GetEntrySize                              ()  const
{
    return GetAttrTypeSize( m_attrType );
}

// ************************************
//
ChannelRole     VertexAttributeChannelDescriptor::GetChannelRole                            ()  const
{
    return m_channelRole;
}

// ************************************
//
int             VertexAttributeChannelDescriptor::GetAttrTypeSize                           ( AttributeType type )
{
    return m_sAttrSizea[ (int) type ];
}

} //model
} //bv
