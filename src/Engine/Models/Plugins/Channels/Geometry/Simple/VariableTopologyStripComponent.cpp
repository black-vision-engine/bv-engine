#include "VariableTopologyStripComponent.h"

#include "Mathematics/defines.h"

#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributeChannelTyped.h"

#include <cassert>

namespace bv { namespace model {

// *******************************
//
VariableTopologyStripComponent::VariableTopologyStripComponent                  ( float size, float speed, float duration, int numSegments, float startX, float startY, float posZ )
    : m_size( size )
    , m_speed( speed )
    , m_activeDuration( duration )
    , numSegments( numSegments )
    , m_startX( startX )
    , m_startY( startY )
    , m_posZ( posZ )
{
    assert( duration > 0.01f );
    assert( numSegments >= 1 );
    assert( speed > 0.01f );

    m_segmentDeltaTime = duration /  (float) numSegments;

    ////FIXME: not null desc should be created via factory
    //const VertexAttributeChannelDescriptor * desc = new VertexAttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );
    //Float3VertexAttributeChannel * vertArrtF3 = new Float3VertexAttributeChannel( desc, desc->SuggestedDefaultName( 0 ), false );

    //float xStart    = -w * 0.5f;
    //float yStart    = -h * 0.5f;

    //float dx        = w / (float) numSegments;
    //float dy        = h;

    //vertArrtF3->AddVertexAttribute(glm::vec3( xStart, yStart + dy, z ) );
    //vertArrtF3->AddVertexAttribute(glm::vec3( xStart, yStart, z ) );
    //vertArrtF3->AddVertexAttribute(glm::vec3( xStart + dx, yStart + dy, z ) );
    //vertArrtF3->AddVertexAttribute(glm::vec3( xStart + dx, yStart, z ) );
    //
    //for ( unsigned int i = 1; i < numSegments; ++i )
    //{
    //    xStart += dx;

    //    vertArrtF3->AddVertexAttribute(glm::vec3( xStart + dx, yStart + dy, z ) );
    //    vertArrtF3->AddVertexAttribute(glm::vec3( xStart + dx, yStart, z ) );        
    //}

    //m_vertexAttributeChannels.push_back( vertArrtF3 );
    //m_positions = vertArrtF3;
}

// *******************************
//
void                     VariableTopologyStripComponent::Update         ( float t )
{
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
VariableTopologyStripComponent *  VariableTopologyStripComponent::Create        ( float size, float speed, float duration, int numSegments, float startX, float startY, float posZ )
{
    assert( numSegments >= 1 );

    return new VariableTopologyStripComponent( size, speed, duration, numSegments, startX, startY, posZ );
}

} //model
} //bv
