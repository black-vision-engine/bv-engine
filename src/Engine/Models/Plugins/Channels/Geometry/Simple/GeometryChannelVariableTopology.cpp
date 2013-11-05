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
    assert( speed > 0.01f );

    m_speed = speed;
    m_size = size;
    m_numSegments = numSegments;
    m_numComponents = numComponents;
    m_curComponentStartTime = 0.f;
    m_curComponent = 0;
    m_numActiveComponents = 1;
    m_needMoreUpdates = true;
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
    if( !m_needMoreUpdates )
    {
        return;
    }

    float pauseTime = 1.f;
    float totalTime = 0.f;

    for( int i = 0; i < m_numActiveComponents; ++i )
    {
        totalTime += pauseTime + m_vtConnectedComponents[ i ]->ComponentDuration();
    }

    float totalActiveTime = totalTime - pauseTime;
    auto cc = m_vtConnectedComponents[ m_curComponent ];

    if ( totalActiveTime < m_curComponentStartTime + cc->ComponentDuration() ) //active component requires update
    {
        cc->Update( t );
    }
    else if( totalTime < m_curComponentStartTime + cc->ComponentDuration() ) //pause - do nothing
    {
        return;
    }
    else //and now to the next component
    {
        m_curComponent++;

        if ( m_curComponent >= m_numComponents )
        {
            m_needMoreUpdates = false;
        }
        else
        {
            m_curComponentStartTime = totalTime;
        }
    }
}

// ******************************
//
bool    GeometryChannelVariableTopology::NeedsPositionsUpdate        ( float t ) const
{
    return false;
}

// ******************************
//
bool    GeometryChannelVariableTopology::NeedsTopologyUpdate         ( float t ) const
{
    return m_vtConnectedComponents[ m_curComponent ]->TopologyChanged( t );
}

// ******************************
//
bool    GeometryChannelVariableTopology::IsTimeInvariant            () const
{
    return !m_needMoreUpdates;
}

// ******************************
//
bool    GeometryChannelVariableTopology::CanBeConnectedTo                    ( IGeometryChannel * channel ) const
{
    return false;
}

// ******************************
//
void            GeometryChannelVariableTopology::AddVTConnectedComponent             ( VariableTopologyStripComponent * cc )
{
    GeometryChannelVariableTopology::AddConnectedComponent( cc );

    m_vtConnectedComponents.push_back( cc );
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
    
        channel->AddVTConnectedComponent( cc );
    }

    return channel;
}


} //model
}  //bv
