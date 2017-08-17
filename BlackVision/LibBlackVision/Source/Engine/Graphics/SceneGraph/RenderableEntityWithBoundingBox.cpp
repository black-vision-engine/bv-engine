#include "stdafx.h"

#include "RenderableEntityWithBoundingBox.h"

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannelDescriptor.h"

#include "Engine/Editors/BVProjectTools.h"

#include "Engine/Models/BoundingVolume.h"

#include "Engine/Graphics/SceneGraph/Lines.h"



#include "Memory/MemoryLeaks.h"



namespace bv {


namespace {

// *******************************
//
RenderableEntity *  BuildRenderableBoundingBox          ( const model::BoundingVolume * volume )
{
    auto component = volume->BuildBoxRepresentation();

    auto box = Cast< Lines * >( BVProjectTools::BuildRenderableFromComponent( component, PrimitiveType::PT_LINES ) );

    box->SetWidth( 5.f );

    return box;
}

// *******************************
//
RenderableEntity *  BuildRenderableCenterOfMass          ( const model::BoundingVolume * volume )
{
    auto component = volume->BuildCenterRepresentation();

    auto box = Cast< Lines * >( BVProjectTools::BuildRenderableFromComponent( component, PrimitiveType::PT_LINES ) );

    box->SetWidth( 5.f );

    return box;
}

}

// ***********************
//
RenderableEntityWithBoundingBox::RenderableEntityWithBoundingBox         ( RenderableType type, RenderableArrayDataSingleVertexBuffer * rad, const model::BoundingVolume * boundingBox, RenderableEffectPtr effect )
    : RenderableEntity( type, rad, effect )
    , m_renderableBoundingBox( nullptr )
    , m_renderableCenterOfMass( nullptr )

{
    if( boundingBox )
    {
        m_renderableBoundingBox = std::unique_ptr< RenderableEntity >( BuildRenderableBoundingBox( boundingBox ) );
        m_renderableCenterOfMass = std::unique_ptr< RenderableEntity >( BuildRenderableCenterOfMass( boundingBox ) );
    }
}

// ***********************
//
RenderableEntity *    RenderableEntityWithBoundingBox::GetBoundingBox                          () const
{
    return m_renderableBoundingBox.get();
}

// ***********************
//
RenderableEntity *          RenderableEntityWithBoundingBox::GetCenterOfMass                         () const
{
    return m_renderableCenterOfMass.get();
}

// ***********************
//
void                RenderableEntityWithBoundingBox::UpdateTransform                         ( const Transform & transform )
{
    RenderableEntity::UpdateTransform( transform );
    if( m_renderableBoundingBox )
    {
        m_renderableBoundingBox->UpdateTransform( transform );
        m_renderableCenterOfMass->UpdateTransform( transform );
    }
}

// ***********************
//
void                RenderableEntityWithBoundingBox::SetLocalTransform                       ( const Transform & t )
{
    RenderableEntity::SetLocalTransform( t );
    if( m_renderableBoundingBox )
    {
        m_renderableBoundingBox->SetLocalTransform( t );
        m_renderableCenterOfMass->SetLocalTransform( t );
    }
}

// ***********************
//
void                RenderableEntityWithBoundingBox::SetLocalTransform                       ( Transform && t )
{
	RenderableEntity::SetLocalTransform( t );
	if( m_renderableBoundingBox )
	{
		m_renderableBoundingBox->SetLocalTransform( t );
		m_renderableCenterOfMass->SetLocalTransform( t );
	}
}

}
