#include "stdafx.h"

#include "BoundingVolume.h"

#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"

#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannelUtils.h"

//#include <glm/gtc/matrix_transform.hpp>

namespace bv { namespace model {

namespace {

model::ConnectedComponentPtr BuildComponentFromBox( const mathematics::Box & box )
{
    auto comp = model::ConnectedComponent::Create();

    auto * compVertDesc = new model::AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );

    auto vertArrtF3 = std::make_shared< model::Float3AttributeChannel >( compVertDesc, "boundingBox", false );

    comp->AddAttributeChannel( vertArrtF3 );

    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymin, box.zmin ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymax, box.zmin ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymin, box.zmin ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymax, box.zmin ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymin, box.zmax ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymax, box.zmax ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymin, box.zmax ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymax, box.zmax ) );

    return comp;
}

//void AddBoxToVAC( VertexAttributesChannel * vac, mathematics::Box box )
//{
//    ConnectedComponentPtr comp = ConnectedComponent::Create();
//
//    auto desc = vac->GetDescriptor();
//
//    assert( desc->GetAttrChannelDescriptor( 0 )->GetSemantic() == AttributeSemantic::AS_POSITION );
//    assert( desc->GetAttrChannelDescriptor( 0 )->GetType() == AttributeType::AT_FLOAT3 );
//
//    auto compVertDesc = Cast< const AttributeChannelDescriptor* >( desc->GetAttrChannelDescriptor( 0 ) );
//
//    Float3AttributeChannelPtr vertArrtF3 = std::make_shared< Float3AttributeChannel >( compVertDesc, "boundingBox", false );
//
//    comp->AddAttributeChannel( vertArrtF3 );
//
//    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymin, box.zmin ) );
//    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymax, box.zmin ) );
//    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymin, box.zmin ) );
//    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymax, box.zmin ) );
//    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymin, box.zmax ) );
//    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymax, box.zmax ) );
//    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymin, box.zmax ) );
//    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymax, box.zmax ) );
//
//    for( UInt32 i = 1; i < desc->GetNumVertexChannels(); i++ ) // FIXME so much when new VAC model arrives
//    {
//        auto attrDesc = Cast< const AttributeChannelDescriptor* >( desc->GetAttrChannelDescriptor( i ) );
//        if( attrDesc->GetType() == AttributeType::AT_FLOAT3 )
//        {
//            auto attr = std::make_shared< Float3AttributeChannel >( attrDesc, "dummy", false );
//            for( int j = 0; j < 8; j++ ) attr->AddAttribute( glm::vec3( 0, 0, 0 ) );
//            comp->AddAttributeChannel( attr );
//        }
//        else if( attrDesc->GetType() == AttributeType::AT_FLOAT2 )
//        {
//            auto attr = std::make_shared< Float2AttributeChannel >( attrDesc, "dummy", false );
//            for( int j = 0; j < 8; j++ ) attr->AddAttribute( glm::vec2( 0, 0 ) );
//            comp->AddAttributeChannel( attr );
//        }
//        else
//            assert( false );
//    }
//
//    vac->AddConnectedComponent( comp );
//
//    box;
//}

//void UpdateBoxInVAC( VertexAttributesChannel * vac, mathematics::Box box )
//{
//    auto comp = vac->GetConnectedComponent( unsigned int( vac->GetComponents().size()-1 ) );
//
//    auto channel = Cast< Float3AttributeChannel * >( comp->GetAttrChannel( AttributeSemantic::AS_POSITION ).get() );
//    
//    auto verts = channel->GetVertices();
//    assert( verts.size() == 8 );
//
//    verts[ 0 ] = glm::vec3( box.xmin, box.ymin, box.zmin );
//    verts[ 1 ] = glm::vec3( box.xmin, box.ymax, box.zmin );
//    verts[ 2 ] = glm::vec3( box.xmax, box.ymin, box.zmin );
//    verts[ 3 ] = glm::vec3( box.xmax, box.ymax, box.zmin );
//    verts[ 4 ] = glm::vec3( box.xmax, box.ymin, box.zmax );
//    verts[ 5 ] = glm::vec3( box.xmax, box.ymax, box.zmax );
//    verts[ 6 ] = glm::vec3( box.xmin, box.ymin, box.zmax );
//    verts[ 7 ] = glm::vec3( box.xmin, box.ymax, box.zmax );
//}

} // anonymous



BoundingVolume::BoundingVolume          ( VertexAttributesChannel * vac )
    : m_vac( vac )
    , m_lastAttribuetesID( 0 )
    , m_lastTopologyID( 0 )
{
    m_box = CalculateBoundingBox( m_vac );
    //AddBoxToVAC( m_vac, m_box );
    //m_transform *= glm::translate( glm::mat4( 1.0f ), glm::vec3( box.xmin, box.ymin, box.zmin ) );
    //m_transform *= glm::scale( glm::mat4( 1.0f ), glm::vec3( box.xmax - box.xmin, box.ymax - box.ymin, box.zmax - box.zmin ) );
}

void                    BoundingVolume::Update                  ()
{
    if( m_lastAttribuetesID < m_vac->GetAttributesUpdateID() )
    {
        //UpdateBoxInVAC( m_vac, m_box );
    }
    else if( m_lastTopologyID < m_vac->GetTopologyUpdateID() )
    {
        m_box = CalculateBoundingBox( m_vac );
        //AddBoxToVAC( m_vac, m_box );
        m_lastTopologyID = m_vac->GetTopologyUpdateID();
        assert( m_lastAttribuetesID == m_vac->GetAttributesUpdateID() );
    }
}

// ***********************
//
const mathematics::Box *           BoundingVolume::GetBoundingBox          () const
{
    return &m_box;
}

// ***********************
//
IConnectedComponentPtr      BoundingVolume::BuildConnectedComponent () const
{
    return BuildComponentFromBox( m_box );
}


} }
