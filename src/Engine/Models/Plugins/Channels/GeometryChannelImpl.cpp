#include "GeometryChannelImpl.h"
#include "ConnectedComponent.h"

#include <cassert>

#include "Engine\Models\Plugins\Channels\VertexAttributeChannelTyped.h"


namespace bv { namespace model
{

// *********************************
//
GeometryChannelStaticRect::GeometryChannelStaticRect(float w, float h)
    : GeometryChannel( PrimitiveType::PT_TRIANGLE_STRIP ) // FIXME: Pass plugin - chyba ty !!!!!!!!!!!!
{
    ConnectedComponent * cc = new ConnectedComponent();

    //FIXME: not null desc should be created via factory
    const VertexAttributeChannelDescriptor * desc = m_desc.AddVertexAttrChannelDesc( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );
    Float3VertexAttributeChannel * vertArrtF3 = new Float3VertexAttributeChannel( desc, desc->SuggestedDefaultName( 0 ), false );

    vertArrtF3->AddVertexAttribute(glm::vec3( -w / 2.f, h / 2.f , 0.f ) );
    vertArrtF3->AddVertexAttribute(glm::vec3( -w / 2.f, -h / 2.f , 0.f ) );
    vertArrtF3->AddVertexAttribute(glm::vec3( w / 2.f, -h / 2.f , 0.f ) );
    vertArrtF3->AddVertexAttribute(glm::vec3( w / 2.f, h / 2.f , 0.f ) );

    cc->m_vertexAttributeChannels.push_back( vertArrtF3 );

    m_connectedComponents.push_back( cc );
}

// *********************************
//
bool                        GeometryChannelStaticRect::CanBeConnectedTo            ( const GeometryChannelDescriptor & desc ) const
{
    return false; //cannot - it is a generator
}

// *********************************
//
GeometryChannelStaticRect *  GeometryChannelStaticRect::Create                      ( float w, float h )
{
    return new GeometryChannelStaticRect( w, h );
}


// *********************************
// FIXME: of course it must be connecte
GeometryChannelStaticRectTextured::GeometryChannelStaticRectTextured( const std::string & texturePath, float w, float h )
    : GeometryChannel( PrimitiveType::PT_TRIANGLE_STRIP ) // FIXME: Pass plugin - chyba ty !!!!!!!!!!!!
{
    ConnectedComponent * cc = new ConnectedComponent();

    //FIXME: not null desc
    const VertexAttributeChannelDescriptor * descPos = m_desc.AddVertexAttrChannelDesc( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );
    const VertexAttributeChannelDescriptor * descMap = m_desc.AddVertexAttrChannelDesc( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_GENERATOR );

    Float3VertexAttributeChannel* vertArrtF3 = new Float3VertexAttributeChannel( descPos, descPos->SuggestedDefaultName( 0 ), true );
    
    vertArrtF3->AddVertexAttribute(glm::vec3( -w / 2.f, h / 2.f , 0.f ) );
    vertArrtF3->AddVertexAttribute(glm::vec3( -w / 2.f, -h / 2.f , 0.f ) );
    vertArrtF3->AddVertexAttribute(glm::vec3( w / 2.f, -h / 2.f , 0.f ) );
    vertArrtF3->AddVertexAttribute(glm::vec3( w / 2.f, h / 2.f , 0.f ) );

    cc->m_vertexAttributeChannels.push_back( vertArrtF3 );

    Float2VertexAttributeChannel * vertArrtF2 = new Float2VertexAttributeChannel( descMap, descMap->SuggestedDefaultName( 0 ), true );
    vertArrtF2->AddVertexAttribute(glm::vec2( 0.f, 0.f ) );
    vertArrtF2->AddVertexAttribute(glm::vec2( 0.f, 1.f ) );
    vertArrtF2->AddVertexAttribute(glm::vec2( 1.f, 1.f ) );
    vertArrtF2->AddVertexAttribute(glm::vec2( 1.f, 0.f ) );

    cc->m_vertexAttributeChannels.push_back( vertArrtF2 );

    m_connectedComponents.push_back( cc );

}

// *********************************
//
bool                        GeometryChannelStaticRectTextured::CanBeConnectedTo ( const GeometryChannelDescriptor & desc ) const
{
    return false; //cannot - it is a generator
}

// *********************************
//
GeometryChannelStaticRectTextured *    GeometryChannelStaticRectTextured::Create( const std::string & texturePath, float w, float h )
{
    return new GeometryChannelStaticRectTextured( texturePath, w, h );
}

} // model
} // bv