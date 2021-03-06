#pragma once

#include "Engine/Models/Plugins/Interfaces/IChannel.h"


namespace bv { namespace model
{

class IAttributeChannelDescriptor;
DEFINE_CONST_PTR_TYPE( IAttributeChannelDescriptor );
class IIndexChannelDescriptor;

//FIXME: add interface to access groups of channels (position, mapping and so on)
class IVertexAttributesChannelDescriptor
{
public:

    virtual unsigned int						        GetNumVertexChannels		()						const = 0;
    virtual bool								        HasIndexChannel				()						const = 0;

    virtual IAttributeChannelDescriptorConstPtr	        GetAttrChannelDescriptor	( int channelIndex )	const = 0;
    virtual const IIndexChannelDescriptor *		        GetIndexChannelDescriptor	()						const = 0;

    virtual unsigned int                                SingleVertexEntrySize       ()                      const = 0;
    virtual unsigned int                                SingleIndexEntrySize        ()                      const = 0;

    virtual										        ~IVertexAttributesChannelDescriptor	() {}

};

} //model
} //bv
