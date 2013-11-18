#include "AttributeChannelDescriptor.h"

#include <sstream>
#include <cassert>


namespace bv { namespace model
{

const unsigned int AttributeChannelDescriptor::m_sAttrSize[ (int) AttributeType::AT_TOTAL ] = { sizeof( float ),     //AT_FLOAT1 = 0,
                                                                                                2 * sizeof( float ), //AT_FLOAT2,
                                                                                                3 * sizeof( float ), //AT_FLOAT3,
                                                                                                4 * sizeof( float )  //AT_FLOAT4
                                                                                               };

// ************************************
//
AttributeChannelDescriptor::AttributeChannelDescriptor    ( AttributeType attrType, AttributeSemantic attrSemantic, ChannelRole channelRole )
    : m_attrType( attrType )
    , m_attrSemantic( attrSemantic )
    , m_channelRole( channelRole )
{
}

// ************************************
//
AttributeChannelDescriptor::~AttributeChannelDescriptor   ()
{
}

// ************************************
//
AttributeType        AttributeChannelDescriptor::GetType                              ()  const
{
    return m_attrType;
}

// ************************************
//
AttributeSemantic    AttributeChannelDescriptor::GetSemantic                          ()  const
{
    return m_attrSemantic;
}

// ************************************
//
unsigned int    AttributeChannelDescriptor::GetEntrySize                              ()  const
{
    return GetAttrTypeSize( m_attrType );
}

// ************************************
//
ChannelRole     AttributeChannelDescriptor::GetChannelRole                            ()  const
{
    return m_channelRole;
}

// ************************************
//
std::string     AttributeChannelDescriptor::SuggestedDefaultName                      ( int num ) const
{
    return DefaultName( m_attrType, m_attrSemantic, num );
}

// ************************************
//
unsigned int    AttributeChannelDescriptor::GetAttrTypeSize                           ( AttributeType type )
{
    return m_sAttrSize[ (int) type ];
}

// ************************************
// FIXME: possibly also add attribyte type to this process
std::string     AttributeChannelDescriptor::DefaultName                               ( AttributeType type, AttributeSemantic semantic, int num )
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
