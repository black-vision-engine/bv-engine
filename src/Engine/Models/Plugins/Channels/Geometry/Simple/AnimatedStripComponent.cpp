#include "AnimatedStripComponent.h"

#include "Engine\Models\Plugins\Channels\Geometry\VertexAttributeChannelTyped.h"

#include <cassert>

namespace bv { namespace model {

AnimatedStripComponent::AnimatedStripComponent                  ( float w, float h, unsigned int numSegments, float z )
{
    //FIXME: not null desc should be created via factory
    const VertexAttributeChannelDescriptor * desc = new VertexAttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );
    Float3VertexAttributeChannel * vertArrtF3 = new Float3VertexAttributeChannel( desc, desc->SuggestedDefaultName( 0 ), false );

    float xStart    = -w * 0.5f;
    float yStart    = -h * 0.5f;

    float dx        = w / (float) numSegments;
    float dy        = h;

    vertArrtF3->AddVertexAttribute(glm::vec3( xStart, yStart + dy, z ) );
    vertArrtF3->AddVertexAttribute(glm::vec3( xStart, yStart, z ) );
    vertArrtF3->AddVertexAttribute(glm::vec3( xStart + dx, yStart + dy, z ) );
    vertArrtF3->AddVertexAttribute(glm::vec3( xStart + dx, yStart, z ) );
    
    for ( unsigned int i = 1; i < numSegments; ++i )
    {
        xStart += dx;

        vertArrtF3->AddVertexAttribute(glm::vec3( xStart + dx, yStart + dy, z ) );
        vertArrtF3->AddVertexAttribute(glm::vec3( xStart + dx, yStart, z ) );        
    }

    m_vertexAttributeChannels.push_back( vertArrtF3 );

}

AnimatedStripComponent::~AnimatedStripComponent                 ()
{
}

AnimatedStripComponent *  AnimatedStripComponent::Create        ( float w, float h, unsigned int numSegments, float z )
{
    assert( numSegments >= 1 );

    return new AnimatedStripComponent( w, h, numSegments, z );
}

} //model
} //bv
