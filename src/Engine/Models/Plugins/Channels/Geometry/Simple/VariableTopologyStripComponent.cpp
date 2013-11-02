#include "VariableTopologyStripComponent.h"

#include "Mathematics/defines.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributeChannelTyped.h"

#include <cmath>
#include <cassert>

namespace bv { namespace model {

// *******************************
//
VariableTopologyStripComponent::VariableTopologyStripComponent                  ( float size, float speed, float duration, int numSegments, float oscilationSpeed, float mainScale, float startX, float startY, float posZ )
    : m_size( size )
    , m_speed( speed )
    , m_activeDuration( duration )
    , numSegments( numSegments )
    , m_oscilationSpeed( oscilationSpeed )
    , m_mainScale( mainScale )
    , m_startX( startX )
    , m_startY( startY )
    , m_posZ( posZ )
    , m_topologyChanged( true )
{
    assert( duration > 0.01f );
    assert( numSegments >= 1 );
    assert( speed > 0.01f );
    assert( oscilationSpeed > 0.01f );
    assert( mainScale > 0.001f );

    m_segmentDeltaTime = duration /  (float) numSegments;
    m_activeSegment = 0;

    //FIXME: not null desc should be created via factory
    const VertexAttributeChannelDescriptor * desc = new VertexAttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );
    Float3VertexAttributeChannel * vertArrtF3 = new Float3VertexAttributeChannel( desc, desc->SuggestedDefaultName( 0 ), false );

    //First part of first strip
    glm::vec3 f0 = EvaluateFunction( 0.f );
    glm::vec3 n0 = EvaluateNormal( EvaluateVelocity( 0.f ) );

    //Second part of first strip
    glm::vec3 f1 = EvaluateFunction( m_segmentDeltaTime );
    glm::vec3 n1 = EvaluateNormal( EvaluateVelocity( m_segmentDeltaTime ) );

    //Add one segment (just so that something is rendered)
    vertArrtF3->AddVertexAttribute( f0 + n0 * 0.5f );
    vertArrtF3->AddVertexAttribute( f0 - n0 * 0.5f );
    vertArrtF3->AddVertexAttribute( f1 - n1 * 0.5f );
    vertArrtF3->AddVertexAttribute( f1 + n1 * 0.5f );

    m_vertexAttributeChannels.push_back( vertArrtF3 );
    m_positions = vertArrtF3;
}

// *******************************
//
void                     VariableTopologyStripComponent::Update         ( float t )
{
    m_topologyChanged = false;

    if ( !IsActive( t ) )
    {
        return;
    }

    t = m_segmentDeltaTime * t * m_speed;
    int nSegment = (int) t;

    assert( nSegment - m_activeSegment <= 1 );

    if ( nSegment == m_activeSegment )
    {
        return;
    }

    //FIXME: variable number of segments should be allowed - this way we explicitely require fast updates (at most one segment per update)
    m_topologyChanged = true;


//    float sclSine   = m_sclSine;
//    float dSine     = fmod( t * m_speedX , (float) TWOPI );
////    float dSine     = fmod( t * m_speedX , (float) TWOPI / m_sclSine );
//
//    float sclCosine = m_sclCosine;
//    float dCosine   = fmod( t * m_speedY , (float) TWOPI );
////    float dCosine   = fmod( t * m_speedY , (float) TWOPI / m_sclCosine );
//
//    std::vector< glm::vec3 > & vx = m_positions->GetVertices();
//
//    float yStart    = -m_h * 0.5f;
//
//    for( unsigned int i = 0; i < vx.size(); i += 2 )
//    {
//        glm::vec3 & vt = vx[ i ];
//        glm::vec3 & vb = vx[ i + 1 ];
//
//        float t = vt.x * sclSine + dSine;
//        float s = vt.x * sclCosine + dCosine;
//
//        float dy = m_sizeY * sin( t );
//        float dz = m_sizeZ * cos( s );
//
//        vt.y = yStart + m_h + dy;
//        vb.y = yStart + dy;
//
//        vt.z = vb.z = m_z + dz;
//    }
}

// *******************************
//
VariableTopologyStripComponent::~VariableTopologyStripComponent                 ()
{
}

// *******************************
//
bool                                VariableTopologyStripComponent::IsActive    ( float t ) const
{
    return t * m_speed < m_activeDuration;
}

// *******************************
//
bool                VariableTopologyStripComponent::TopologyChanged             ( float t ) const
{
    return m_topologyChanged;
}

// *******************************
//
glm::vec3               VariableTopologyStripComponent::EvaluateFunction                ( float t ) const
{
    float val =  m_mainScale * sinf( t * m_oscilationSpeed );
    float x   = t * m_speed;

    return glm::vec3(x, val, 0.f) + glm::vec3(m_startX,m_startY, m_posZ);
;
}

// *******************************
//
glm::vec3           VariableTopologyStripComponent::EvaluateVelocity                ( float t ) const
{
    return glm::vec3( m_speed, m_oscilationSpeed * m_mainScale * cosf( t * m_oscilationSpeed ), 0.f );
}

// *******************************
//
glm::vec3           VariableTopologyStripComponent::EvaluateNormal                  ( const glm::vec3 & tangent ) const
{
    return glm::normalize( glm::vec3( -tangent.y, tangent.x, 0.f ) );
}

// *******************************
//
VariableTopologyStripComponent *  VariableTopologyStripComponent::Create        ( float size, float speed, float duration, int numSegments, float oscilationSpeed, float mainScale, float startX, float startY, float posZ )
{
    assert( numSegments >= 1 );

    return new VariableTopologyStripComponent( size, speed, duration, numSegments, oscilationSpeed, mainScale, startX, startY, posZ );
}

} //model
} //bv
