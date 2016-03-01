#include "stdafx.h"

#include "TriangleStrip.h"

#include "Engine/Graphics/Resources/RenderableArrayDataArrays.h"
#include "Engine/Graphics/Shaders/RenderableEffect.h"

#include "Mathematics/Box.h"

namespace bv {

// **************************
//
TriangleStrip::TriangleStrip   ( RenderableArrayDataArraysSingleVertexBuffer * rad, const mathematics::Box * boundingBox, RenderableEffectPtr effect )
    : Triangles( static_cast< RenderableArrayDataSingleVertexBuffer * >( rad ), boundingBox, effect, RenderableType::RT_TRIANGLE_STRIP )
{
}

// **************************
//
TriangleStrip::~TriangleStrip  ()
{
}

// **************************
//
int     TriangleStrip::NumTriangles    ( unsigned int ccNum ) const
{
    return NumVertices( ccNum ) - 2;
}

// **************************
//
int     TriangleStrip::NumVertices     ( unsigned int ccNum ) const
{
    return RAD()->GetNumVerticesInConnectedComponent( ccNum );
}

}
