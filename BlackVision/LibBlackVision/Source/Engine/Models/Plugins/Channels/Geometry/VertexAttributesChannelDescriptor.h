#pragma once

#include <vector>

#include "Engine/Types/Enums.h"

#include "Engine/Models/Plugins/Interfaces/IVertexAttributesChannelDescriptor.h"


namespace bv { namespace model
{

class IAttributeChannelDescriptor;
class AttributeChannelDescriptor;
DEFINE_CONST_PTR_TYPE( AttributeChannelDescriptor );
class IndexChannelDescriptor;

class VertexAttributesChannelDescriptor : public IVertexAttributesChannelDescriptor
{
private:

    std::vector< AttributeChannelDescriptorConstPtr >   m_attributeChannelDescriptors;
    IndexChannelDescriptor *                            m_indexChannelDescriptor;

public:

                                                        VertexAttributesChannelDescriptor   ();
                                                        VertexAttributesChannelDescriptor	( const VertexAttributesChannelDescriptor & other );
    virtual										        ~VertexAttributesChannelDescriptor	();

    virtual unsigned int								GetNumVertexChannels		        ()						const;
    virtual bool								        HasIndexChannel				        ()						const;

    virtual IAttributeChannelDescriptorConstPtr	        GetAttrChannelDescriptor	        ( int channelIndex )	const;
    virtual const IIndexChannelDescriptor *		        GetIndexChannelDescriptor	        ()						const;

    virtual unsigned int                                SingleVertexEntrySize               ()                      const;
    virtual unsigned int                                SingleIndexEntrySize                ()                      const;

    AttributeChannelDescriptorConstPtr                  AddAttrChannelDesc                  ( AttributeType attrType, AttributeSemantic attrSemantic, ChannelRole channelRole );
    void                                                AddAttrChannelDesc                  ( AttributeChannelDescriptorConstPtr desc );

    const IndexChannelDescriptor *                      SetIndexChannelDesc                 ( IndexType type );
    void                                                SetIndexChannelDesc                 ( IndexChannelDescriptor * desc );

    //@param[ occuranceIdx ] for occuranceIdx < 0 function will return the last attrChannel
    AttributeChannelDescriptorConstPtr					GetAttrChannelDescriptor	        ( AttributeSemantic semantic, Int32 occuranceIdx = 0 ) const;
};

} //model
} //bv
