#include "GeometryChannelDescriptor.h"

#include <cassert>

#include "Engine\Models\Plugins\Channels\VertexAttributeChannelDescriptor.h"
#include "Engine\Models\Plugins\Channels\IndexChannelDescriptor.h"

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
    delete m_indexChannelDescriptor;

    for( auto desc : m_vertexAttributeChannelDescriptors )
    {
        delete desc;
    }
}

// ************************************
//
int		GeometryChannelDescriptor::GetNumVertexChannels	() const
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
void    GeometryChannelDescriptor::AddVertexAttrChannelDesc    ( AttributeType attrType, AttributeSemantic attrSemantic, ChannelRole channelRole )
{
    return AddVertexAttrChannelDesc( new VertexAttributeChannelDescriptor( attrType, attrSemantic, channelRole ) );
}

// ************************************
//
void    GeometryChannelDescriptor::AddVertexAttrChannelDesc    ( VertexAttributeChannelDescriptor * desc )
{
    m_vertexAttributeChannelDescriptors.push_back( desc );
}

// ************************************
//
void    GeometryChannelDescriptor::SetIndexAttrChannelDesc     ( IndexType type )
{
    SetIndexAttrChannelDesc( new IndexChannelDescriptor( type ) );
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
