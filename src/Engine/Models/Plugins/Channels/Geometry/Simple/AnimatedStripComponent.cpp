#include "AnimatedStripComponent.h"

#include <cassert>

#include "Mathematics/defines.h"

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"


namespace bv { namespace model {

// *******************************
//
AnimatedStripComponent::AnimatedStripComponent                  ( float w, float h, unsigned int numSegments, float z, float sclSine, float sclCosine, float speedX, float speedY, float sizeY, float sizeZ )
    : m_sclSine( sclSine )
    , m_sclCosine( sclCosine )
    , m_speedX( speedX )
    , m_speedY( speedY )
    , m_h( h )
    , m_z( z )
    , m_sizeY( sizeY )
    , m_sizeZ( sizeZ )
{
    //FIXME: not null desc should be created via factory
    const AttributeChannelDescriptor * desc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );
    Float3AttributeChannelPtr vertArrtF3 = std::make_shared< Float3AttributeChannel >( desc, desc->SuggestedDefaultName( 0 ), false );

    float xStart    = -w * 0.5f;
    float yStart    = -h * 0.5f;

    float dx        = w / (float) numSegments;
    float dy        = h;

    vertArrtF3->AddAttribute(glm::vec3( xStart, yStart + dy, z ) );
    vertArrtF3->AddAttribute(glm::vec3( xStart, yStart, z ) );
    vertArrtF3->AddAttribute(glm::vec3( xStart + dx, yStart + dy, z ) );
    vertArrtF3->AddAttribute(glm::vec3( xStart + dx, yStart, z ) );
    
    for ( unsigned int i = 1; i < numSegments; ++i )
    {
        xStart += dx;

        vertArrtF3->AddAttribute(glm::vec3( xStart + dx, yStart + dy, z ) );
        vertArrtF3->AddAttribute(glm::vec3( xStart + dx, yStart, z ) );        
    }

    AddAttributeChannel( AttributeChannelPtr( vertArrtF3 ) ); // FIXME: Need to be sure that can take ownership
    m_positions = vertArrtF3;
}

// *******************************
//
void                     AnimatedStripComponent::Update         ( TimeType t )
{
    float sclSine   = m_sclSine;
    float dSine     = fmod( float( t ) * m_speedX , (float) TWOPI );
//    float dSine     = fmod( t * m_speedX , (float) TWOPI / m_sclSine );

    float sclCosine = m_sclCosine;
    float dCosine   = fmod( float( t ) * m_speedY , (float) TWOPI );
//    float dCosine   = fmod( t * m_speedY , (float) TWOPI / m_sclCosine );

    std::vector< glm::vec3 > & vx = m_positions->GetVertices();

    float yStart    = -m_h * 0.5f;

    for( unsigned int i = 0; i < vx.size(); i += 2 )
    {
        glm::vec3 & vt = vx[ i ];
        glm::vec3 & vb = vx[ i + 1 ];

        float t = vt.x * sclSine + dSine;
        float s = vt.x * sclCosine + dCosine;

        float dy = m_sizeY * sin( t );
        float dz = m_sizeZ * cos( s );

        vt.y = yStart + m_h + dy;
        vb.y = yStart + dy;

        vt.z = vb.z = m_z + dz;
    }
}

// *******************************
//
AnimatedStripComponent::~AnimatedStripComponent                 ()
{
}

// *******************************
//
AnimatedStripComponent *  AnimatedStripComponent::Create        ( float w, float h, unsigned int numSegments, float z, float sclSine, float sclCosine, float speedX, float speedY, float sizeY, float sizeZ )
{
    assert( numSegments >= 1 );

    return new AnimatedStripComponent( w, h, numSegments, z, sclSine, sclCosine, speedX, speedY, sizeY, sizeZ );
}

} //model
} //bv
