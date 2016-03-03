#include "stdafx.h"

#include "RenderableEntityWithBoundingBox.h"

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannelDescriptor.h"

#include "Engine/Models/BVProjectTools.h"

#include "Mathematics/Box.h"

namespace bv {


namespace {

model::ConnectedComponentPtr BuildComponentFromBox( const mathematics::Box * box )
{
    auto comp = model::ConnectedComponent::Create();

    auto * compVertDesc = new model::AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );

    auto vertArrtF3 = std::make_shared< model::Float3AttributeChannel >( compVertDesc, "boundingBox", false );

    comp->AddAttributeChannel( vertArrtF3 );

    vertArrtF3->AddAttribute( glm::vec3( box->xmin, box->ymin, box->zmin ) );
    vertArrtF3->AddAttribute( glm::vec3( box->xmin, box->ymax, box->zmin ) );
    vertArrtF3->AddAttribute( glm::vec3( box->xmax, box->ymin, box->zmin ) );
    vertArrtF3->AddAttribute( glm::vec3( box->xmax, box->ymax, box->zmin ) );
    vertArrtF3->AddAttribute( glm::vec3( box->xmax, box->ymin, box->zmax ) );
    vertArrtF3->AddAttribute( glm::vec3( box->xmax, box->ymax, box->zmax ) );
    vertArrtF3->AddAttribute( glm::vec3( box->xmin, box->ymin, box->zmax ) );
    vertArrtF3->AddAttribute( glm::vec3( box->xmin, box->ymax, box->zmax ) );

    return comp;
}

// *******************************
//
RenderableEntity *  BuildRenderableBoundingBox          ( const mathematics::Box * box )
{
    auto component = BuildComponentFromBox( box );

    return BVProjectTools::BuildRenderableFromComponent( component );
}

}

// ***********************
//
RenderableEntityWithBoundingBox::RenderableEntityWithBoundingBox         ( RenderableType type, RenderableArrayDataSingleVertexBuffer * rad, const mathematics::Box * boundingBox, RenderableEffectPtr effect )
    : RenderableEntity( type, rad, effect )
{
    if( boundingBox )
        m_renderableBoundingBox = BuildRenderableBoundingBox( boundingBox );
}


// ***********************
//
RenderableEntity *    RenderableEntityWithBoundingBox::GetBoundingBox                          () const
{
    return m_renderableBoundingBox;
}


}
