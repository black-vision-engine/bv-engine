#pragma once

#include "Engine/Models/Plugins/Interfaces/IVertexAttributeChannelDescriptor.h"

namespace bv { namespace model
{

class VertexAttributeChannelDescriptor : public IVertexAttributeChannelDescriptor
{
private:

    static const int m_sAttrSizea[ (int) AttributeType::AT_TOTAL ];

    AttributeType       m_attrType;
    AttributeSemantic   m_attrSemantic;
    ChannelRole         m_channelRole;

public:

                                VertexAttributeChannelDescriptor    ( AttributeType attrType, AttributeSemantic attrSemantic, ChannelRole channelRole );
    virtual                     ~VertexAttributeChannelDescriptor   ();

    virtual AttributeType       GetType                             ()  const;
    virtual AttributeSemantic   GetSemantic                         ()  const;
    virtual int                 GetEntrySize                        ()  const;

    virtual ChannelRole         GetChannelRole                      ()  const;

    static  int                 GetAttrTypeSize                     ( AttributeType type );

};

} //model
} //bv
