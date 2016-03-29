#include "stdafx.h"

#include "RenderableEntityWithBoundingBox.h"

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannelDescriptor.h"

#include "Engine/Models/BVProjectTools.h"

#include "Engine/Models/BoundingVolume.h"


namespace bv {


namespace {

// *******************************
//
RenderableEntity *  BuildRenderableBoundingBox          ( const model::BoundingVolume * volume )
{
    auto component = volume->BuildConnectedComponent();

    return BVProjectTools::BuildRenderableFromComponent( component, PrimitiveType::PT_LINES );
}

}

// ***********************
//
RenderableEntityWithBoundingBox::RenderableEntityWithBoundingBox         ( RenderableType type, RenderableArrayDataSingleVertexBuffer * rad, const model::BoundingVolume * boundingBox, RenderableEffectPtr effect )
    : RenderableEntity( type, rad, effect )
{
    if( boundingBox )
        m_renderableBoundingBox = BuildRenderableBoundingBox( boundingBox );
    else
        m_renderableBoundingBox = nullptr;
}


// ***********************
//
RenderableEntity *    RenderableEntityWithBoundingBox::GetBoundingBox                          () const
{
    return m_renderableBoundingBox;
}


// ***********************
//
void                RenderableEntityWithBoundingBox::UpdateTransform                         ( const Transform & transform )
{
    RenderableEntity::UpdateTransform( transform );
    if( m_renderableBoundingBox )
        m_renderableBoundingBox->UpdateTransform( transform );
}

// ***********************
//
void                RenderableEntityWithBoundingBox::SetLocalTransform                       ( const Transform & t )
{
    RenderableEntity::SetLocalTransform( t );
    if( m_renderableBoundingBox )
        m_renderableBoundingBox->SetLocalTransform( t );
}

}
