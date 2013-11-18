#include "VertexAttributesChannelDescriptor.h"

#include <cassert>

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/IndexChannelDescriptor.h"


namespace bv { namespace model
{

// ************************************
//
VertexAttributesChannelDescriptor::VertexAttributesChannelDescriptor    ()
    : m_indexChannelDescriptor( nullptr )
{
    
}

// ************************************
//
VertexAttributesChannelDescriptor::~VertexAttributesChannelDescriptor   ()
{
    //FIXME make sure that this is the owner of all descriptors
    //delete m_indexChannelDescriptor;

    //for( auto desc : m_vertexAttributeChannelDescriptors )
    //{
    //    delete desc;
    //}
}

// ************************************
//
unsigned int VertexAttributesChannelDescriptor::GetNumVertexChannels	() const
{
    return m_attributeChannelDescriptors.size();
}

// ************************************
//
bool	VertexAttributesChannelDescriptor::HasIndexChannel		() const
{
    return m_indexChannelDescriptor != nullptr;
}

// ************************************
//
const IAttributeChannelDescriptor *	VertexAttributesChannelDescriptor::GetAttrChannelDescriptor	( int channelIndex ) const
{
    assert( channelIndex >= 0 );
    assert( channelIndex < (int) m_attributeChannelDescriptors.size() );

    return m_attributeChannelDescriptors[ channelIndex ];
}

// ************************************
//
const IIndexChannelDescriptor *		        VertexAttributesChannelDescriptor::GetIndexChannelDescriptor	()					 const
{
    return m_indexChannelDescriptor;
}

// ************************************
//
unsigned int                                VertexAttributesChannelDescriptor::SingleVertexEntrySize       ()                    const
{
    unsigned int totalSize = 0;

    for( auto channelDesc : m_attributeChannelDescriptors )
    {
        totalSize += channelDesc->GetEntrySize();
    }

    return totalSize;
}

// ************************************
//
unsigned int                                VertexAttributesChannelDescriptor::SingleIndexEntrySize        ()                    const
{
    if( HasIndexChannel() )
    {
        return m_indexChannelDescriptor->GetIndexSize();
    }

    return 0;
}

// ************************************
//
const AttributeChannelDescriptor *  VertexAttributesChannelDescriptor::AddAttrChannelDesc    ( AttributeType attrType, AttributeSemantic attrSemantic, ChannelRole channelRole )
{
    AttributeChannelDescriptor * desc = new AttributeChannelDescriptor( attrType, attrSemantic, channelRole );
    AddAttrChannelDesc( desc );

    return desc;
}

// ************************************
//
void    VertexAttributesChannelDescriptor::AddAttrChannelDesc    ( const AttributeChannelDescriptor * desc )
{
    m_attributeChannelDescriptors.push_back( desc );
}

// ************************************
//
const IndexChannelDescriptor *   VertexAttributesChannelDescriptor::SetIndexChannelDesc     ( IndexType type )
{
    IndexChannelDescriptor * desc = new IndexChannelDescriptor( type );
    SetIndexChannelDesc( desc );

    return desc;
}

// ************************************
//
void    VertexAttributesChannelDescriptor::SetIndexChannelDesc     ( IndexChannelDescriptor * desc )
{
    if ( m_indexChannelDescriptor != nullptr )
    {
        delete m_indexChannelDescriptor;
    }

    m_indexChannelDescriptor = desc;
}

} //model
}//bv
