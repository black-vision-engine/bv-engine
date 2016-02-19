#include "stdafx.h"

#include "RingComponent.h"

#include <cassert>

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"


namespace bv { namespace model
{

// *********************************
//
RingComponent::RingComponent( float startAngle, float endAngle, float innerRadius, float outerRadius, int fragmentsNum )
{
    //FIXME: not null desc should be created via factory
    const AttributeChannelDescriptor * desc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );

    Float3AttributeChannelPtr vertArrtF3 = std::make_shared< Float3AttributeChannel >( desc, desc->SuggestedDefaultName( 0 ), false );

	glm::vec2 xyInner = glm::vec2( innerRadius * cos(startAngle), innerRadius * sin(startAngle) );
	glm::vec2 xyOuter = glm::vec2( outerRadius * cos(startAngle), outerRadius * sin(startAngle) );


    vertArrtF3->AddAttribute(glm::vec3(xyOuter.x, xyOuter.y, 0.f) );
    vertArrtF3->AddAttribute(glm::vec3(xyInner.x, xyInner.y, 0.f) );

	float angleDev = endAngle - startAngle;

	float step = angleDev / float(fragmentsNum);

	int i = 0;

	for(float ang = startAngle + step; i < fragmentsNum; ++i, ang += step)
	{
		glm::vec2 xyInner = glm::vec2( innerRadius * cos(ang), innerRadius * sin(ang) );
		glm::vec2 xyOuter = glm::vec2( outerRadius * cos(ang), outerRadius * sin(ang) );

        vertArrtF3->AddAttribute(glm::vec3(xyOuter.x, xyOuter.y, 0.f) );
        vertArrtF3->AddAttribute(glm::vec3(xyInner.x, xyInner.y, 0.f) );
	} 
    
    AddAttributeChannel( AttributeChannelPtr( vertArrtF3 ) );
}

RingComponentPtr             RingComponent::Create                      ( float startAngle, float endAngle, float innerRadius, float outerRadius, int fragmentsNum )
{
    return RingComponentPtr( new RingComponent( startAngle, endAngle, innerRadius, outerRadius, fragmentsNum ) );
}

} // model
} // bv
