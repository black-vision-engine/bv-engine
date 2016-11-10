#pragma once

#include "CoreDEF.h"
#include <string>

#include "Engine/Models/Plugins/Interfaces/IAttributeChannelDescriptor.h"


namespace bv { namespace model
{

class AttributeChannelDescriptor;
DEFINE_PTR_TYPE( AttributeChannelDescriptor );

// FIXME: Czy kazdy typ moze miec kazda semantyke? I jak ewentualnie wymusic ograniczenia/wiezy?
class AttributeChannelDescriptor : public IAttributeChannelDescriptor
{
private:

    static const unsigned int m_sAttrSize[ (int) AttributeType::AT_TOTAL ];

    AttributeType       m_attrType;
    AttributeSemantic   m_attrSemantic;
    ChannelRole         m_channelRole;

public:

                                AttributeChannelDescriptor          ( AttributeType attrType, AttributeSemantic attrSemantic, ChannelRole channelRole );
    virtual                     ~AttributeChannelDescriptor         ();

    virtual AttributeType       GetType                             ()  const;
    virtual AttributeSemantic   GetSemantic                         ()  const;
    virtual unsigned int        GetEntrySize                        ()  const;

    virtual ChannelRole         GetChannelRole                      ()  const;

    //@@description: num argument means the number of exactly the same channel types already present (e.g. uv mapping channels - 
    //if two are already present, then this one should be third with num == 2 or numm == 3 depending on startng offset (zero or one))
    std::string                 SuggestedDefaultName                ( int num ) const;

    static unsigned int         GetAttrTypeSize                     ( AttributeType type );

    //@@description: num argument means the number of exactly the same channel types already present (e.g. uv mapping channels - 
    //if two are already present, then this one should be third with num == 2 or numm == 3 depending on startng offset (zero or one))
    static  std::string         DefaultName                         ( AttributeType type, AttributeSemantic semantic, int num );

};

} //model
} //bv
