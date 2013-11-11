#include "GeometryChannelDescriptor.h"

#include <cassert>

#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributeChannelDescriptor.h"
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
    return m_vertexAttributeChannelDescriptors.size();
}

// ************************************
//
bool	GeometryChannelDescriptor::HasIndexChannel		() const
{
    return m_indexChannelDescriptor != nullptr;
}

// ************************************
//
const IVertexAttributeChannelDescriptor *	GeometryChannelDescriptor::GetVertexChannelDescriptor	( int channelIndex ) const
{
    assert( channelIndex >= 0 );
    assert( channelIndex < (int) m_vertexAttributeChannelDescriptors.size() );

    return m_vertexAttributeChannelDescriptors[ channelIndex ];
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

    for( auto channelDesc : m_vertexAttributeChannelDescriptors )
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
const VertexAttributeChannelDescriptor *  GeometryChannelDescriptor::AddVertexAttrChannelDesc    ( AttributeType attrType, AttributeSemantic attrSemantic, ChannelRole channelRole )
{
    VertexAttributeChannelDescriptor * desc = new VertexAttributeChannelDescriptor( attrType, attrSemantic, channelRole );
    AddVertexAttrChannelDesc( desc );

    return desc;
}

// ************************************
//
void    GeometryChannelDescriptor::AddVertexAttrChannelDesc    ( const VertexAttributeChannelDescriptor * desc )
{
    m_vertexAttributeChannelDescriptors.push_back( desc );
}

// ************************************
//
const IndexChannelDescriptor *   GeometryChannelDescriptor::SetIndexAttrChannelDesc     ( IndexType type )
{
    IndexChannelDescriptor * desc = new IndexChannelDescriptor( type );
    SetIndexAttrChannelDesc( desc );

    return desc;
}

// ************************************
//
void    GeometryChannelDescriptor::SetIndexAttrChannelDesc     ( IndexChannelDescriptor * desc )
{
    if ( m_indexChannelDescriptor != nullptr )
    {
        delete m_indexChannelDescriptor;
    }

    m_indexChannelDescriptor = desc;
}

} //model
}//bv
