#include "ConnectedComponent.h"

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"

#include "Engine/Models/Plugins/Interfaces/IAttributeChannel.h"


namespace bv { namespace model
{

// ************************************
//
ConnectedComponentPtr ConnectedComponent::Create()
{
    return ConnectedComponentPtr( new ConnectedComponent() );
}

// ************************************
//
ConnectedComponent::ConnectedComponent()
{
}

// ************************************
//
ConnectedComponent::~ConnectedComponent()
{}

// ************************************
//
void                                        ConnectedComponent::Update( TimeType t )
{
    { t; } // FIXME: suppress unused variable
    //FIXME: Forward do channels
}

// ************************************
//
std::vector< IAttributeChannelPtr >         ConnectedComponent::GetAttributeChannels  () const
{
    std::vector< IAttributeChannelPtr > ret;
    ret.reserve( m_attributeChannels.size() );

    for( auto att : m_attributeChannels )
        ret.push_back( att );

    return ret;
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

// ************************************
//
const std::vector< AttributeChannelPtr > &  ConnectedComponent::GetAttributeChannelsPtr     () const
{
    return m_attributeChannels;
}

// ************************************
//
void                                        ConnectedComponent::AddAttributeChannel         ( const AttributeChannelPtr& attr )
{
    m_attributeChannels.push_back( attr );
}


} // model
} // bv
