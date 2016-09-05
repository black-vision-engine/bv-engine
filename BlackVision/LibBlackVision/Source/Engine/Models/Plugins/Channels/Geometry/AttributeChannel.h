#pragma once

#include <vector>
#include "Engine/Models/Plugins/Interfaces/IAttributeChannel.h"


namespace bv { namespace model
{

class AttributeChannelDescriptor;
DEFINE_PTR_TYPE( AttributeChannelDescriptor );
DEFINE_CONST_PTR_TYPE( AttributeChannelDescriptor );

class AttributeChannel : public IAttributeChannel
{
private:

    AttributeChannelDescriptorConstPtr              m_desc;
    std::string                                     m_name;

    bool                                            m_readOnly;

public:
    
    explicit                                        AttributeChannel    ( const AttributeChannelDescriptor * desc, const std::string & name, bool readOnly = false );
                                                    ~AttributeChannel   ();

    void                                            SetReadOnly         ( bool readOnly );               
    virtual bool                                    IsReadOnly          ()  const;

    virtual IAttributeChannelDescriptorConstPtr     GetDescriptor       ()  const;
    virtual std::string                             GetName             ()  const;
};

DEFINE_PTR_TYPE(AttributeChannel)
DEFINE_CONST_PTR_TYPE(AttributeChannel)

} // model
} // bv
