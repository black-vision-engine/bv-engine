#include "RectComponent.h"

#include <cassert>

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"


namespace bv { namespace model
{

// *********************************
//
RectComponent::RectComponent( float w, float h, float tx, float ty, float tz )
{
    //FIXME: not null desc should be created via factory
    const AttributeChannelDescriptor * desc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );

    Float3AttributeChannel * vertArrtF3 = new Float3AttributeChannel( desc, desc->SuggestedDefaultName( 0 ), false );

    vertArrtF3->AddAttribute( glm::vec3( -w / 2.f + tx, -h / 2.f + ty, tz ) );
    vertArrtF3->AddAttribute( glm::vec3( w / 2.f  + tx, -h / 2.f + ty, tz ) );
    vertArrtF3->AddAttribute( glm::vec3( -w / 2.f + tx, h / 2.f  + ty, tz ) );
    vertArrtF3->AddAttribute( glm::vec3( w / 2.f  + tx, h / 2.f + ty,  tz ) );

    AddAttributeChannel( AttributeChannelPtr( vertArrtF3 ) );

    m_v3Attrs = vertArrtF3;
}

// *********************************
//
void                    RectComponent::SetRectSize     ( float w, float h )
{
    auto data = m_v3Attrs->GetVertices();
    assert( data.size() == 4 );

    float tx = 0.f;
    float ty = 0.f;
    float tz = 0.f;

    data[ 0 ] = glm::vec3( -w / 2.f + tx, -h / 2.f + ty, tz );
    data[ 1 ] = glm::vec3( w / 2.f  + tx, -h / 2.f + ty, tz );
    data[ 2 ] = glm::vec3( -w / 2.f + tx, h / 2.f  + ty, tz );
    data[ 3 ] = glm::vec3( w / 2.f  + tx, h / 2.f + ty,  tz );
}

// *********************************
//
RectComponent *              RectComponent::Create      ( float w, float h, float tx, float ty, float tz )
{
    return new RectComponent( w, h, tx, ty, tz );
}

} // model
} // bv
