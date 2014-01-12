#include "RectComponent.h"

#include <cassert>

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"


namespace bv { namespace model
{

// *********************************
//
RectComponent::RectComponent(float w, float h, float tx, float ty, float tz )
{
    //FIXME: not null desc should be created via factory
    const AttributeChannelDescriptor * desc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );

    Float3AttributeChannel * vertArrtF3 = new Float3AttributeChannel( desc, desc->SuggestedDefaultName( 0 ), false );

    vertArrtF3->AddAttribute(glm::vec3( -w / 2.f + tx, -h / 2.f + ty, tz ) );
    vertArrtF3->AddAttribute(glm::vec3( w / 2.f  + tx, -h / 2.f + ty, tz ) );
    vertArrtF3->AddAttribute(glm::vec3( -w / 2.f + tx, h / 2.f  + ty, tz ) );
    vertArrtF3->AddAttribute(glm::vec3( w / 2.f  + tx, h / 2.f + ty,  tz ) );

    AddAttributeChannel( AttributeChannelPtr( vertArrtF3 ) );
}

RectComponent *              RectComponent::Create                      ( float w, float h, float tx, float ty, float tz )
{
    return new RectComponent( w, h, tx, ty, tz );
}

} // model
} // bv
