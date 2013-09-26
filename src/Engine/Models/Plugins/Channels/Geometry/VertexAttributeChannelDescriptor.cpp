#include "VertexAttributeChannelDescriptor.h"

#include <sstream>
#include <cassert>

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
std::string     VertexAttributeChannelDescriptor::SuggestedDefaultName                      ( int num ) const
{
    return DefaultName( m_attrType, m_attrSemantic, num );
}

// ************************************
//
int             VertexAttributeChannelDescriptor::GetAttrTypeSize                           ( AttributeType type )
{
    return m_sAttrSizea[ (int) type ];
}

// ************************************
// FIXME: possibly also add attribyte type to this process
std::string     VertexAttributeChannelDescriptor::DefaultName                               ( AttributeType type, AttributeSemantic semantic, int num )
{
    assert( num >= 0 );

    std::ostringstream oss;
    
    switch( semantic )
    {
        case AttributeSemantic::AS_POSITION:
            oss << "pos";
            break;
        case AttributeSemantic::AS_TEXCOORD:
            oss << "tex";
            break;
        case AttributeSemantic::AS_COLOR:
            oss << "col";
            break;
        case AttributeSemantic::AS_NORMAL:
            oss << "norm";
            break;
        case AttributeSemantic::AS_BINORMAL:
            oss << "binorm";
            break;
        case AttributeSemantic::AS_CUSTOM:
            oss << "custom";
            break;
        default:
            assert( false );
    }

    oss << num;

    return oss.str();
}

} //model
} //bv
