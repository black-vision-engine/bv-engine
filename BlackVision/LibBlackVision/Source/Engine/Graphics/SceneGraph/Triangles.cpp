#include "stdafx.h"

#include "Triangles.h"

#include "Engine/Graphics/Resources/RenderableArrayData.h"
#include "Engine/Graphics/Shaders/RenderableEffect.h"


namespace bv
{

// *********************************
//
Triangles::Triangles               ( RenderableArrayDataSingleVertexBuffer * rad, const mathematics::Box * boundingBox, RenderableEffectPtr effect, RenderableType type )
    : RenderableEntityWithBoundingBox( type, rad, boundingBox, effect )
{
}

// *********************************
//
Triangles::~Triangles              ()
{
}

} //bv
