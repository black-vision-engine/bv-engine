#include "GeometryChannelVariableTopology.h"

#include <cassert>
#include <cmath>

#include "Mathematics/defines.h"

#include "Engine/Models/Plugins/Channels/Geometry/GeometryChannelUtils.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/AnimatedStripComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributeChannelDescriptor.h"

#include "Engine/Models/Plugins/Interfaces/IVertexAttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/VariableTopologyStripComponent.h"

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
    
    float defaultDuration = 2.0f;
    float defaultOscilation = oscilationSpeed;
    float defaultScale = 1.f;
    float radius = 3.f;
    float dz = 0.1f;

    for( int i = 0; i < numComponents; ++i )
    {
        float alpha = (float) i * TWOPI_F / (float) numComponents;

        float duration = defaultDuration / (float) (i + 1);
        float oscilation = defaultOscilation / (float) (i + 1);
        float scale = defaultScale / (float) (i + 1);

        float x = radius * cosf( alpha );
        float y = radius * sinf( alpha );
        float z = dz * (float) i;

        VariableTopologyStripComponent * cc = VariableTopologyStripComponent::Create( size, speed, duration, numSegments, oscilation, scale, x, y, z );

        if ( i == 0 )
        {
            channel->SetDescriptor( DescriptorFromConnectedComponent( cc ) );
        }
    
        channel->AddConnectedComponent( cc );
    }

    return channel;
}


} //model
}  //bv
