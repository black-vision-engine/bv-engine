#pragma once

#include "IChannel.h"

namespace bv { namespace model
{

class IVertexAttributeChannelDescriptor;
class IIndexChannelDescriptor;

//FIXME: add interface to access groups of channels (position, mapping and so on)
class IGeometryChannelDescriptor
{
public:

    virtual unsigned int						        GetNumVertexChannels		()						const = 0;
	virtual bool								        HasIndexChannel				()						const = 0;

	virtual const IVertexAttributeChannelDescriptor *	GetVertexChannelDescriptor	( int channelIndex )	const = 0;
	virtual const IIndexChannelDescriptor *		        GetIndexChannelDescriptor	()						const = 0;

    virtual unsigned int                                SingleVertexEntrySize       ()                      const = 0;
    virtual unsigned int                                SingleIndexEntrySize        ()                      const = 0;

    virtual										        ~IGeometryChannelDescriptor	() {}

};

} //model
} //bv
