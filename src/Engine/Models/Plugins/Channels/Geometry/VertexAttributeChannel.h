#pragma once

#include "Engine\Models\Plugins\Interfaces\IVertexAttributeChannel.h"

namespace bv { namespace model
{

class VertexAttributeChannelDescriptor;

class VertexAttributeChannel : public IVertexAttributeChannel
{
private:

    const VertexAttributeChannelDescriptor *    m_desc;
    std::string                                 m_name;

    bool                                        m_readOnly;

public:
    
    explicit                                            VertexAttributeChannel  ( const VertexAttributeChannelDescriptor * desc, const std::string & name, bool readOnly = false );
                                                        ~VertexAttributeChannel ();

    void                                                SetReadOnly             ( bool readOnly );               
    virtual bool                                        IsReadOnly              ()  const;

    virtual const IVertexAttributeChannelDescriptor *   GetDescriptor           ()  const;
    virtual std::string                                 GetName                 ()  const;

};

} // model
} // bv