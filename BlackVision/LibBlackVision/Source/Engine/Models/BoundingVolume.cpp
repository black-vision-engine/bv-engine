#include "stdafx.h"

#include "BoundingVolume.h"

#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"

#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannelUtils.h"

#include "Engine/Models/Plugins/Parameters/CompositeTypedParameters.h"

//#include <glm/gtc/matrix_transform.hpp>

namespace bv { namespace model {

namespace {

model::ConnectedComponentPtr BuildBoxComponent( const mathematics::Box & box )
{
    auto comp = model::ConnectedComponent::Create();

    auto * compVertDesc = new model::AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );

    auto vertArrtF3 = std::make_shared< model::Float3AttributeChannel >( compVertDesc, "boundingBox", false );

    comp->AddAttributeChannel( vertArrtF3 );

// x-edges
    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymin, box.zmin ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymin, box.zmin ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymax, box.zmin ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymax, box.zmin ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymin, box.zmax ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymin, box.zmax ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymax, box.zmax ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymax, box.zmax ) );

// y-edges
    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymin, box.zmin ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymax, box.zmin ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymin, box.zmin ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymax, box.zmin ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymin, box.zmax ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymax, box.zmax ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymin, box.zmax ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymax, box.zmax ) );

// z-edges
    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymin, box.zmin ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymin, box.zmax ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymax, box.zmin ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymax, box.zmax ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymin, box.zmin ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymin, box.zmax ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymax, box.zmin ) );
    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymax, box.zmax ) );

    return comp;
}

model::ConnectedComponentPtr BuildCenterComponent( const glm::vec3 & center )
{
    auto comp = model::ConnectedComponent::Create();

    auto * compVertDesc = new model::AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );

    auto vertArrtF3 = std::make_shared< model::Float3AttributeChannel >( compVertDesc, "boundingBox", false );

    comp->AddAttributeChannel( vertArrtF3 );

//// x-edges
//    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymin, box.zmin ) );
//    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymin, box.zmin ) );
//    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymax, box.zmin ) );
//    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymax, box.zmin ) );
//    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymin, box.zmax ) );
//    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymin, box.zmax ) );
//    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymax, box.zmax ) );
//    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymax, box.zmax ) );
//
//// y-edges
//    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymin, box.zmin ) );
//    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymax, box.zmin ) );
//    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymin, box.zmin ) );
//    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymax, box.zmin ) );
//    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymin, box.zmax ) );
//    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymax, box.zmax ) );
//    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymin, box.zmax ) );
//    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymax, box.zmax ) );
//
//// z-edges
//    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymin, box.zmin ) );
//    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymin, box.zmax ) );
//    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymax, box.zmin ) );
//    vertArrtF3->AddAttribute( glm::vec3( box.xmin, box.ymax, box.zmax ) );
//    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymin, box.zmin ) );
//    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymin, box.zmax ) );
//    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymax, box.zmin ) );
//    vertArrtF3->AddAttribute( glm::vec3( box.xmax, box.ymax, box.zmax ) );

// "cross-hair" in the "center of mass"
    glm::vec3 mid = center;

    const float scale = 0.3f;

    glm::vec3 sizeX = glm::vec3( scale /** ( box.xmax - box.xmin )*/,   0,                                  0                               );
    glm::vec3 sizeY = glm::vec3( 0,                                 scale /** ( box.ymax - box.ymin )*/,    0                               );
    glm::vec3 sizeZ = glm::vec3( 0,                                 0,                                  scale /** ( box.zmax - box.zmin )*/ );

    vertArrtF3->AddAttribute( mid - sizeX );
    vertArrtF3->AddAttribute( mid + sizeX );
    vertArrtF3->AddAttribute( mid - sizeY );
    vertArrtF3->AddAttribute( mid + sizeY );
    vertArrtF3->AddAttribute( mid - sizeZ );
    vertArrtF3->AddAttribute( mid + sizeZ );

    return comp;
}

} // anonymous



BoundingVolume::BoundingVolume          ( VertexAttributesChannel * vac, ParamTransform * param )
    : m_vac( vac )
    , m_lastAttribuetesID( 0 )
    , m_lastTopologyID( 0 )
    , m_param( param )
{
    if( vac )
        m_box = CalculateBoundingBox( m_vac );
    //m_transform *= glm::translate( glm::mat4( 1.0f ), glm::vec3( box.xmin, box.ymin, box.zmin ) );
    //m_transform *= glm::scale( glm::mat4( 1.0f ), glm::vec3( box.xmax - box.xmin, box.ymax - box.ymin, box.zmax - box.zmin ) );
}

void                    BoundingVolume::Update                  ()
{
    if( m_vac == nullptr )
        return;

    if( m_param )
        m_center = m_param->GetTransform().GetCenter( m_param->GetTimeEvaluator()->GetLocalTime() );
    else
        m_center = glm::vec3( 0, 0, 0 );

    if( m_lastAttribuetesID < m_vac->GetAttributesUpdateID() )
    {
        m_box = CalculateBoundingBox( m_vac );
        m_lastAttribuetesID = m_vac->GetAttributesUpdateID();
    }
    else if( m_lastTopologyID < m_vac->GetTopologyUpdateID() )
    {
        m_box = CalculateBoundingBox( m_vac );
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
IConnectedComponentPtr      BoundingVolume::BuildBoxRepresentation () const
{
    return BuildBoxComponent( m_box );
}

// ***********************
//
IConnectedComponentPtr              BoundingVolume::BuildCenterRepresentation () const
{
    return BuildCenterComponent( m_center );
}

// ***********************
//
void                        BoundingVolume::UpdateVAC               ( const IVertexAttributesChannel * vac_ ) const
{
    if( vac_ == nullptr )
    {
        m_vac = nullptr;
    }
    else
    {
        auto vac = Cast< VertexAttributesChannel * const >( RemoveConst( vac_ ) );

        m_vac = vac;
    }
}

// ***********************
//
void                                BoundingVolume::UpdateParam             ( const ParamTransform * param ) const
{
    m_param = param;
}

} }
