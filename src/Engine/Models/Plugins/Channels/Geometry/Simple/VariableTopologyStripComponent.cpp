#include "VariableTopologyStripComponent.h"

#include <cmath>
#include <cassert>

#include "Mathematics/defines.h"

#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributeChannelTyped.h"


namespace bv { namespace model {

// *******************************
//
VariableTopologyStripComponent::VariableTopologyStripComponent                  ( float size, float speed, TimeType duration, int numSegments, float oscilationSpeed, float mainScale, float startX, float startY, float posZ )
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
    vertArrtF3->AddVertexAttribute( TopPosition( f0, n0 ) );
    vertArrtF3->AddVertexAttribute( BottomPosition( f0, n0 ) );
    vertArrtF3->AddVertexAttribute( TopPosition( f1, n1 ) );
    vertArrtF3->AddVertexAttribute( BottomPosition( f1, n1 ) );

    m_vertexAttributeChannels.push_back( vertArrtF3 );
    m_positions = vertArrtF3;
}

// *******************************
//
void                     VariableTopologyStripComponent::Update         ( TimeType t )
{
    if ( !IsActive( t ) )
    {
        return;
    }

    int nSegment = (int) ( t * m_speed / m_segmentDeltaTime );

    assert( nSegment - m_activeSegment <= 1 );

    if ( nSegment == m_activeSegment )
    {
        return;
    }

    m_activeSegment = nSegment;

    //FIXME: variable number of segments should be allowed - this way we explicitely require fast updates (at most one segment per update)
    m_topologyChanged = true;

    //Second part of first strip
    t = m_segmentDeltaTime * ( nSegment + 1 );
    glm::vec3 f = EvaluateFunction( t );
    glm::vec3 n = EvaluateNormal( EvaluateVelocity( t ) );

    m_positions->AddVertexAttribute( TopPosition( f, n ) );
    m_positions->AddVertexAttribute( BottomPosition( f, n ) );
}

// *******************************
//
TimeType               VariableTopologyStripComponent::ComponentDuration           () const
{
    return m_activeDuration / TimeType( m_speed );
}

// *******************************
//
VariableTopologyStripComponent::~VariableTopologyStripComponent                 ()
{
}

// *******************************
//
bool                                VariableTopologyStripComponent::IsActive    ( TimeType t ) const
{
    return t * m_speed < m_activeDuration;
}

// *******************************
//
bool                VariableTopologyStripComponent::TopologyChanged             ( TimeType t ) const
{
    bool retVal = m_topologyChanged;
    m_topologyChanged = false;

    return retVal;
}

// *******************************
//
glm::vec3               VariableTopologyStripComponent::EvaluateFunction        ( TimeType t ) const
{
    float val =  m_mainScale * sinf( float( t ) * m_oscilationSpeed );
    float x   = float( t ) * m_speed;

    return glm::vec3( x, val, 0.f ) + glm::vec3( m_startX,m_startY, m_posZ );
}

// *******************************
//
glm::vec3           VariableTopologyStripComponent::EvaluateVelocity            ( TimeType t ) const
{
    return glm::vec3( m_speed, m_oscilationSpeed * m_mainScale * cosf( float( t ) * m_oscilationSpeed ), 0.f );
}

// *******************************
//
glm::vec3           VariableTopologyStripComponent::EvaluateNormal                  ( const glm::vec3 & tangent ) const
{
    return glm::normalize( glm::vec3( -tangent.y, tangent.x, 0.f ) );
}

// *******************************
//
glm::vec3           VariableTopologyStripComponent::TopPosition                     ( const glm::vec3 & val, const glm::vec3 & normal ) const
{
    return val + normal * 0.5f;
}

// *******************************
//
glm::vec3           VariableTopologyStripComponent::BottomPosition                  ( const glm::vec3 & val, const glm::vec3 & normal ) const
{
    return val - normal * 0.5f;
}

// *******************************
//
VariableTopologyStripComponent *  VariableTopologyStripComponent::Create        ( float size, float speed, TimeType duration, int numSegments, float oscilationSpeed, float mainScale, float startX, float startY, float posZ )
{
    assert( numSegments >= 1 );

    return new VariableTopologyStripComponent( size, speed, duration, numSegments, oscilationSpeed, mainScale, startX, startY, posZ );
}

} //model
} //bv
