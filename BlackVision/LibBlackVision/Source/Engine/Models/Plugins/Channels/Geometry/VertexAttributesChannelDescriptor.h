#pragma once

#include <vector>

#include "Engine/Types/Enums.h"

#include "Engine/Models/Plugins/Interfaces/IVertexAttributesChannelDescriptor.h"


namespace bv { namespace model
{

class IAttributeChannelDescriptor;
class AttributeChannelDescriptor;
class IndexChannelDescriptor;

class VertexAttributesChannelDescriptor : public IVertexAttributesChannelDescriptor
{
private:

    std::vector< const AttributeChannelDescriptor * >   m_attributeChannelDescriptors;
    IndexChannelDescriptor *                            m_indexChannelDescriptor;

public:

                                                        VertexAttributesChannelDescriptor   ();
                                                        VertexAttributesChannelDescriptor	( const VertexAttributesChannelDescriptor & other );
    virtual										        ~VertexAttributesChannelDescriptor	();

    virtual unsigned int								GetNumVertexChannels		()						const;
    virtual bool								        HasIndexChannel				()						const;

    virtual const IAttributeChannelDescriptor *	        GetAttrChannelDescriptor	( int channelIndex )	const;
    virtual const IIndexChannelDescriptor *		        GetIndexChannelDescriptor	()						const;

    virtual unsigned int                                SingleVertexEntrySize       ()                      const;
    virtual unsigned int                                SingleIndexEntrySize        ()                      const;

    const AttributeChannelDescriptor *                  AddAttrChannelDesc      ( AttributeType attrType, AttributeSemantic attrSemantic, ChannelRole channelRole );
    void                                                AddAttrChannelDesc      ( const AttributeChannelDescriptor * desc );

    const IndexChannelDescriptor *                      SetIndexChannelDesc     ( IndexType type );
    void                                                SetIndexChannelDesc     ( IndexChannelDescriptor * desc );

    //@param[ occuranceIdx ] for occuranceIdx < 0 function will return the last attrChannel
    const AttributeChannelDescriptor *					GetAttrChannelDescriptor	( AttributeSemantic semantic, Int32 occuranceIdx = 0 ) const;
};

} //model
} //bv
