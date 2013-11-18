#include "ConnectedComponent.h"

#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributeChannel.h"

#include "Engine/Models/Plugins/Interfaces/IVertexAttributeChannel.h"


namespace bv { namespace model
{

// ************************************
//
ConnectedComponent::ConnectedComponent()
{
}

// ************************************
//
ConnectedComponent::~ConnectedComponent()
{
}

// ************************************
//
void                                        ConnectedComponent::Update( TimeType t )
{
    //FIXME: Forward do channels
}

// ************************************
//
std::vector< IVertexAttributeChannel* >     ConnectedComponent::GetVertexAttributeChannels  () const
{
    return std::vector< IVertexAttributeChannel* >(m_vertexAttributeChannels.begin(), m_vertexAttributeChannels.end());
}

// ************************************
//
unsigned int                                ConnectedComponent::GetNumVertices              () const
{
    if( m_vertexAttributeChannels.empty() )
    {
        return 0;
    }
    else
    {
        return m_vertexAttributeChannels[ 0 ]->GetNumEntries();
    }
}

// ************************************
//
unsigned int                               ConnectedComponent::GetNumPrimitives            () const
{

    // TODO: co tutaj?
    //FIXME: GAWNO

    return 0;
}

} // model
} // bv
