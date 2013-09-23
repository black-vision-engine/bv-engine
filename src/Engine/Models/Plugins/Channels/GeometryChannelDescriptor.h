#pragma once

#include <vector>

#include "Engine\Types\Enums.h"

#include "Engine/Models/Plugins/Interfaces/IGeometryChannelDescriptor.h"


namespace bv { namespace model
{

class VertexAttributeChannelDescriptor;
class IndexChannelDescriptor;

class GeometryChannelDescriptor : public IGeometryChannelDescriptor
{
private:

    std::vector< VertexAttributeChannelDescriptor * >   m_vertexAttributeChannelDescriptors;
    IndexChannelDescriptor *                            m_indexChannelDescriptor;

public:

                                                        GeometryChannelDescriptor   ();
    virtual										        ~GeometryChannelDescriptor	();

    virtual int									        GetNumVertexChannels		()						const;
	virtual bool								        HasIndexChannel				()						const;

	virtual const IVertexAttributeChannelDescriptor *	GetVertexChannelDescriptor	( int channelIndex )	const;
	virtual const IIndexChannelDescriptor *		        GetIndexChannelDescriptor	()						const;

    void                                                AddVertexAttrChannelDesc    ( AttributeType attrType, AttributeSemantic attrSemantic, ChannelRole channelRole );
    void                                                AddVertexAttrChannelDesc    ( VertexAttributeChannelDescriptor * desc );

    void                                                SetIndexAttrChannelDesc     ( IndexType type );
    void                                                SetIndexAttrChannelDesc     ( IndexChannelDescriptor * desc );

};

} //model
} //bv
