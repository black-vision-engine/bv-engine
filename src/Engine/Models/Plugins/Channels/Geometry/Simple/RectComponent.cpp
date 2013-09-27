#include "RectComponent.h"

#include <cassert>

#include "Engine\Models\Plugins\Channels\Geometry\VertexAttributeChannelTyped.h"


namespace bv { namespace model
{

// *********************************
//
RectComponent::RectComponent(float w, float h)
{
    //FIXME: not null desc should be created via factory
    const VertexAttributeChannelDescriptor * desc = new VertexAttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );

    Float3VertexAttributeChannel * vertArrtF3 = new Float3VertexAttributeChannel( desc, desc->SuggestedDefaultName( 0 ), false );

    vertArrtF3->AddVertexAttribute(glm::vec3( -w / 2.f, -h / 2.f , 0.f ) );
    vertArrtF3->AddVertexAttribute(glm::vec3( w / 2.f, -h / 2.f , 0.f ) );
    vertArrtF3->AddVertexAttribute(glm::vec3( -w / 2.f, h / 2.f , 0.f ) );
    vertArrtF3->AddVertexAttribute(glm::vec3( w / 2.f, h / 2.f , 0.f ) );

    m_vertexAttributeChannels.push_back( vertArrtF3 );
}

RectComponent *              RectComponent::Create                      ( float w, float h )
{
    return new RectComponent( w, h );
}

} // model
} // bv