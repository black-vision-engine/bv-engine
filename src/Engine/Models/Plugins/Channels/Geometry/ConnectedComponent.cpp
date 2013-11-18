#include "ConnectedComponent.h"

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"

#include "Engine/Models/Plugins/Interfaces/IAttributeChannel.h"


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
std::vector< IAttributeChannel* >     ConnectedComponent::GetAttributeChannels  () const
{
    return std::vector< IAttributeChannel* >( m_attributeChannels.begin(), m_attributeChannels.end());
}

// ************************************
//
unsigned int                                ConnectedComponent::GetNumVertices              () const
{
    if( m_attributeChannels.empty() )
    {
        return 0;
    }
    else
    {
        return m_attributeChannels[ 0 ]->GetNumEntries();
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
