#pragma once

#include <string>
#include "Engine/Models/Plugins/Interfaces/IVertexAttributeChannelDescriptor.h"

namespace bv { namespace model
{

// FIXME: Czy kazdy typ moze miec kazda semantyke? I jak ewentualnie wymusic ograniczenia/wiezy?
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

    //@@description: num argument means the number of exactly the same channel types already present (e.g. uv mapping channels - 
    //if two are already present, then this one should be third with num == 2 or numm == 3 depending on startng offset (zero or one))
    std::string                 SuggestedDefaultName                ( int num ) const;

    static  int                 GetAttrTypeSize                     ( AttributeType type );

    //@@description: num argument means the number of exactly the same channel types already present (e.g. uv mapping channels - 
    //if two are already present, then this one should be third with num == 2 or numm == 3 depending on startng offset (zero or one))
    static  std::string         DefaultName                         ( AttributeType type, AttributeSemantic semantic, int num );

};

} //model
} //bv
