#include "stdafx.h"

#include "BoundingVolume.h"

#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"

#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannelUtils.h"

//#include <glm/gtc/matrix_transform.hpp>

namespace bv { namespace model {

namespace {

mathematics::Box    GetBoundingBox( IVertexAttributesChannelConstPtr vac_ )
{
    auto vac = Cast< const VertexAttributesChannel* >( vac_.get() );

    return vac->GetBoundingBox();
}

void AddBoxToVAC( const IVertexAttributesChannel* vac_, mathematics::Box box )
{
    auto vac = RemoveConst( Cast< const VertexAttributesChannel* >( vac_ ) );

    ConnectedComponentPtr comp = ConnectedComponent::Create();

    auto desc = vac->GetDescriptor();

    assert( desc->GetAttrChannelDescriptor( 0 )->GetSemantic() == AttributeSemantic::AS_POSITION );
    assert( desc->GetAttrChannelDescriptor( 0 )->GetType() == AttributeType::AT_FLOAT3 );

    auto compVertDesc = Cast< const AttributeChannelDescriptor* >( desc->GetAttrChannelDescriptor( 0 ) );

    Float3AttributeChannelPtr vertArrtF3 = std::make_shared< Float3AttributeChannel >( compVertDesc, "boundingBox", false );

    comp->AddAttributeChannel( vertArrtF3 );

    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymin, box.zmin ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymax, box.zmin ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymin, box.zmin ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymax, box.zmin ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymin, box.zmax ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymax, box.zmax ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymin, box.zmax ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymax, box.zmax ) );

    for( UInt32 i = 1; i < desc->GetNumVertexChannels(); i++ ) // FIXME so much when new VAC model arrives
    {
        auto attrDesc = Cast< const AttributeChannelDescriptor* >( desc->GetAttrChannelDescriptor( i ) );
        if( attrDesc->GetType() == AttributeType::AT_FLOAT3 )
        {
            auto attr = std::make_shared< Float3AttributeChannel >( attrDesc, "dummy", false );
            for( int j = 0; j < 8; j++ ) attr->AddAttribute( glm::vec3( 0, 0, 0 ) );
            comp->AddAttributeChannel( attr );
        }
        else if( attrDesc->GetType() == AttributeType::AT_FLOAT2 )
        {
            auto attr = std::make_shared< Float2AttributeChannel >( attrDesc, "dummy", false );
            for( int j = 0; j < 8; j++ ) attr->AddAttribute( glm::vec2( 0, 0 ) );
            comp->AddAttributeChannel( attr );
        }
        else
            assert( false );
    }

    vac->AddConnectedComponent( comp );

    box;
}

} // anonymous



BoundingVolume::BoundingVolume          ( VertexAttributesChannel * vac )
    : m_vac( vac )
    , m_lastAttribuetesID( 0 )
    , m_lastTopologyID( 0 )
{
    m_box = m_vac->GetBoundingBox();
    AddBoxToVAC( m_vac, m_box );
    //m_transform *= glm::translate( glm::mat4( 1.0f ), glm::vec3( box.xmin, box.ymin, box.zmin ) );
    //m_transform *= glm::scale( glm::mat4( 1.0f ), glm::vec3( box.xmax - box.xmin, box.ymax - box.ymin, box.zmax - box.zmin ) );
}

void                    BoundingVolume::Update                  ()
{
    if( m_lastAttribuetesID < m_vac->GetAttributesUpdateID() )
    {
        assert( false );
    }
    else if( m_lastTopologyID < m_vac->GetTopologyUpdateID() )
    {
        m_box = CalculateBoundingBox( m_vac );
        AddBoxToVAC( m_vac, m_box ); // FIXME: update and not add
        m_lastTopologyID = m_vac->GetTopologyUpdateID();
        assert( m_lastAttribuetesID == m_vac->GetAttributesUpdateID() );
    }
}

} }
