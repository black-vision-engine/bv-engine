#include "stdafx.h"

#include "Triangles.h"

#include "Engine/Graphics/Resources/RenderableArrayData.h"
#include "Engine/Graphics/Shaders/RenderableEffect.h"

#include "Mathematics/Box.h"

namespace bv
{

// *********************************
//
Triangles::Triangles               ( RenderableArrayDataSingleVertexBuffer * rad, const mathematics::Box * boundingBox, RenderableEffectPtr effect, RenderableType type )
    : RenderableEntity( type, rad, effect )
    , m_boundingBox( boundingBox )
{
}

// *********************************
//
Triangles::~Triangles              ()
{
}

// ***********************
//
const mathematics::Box *      Triangles::GetBoundingBox  () const
{
    return m_boundingBox;
}

} //bv
