#pragma once

#include <vector>

#include "Engine/Types/Enums.h"

#include "Engine/Models/Plugins/Interfaces/IGeometryChannelDescriptor.h"


namespace bv { namespace model
{

class VertexAttributeChannelDescriptor;
class IndexChannelDescriptor;

class GeometryChannelDescriptor : public IGeometryChannelDescriptor
{
private:

    std::vector< const VertexAttributeChannelDescriptor * >     m_vertexAttributeChannelDescriptors;
    IndexChannelDescriptor *                                    m_indexChannelDescriptor;

public:

                                                        GeometryChannelDescriptor   ();
    virtual										        ~GeometryChannelDescriptor	();

    virtual unsigned int								GetNumVertexChannels		()						const;
	virtual bool								        HasIndexChannel				()						const;

	virtual const IVertexAttributeChannelDescriptor *	GetVertexChannelDescriptor	( int channelIndex )	const;
	virtual const IIndexChannelDescriptor *		        GetIndexChannelDescriptor	()						const;

    virtual unsigned int                                SingleVertexEntrySize       ()                      const;
    virtual unsigned int                                SingleIndexEntrySize        ()                      const;

    const VertexAttributeChannelDescriptor *            AddVertexAttrChannelDesc    ( AttributeType attrType, AttributeSemantic attrSemantic, ChannelRole channelRole );
    void                                                AddVertexAttrChannelDesc    ( const VertexAttributeChannelDescriptor * desc );

    const IndexChannelDescriptor *                      SetIndexAttrChannelDesc     ( IndexType type );
    void                                                SetIndexAttrChannelDesc     ( IndexChannelDescriptor * desc );

};

} //model
} //bv
