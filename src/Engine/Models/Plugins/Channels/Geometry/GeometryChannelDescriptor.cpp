#include "GeometryChannelDescriptor.h"

#include <cassert>

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/IndexChannelDescriptor.h"


namespace bv { namespace model
{

// ************************************
//
GeometryChannelDescriptor::GeometryChannelDescriptor    ()
    : m_indexChannelDescriptor( nullptr )
{
    
}

// ************************************
//
GeometryChannelDescriptor::~GeometryChannelDescriptor   ()
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
unsigned int GeometryChannelDescriptor::GetNumVertexChannels	() const
{
    return m_attributeChannelDescriptors.size();
}

// ************************************
//
bool	GeometryChannelDescriptor::HasIndexChannel		() const
{
    return m_indexChannelDescriptor != nullptr;
}

// ************************************
//
const IAttributeChannelDescriptor *	GeometryChannelDescriptor::GetAttrChannelDescriptor	( int channelIndex ) const
{
    assert( channelIndex >= 0 );
    assert( channelIndex < (int) m_attributeChannelDescriptors.size() );

    return m_attributeChannelDescriptors[ channelIndex ];
}

// ************************************
//
const IIndexChannelDescriptor *		        GeometryChannelDescriptor::GetIndexChannelDescriptor	()					 const
{
    return m_indexChannelDescriptor;
}

// ************************************
//
unsigned int                                GeometryChannelDescriptor::SingleVertexEntrySize       ()                    const
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
unsigned int                                GeometryChannelDescriptor::SingleIndexEntrySize        ()                    const
{
    if( HasIndexChannel() )
    {
        return m_indexChannelDescriptor->GetIndexSize();
    }

    return 0;
}

// ************************************
//
const AttributeChannelDescriptor *  GeometryChannelDescriptor::AddAttrChannelDesc    ( AttributeType attrType, AttributeSemantic attrSemantic, ChannelRole channelRole )
{
    AttributeChannelDescriptor * desc = new AttributeChannelDescriptor( attrType, attrSemantic, channelRole );
    AddAttrChannelDesc( desc );

    return desc;
}

// ************************************
//
void    GeometryChannelDescriptor::AddAttrChannelDesc    ( const AttributeChannelDescriptor * desc )
{
    m_attributeChannelDescriptors.push_back( desc );
}

// ************************************
//
const IndexChannelDescriptor *   GeometryChannelDescriptor::SetIndexChannelDesc     ( IndexType type )
{
    IndexChannelDescriptor * desc = new IndexChannelDescriptor( type );
    SetIndexChannelDesc( desc );

    return desc;
}

// ************************************
//
void    GeometryChannelDescriptor::SetIndexChannelDesc     ( IndexChannelDescriptor * desc )
{
    if ( m_indexChannelDescriptor != nullptr )
    {
        delete m_indexChannelDescriptor;
    }

    m_indexChannelDescriptor = desc;
}

} //model
}//bv
