#include "GeometryChannelVariableTopology.h"

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
GeometryChannelVariableTopology::GeometryChannelVariableTopology     (  float size, float speed, float oscilationSpeed, int numSegments, int numComponents )
    : GeometryChannel( PrimitiveType::PT_TRIANGLE_STRIP )
{
    assert( numSegments >= 1 );
    assert( numComponents >= 1 );
    assert( m_speed > 0.01f );
    m_speed = speed;
    m_size = size;
    m_numSegments = numSegments;
    m_numComponents = numComponents;
}

// ******************************
//
GeometryChannelVariableTopology::~GeometryChannelVariableTopology    ()
{
}

// ******************************
//
void    GeometryChannelVariableTopology::Update                      ( float t )
{
    assert( m_connectedComponents.size() == 1 );

    m_connectedComponents[ 0 ]->Update( t );
}

// ******************************
//
bool    GeometryChannelVariableTopology::NeedsPositionsUpdate        ( float t ) const
{
    return true;
}

// ******************************
//
GeometryChannelVariableTopology *   GeometryChannelVariableTopology::Create  ( float size, float speed, float oscilationSpeed, int numSegments, int numComponents )
{
    GeometryChannelVariableTopology * channel = new GeometryChannelVariableTopology( size, speed, oscilationSpeed, numSegments, numComponents );
    return channel;
}


} //model
}  //bv
