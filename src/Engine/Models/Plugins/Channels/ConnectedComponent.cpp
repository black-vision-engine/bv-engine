#include "ConnectedComponent.h"

#include "Engine\Models\Plugins\Interfaces\IVertexAttributeChannel.h"

namespace bv { namespace model
{

ConnectedComponent::ConnectedComponent()
{
}


std::vector< IVertexAttributeChannel* >&    ConnectedComponent::GetVertexAttributeChannels  () const
{
    return m_VertexAttributeChannels;
}

int                                         ConnectedComponent::GetNumVertices              () const
{
    if( m_VertexAttributeChannels.empty() )
    {
        return 0;
    }
    else
    {
        return m_VertexAttributeChannels[0]->GetNumEntries();
    }
}

int                                         ConnectedComponent::GetNumPrimitives            () const
{
    // TODO: co tutaj?

    return 0;
}

} // model
} // bv