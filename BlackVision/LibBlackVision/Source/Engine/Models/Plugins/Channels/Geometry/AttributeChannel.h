#pragma once

#include <vector>
#include "Engine/Models/Plugins/Interfaces/IAttributeChannel.h"


namespace bv { namespace model
{

class AttributeChannelDescriptor;

class AttributeChannel : public IAttributeChannel
{
private:

    const AttributeChannelDescriptor *  m_desc;
    std::string                         m_name;

    bool                                m_readOnly;

public:
    
    explicit                                        AttributeChannel    ( const AttributeChannelDescriptor * desc, const std::string & name, bool readOnly = false );
                                                    ~AttributeChannel   ();

    void                                            SetReadOnly         ( bool readOnly );               
    virtual bool                                    IsReadOnly          ()  const;

    virtual const IAttributeChannelDescriptor *     GetDescriptor       ()  const;
    virtual std::string                             GetName             ()  const;


    static IAttributeChannelPtr                     GetPositionChannel( const std::vector< IAttributeChannelPtr > & channels );
    static IAttributeChannelPtr                     GetUVChannel( const std::vector< IAttributeChannelPtr >& channels, unsigned int index );
};

DEFINE_PTR_TYPE(AttributeChannel)
DEFINE_CONST_PTR_TYPE(AttributeChannel)

} // model
} // bv
