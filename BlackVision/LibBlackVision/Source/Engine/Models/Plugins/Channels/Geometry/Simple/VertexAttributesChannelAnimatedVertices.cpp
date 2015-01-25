#include "VertexAttributesChannelAnimatedVertices.h"

#include <cassert>
#include <cmath>

#include "Mathematics/defines.h"

#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannelUtils.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/AnimatedStripComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"

#include "Engine/Models/Plugins/Interfaces/IAttributeChannel.h"


namespace bv { namespace model {

// ******************************
//
VertexAttributesChannelAnimatedVertices::VertexAttributesChannelAnimatedVertices     (  float w, float h, float speedX, float speedY, float cyclesX, float cyclesY )
    : VertexAttributesChannel( PrimitiveType::PT_TRIANGLE_STRIP )
{
    m_w         = w;
    m_h         = h;
    m_speedX    = speedX;
    m_speedY    = speedY;
    m_cyclesX   = cyclesX;
    m_cyclesY   = cyclesY;
}

// ******************************
//
VertexAttributesChannelAnimatedVertices::~VertexAttributesChannelAnimatedVertices    ()
{
}

// ******************************
//
void    VertexAttributesChannelAnimatedVertices::Update                      ( TimeType t )
{
    assert( m_connectedComponents.size() == 1 );

    m_connectedComponents[ 0 ]->Update( t );
}

// ******************************
//
bool    VertexAttributesChannelAnimatedVertices::NeedsAttributesUpdate      () const
{
    return true;
}

// ******************************
//
VertexAttributesChannelAnimatedVertices *   VertexAttributesChannelAnimatedVertices::Create  ( float w, float h, float z, unsigned int numSegments, float speedX, float speedY, float cyclesX, float cyclesY, float sizeY, float sizeZ )
{
    float sclSine   = (float) TWOPI * cyclesX / w;
    float sclCosine = (float) TWOPI * cyclesY / w;

    VertexAttributesChannelAnimatedVertices * channel = new VertexAttributesChannelAnimatedVertices( w, h, speedX, speedY, cyclesX, cyclesY );
    AnimatedStripComponentPtr cc = AnimatedStripComponent::Create( w, h, numSegments, z, sclSine, sclCosine, speedX, speedY, sizeY, sizeZ );

    ChannelFromConnectedComponents( channel, 1, cc );

    return channel;
}

} //model
} //bv
