#include "GeometryChannelAnimatedVertices.h"

#include <cassert>
#include <cmath>

#include "Mathematics/defines.h"

#include "Engine/Models/Plugins/Channels/Geometry/GeometryChannelUtils.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/AnimatedStripComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributeChannelDescriptor.h"

#include "Engine/Models/Plugins/Interfaces/IVertexAttributeChannel.h"

namespace bv { namespace model {

// ******************************
//
GeometryChannelAnimatedVertices::GeometryChannelAnimatedVertices     (  float w, float h, float speedX, float speedY, float cyclesX, float cyclesY )
    : GeometryChannel( PrimitiveType::PT_TRIANGLE_STRIP )
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
GeometryChannelAnimatedVertices::~GeometryChannelAnimatedVertices    ()
{
}

// ******************************
//
void    GeometryChannelAnimatedVertices::Update                      ( float t )
{
    assert( m_connectedComponents.size() == 1 );

    m_connectedComponents[ 0 ]->Update( t );
}

// ******************************
//
bool    GeometryChannelAnimatedVertices::NeedsAttributesUpdate        ( float t ) const
{
    return true;
}

// ******************************
//
GeometryChannelAnimatedVertices *   GeometryChannelAnimatedVertices::Create  ( float w, float h, float z, unsigned int numSegments, float speedX, float speedY, float cyclesX, float cyclesY, float sizeY, float sizeZ )
{
    float sclSine   = (float) TWOPI * cyclesX / w;
    float sclCosine = (float) TWOPI * cyclesY / w;

    GeometryChannelAnimatedVertices * channel = new GeometryChannelAnimatedVertices( w, h, speedX, speedY, cyclesX, cyclesY );
    AnimatedStripComponent * cc = AnimatedStripComponent::Create( w, h, numSegments, z, sclSine, sclCosine, speedX, speedY, sizeY, sizeZ );

    ChannelFromConnectedComponents( channel, 1, cc );

    return channel;
}

} //model
} //bv
