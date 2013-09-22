#include "GeometryChannel.h"
#include "ConnectedComponent.h"

#include "Engine\Models\Plugins\Channels\VertexAttributeChannel.h"

#include <cassert>

namespace bv { namespace model
{

// *********************************
//
GeometryChannel::GeometryChannel     (const IPlugin* pl)
{

}

// *********************************
//
GeometryChannel::~GeometryChannel   ()
{}

// *********************************
//
void                                    GeometryChannel::Update              (float t)
{
    for( auto comp : m_components )
    {
        comp->Update( t );
    }
}

// *********************************
//
const std::vector< AttrType >&          GeometryChannel::GetType             () const
{
    return m_type;
}

// *********************************
//
const std::vector< AttrSemantic >&      GeometryChannel::GetSemantic         () const
{
    return m_semantic;
}

// *********************************
//
PrimitiveType                           GeometryChannel::GetPrimitiveType    () const
{
    return m_primitiveType;
}

// *********************************
//
int                                     GeometryChannel::GetNumPrimitives   ( IConnectedComponent* connComp ) const
{
    int verNum = connComp->GetNumVertices();

    switch(GetPrimitiveType())
    {
        case PrimitiveType::PT_POINTS:
            return verNum;
        case PrimitiveType::PT_LINES:
            assert(verNum % 2 == 0);
            return verNum / 2;
        case PrimitiveType::PT_LINE_STRIP:
            return verNum - 1;
        case PrimitiveType::PT_LINE_LOOP:
            return verNum;
        case PrimitiveType::PT_TRIANGLES:
            assert(verNum % 3 == 0);
            return verNum / 3;
        case PrimitiveType::PT_TRIANGLE_STRIP:
            return verNum - 2;
        case PrimitiveType::PT_TRIANGLE_FAN:
            return verNum - 2;
        case PrimitiveType::PT_QUADS:
            assert(verNum % 4 == 0);
            return verNum / 4;
        case PrimitiveType::PT_QUAD_STRIP:
            return verNum - 3;
        case PrimitiveType::PT_POLYGON:
            return 1;
        default:
            assert(false);
            return 0;
    }
}

// *********************************
//
std::vector< IConnectedComponent* >     GeometryChannel::GetComponents       () const
{
    return std::vector< IConnectedComponent* >( m_components.begin(), m_components.end() );
}

// *********************************
//
GeometryChannelStaticRect::GeometryChannelStaticRect(float w, float h)
    : GeometryChannel( nullptr ) // FIXME: Pass plugin
{
    ConnectedComponent* connComp = new ConnectedComponent();

    Float3VertexAttributeChannel* vertArrtF3 = new Float3VertexAttributeChannel(AttrSemantic::AS_POSITION);

    vertArrtF3->AddVertexAttribute(glm::vec3( -w / 2.f, h / 2.f , 0.f ) );
    vertArrtF3->AddVertexAttribute(glm::vec3( -w / 2.f, -h / 2.f , 0.f ) );
    vertArrtF3->AddVertexAttribute(glm::vec3( w / 2.f, -h / 2.f , 0.f ) );
    vertArrtF3->AddVertexAttribute(glm::vec3( w / 2.f, h / 2.f , 0.f ) );

    connComp->m_vertexAttributeChannels.push_back( vertArrtF3 );

    m_components.push_back(connComp);
}

// *********************************
//
GeometryChannelStaticRectTextured::GeometryChannelStaticRectTextured( const std::string& texturePath, float w, float h )
    : GeometryChannel( nullptr ) // FIXME: Pass plugin
{
    ConnectedComponent* connComp = new ConnectedComponent();

    Float3VertexAttributeChannel* vertArrtF3 = new Float3VertexAttributeChannel(AttrSemantic::AS_POSITION);

    vertArrtF3->AddVertexAttribute(glm::vec3( -w / 2.f, h / 2.f , 0.f ) );
    vertArrtF3->AddVertexAttribute(glm::vec3( -w / 2.f, -h / 2.f , 0.f ) );
    vertArrtF3->AddVertexAttribute(glm::vec3( w / 2.f, -h / 2.f , 0.f ) );
    vertArrtF3->AddVertexAttribute(glm::vec3( w / 2.f, h / 2.f , 0.f ) );

    connComp->m_vertexAttributeChannels.push_back( vertArrtF3 );

    Float2VertexAttributeChannel* vertArrtF2 = new Float2VertexAttributeChannel(AttrSemantic::AS_TEXCOORD);
    vertArrtF2->AddVertexAttribute(glm::vec2( 0.f, 0.f ) );
    vertArrtF2->AddVertexAttribute(glm::vec2( 0.f, 1.f ) );
    vertArrtF2->AddVertexAttribute(glm::vec2( 1.f, 1.f ) );
    vertArrtF2->AddVertexAttribute(glm::vec2( 1.f, 0.f ) );

    connComp->m_vertexAttributeChannels.push_back( vertArrtF2 );

    m_components.push_back(connComp);
}

// *********************************
//
GeometryChannelStaticRectTextured*    GeometryChannelStaticRectTextured::Create( const std::string& texturePath, float w, float h )
{
    return new GeometryChannelStaticRectTextured(texturePath, w, h);
}

} // model
} // bv