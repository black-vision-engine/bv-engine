#include "TriangleStrip.h"

#include "Engine/Graphics/Resources/RenderableArrayDataArrays.h"
#include "Engine/Graphics/Shaders/RenderableEffect.h"


namespace bv {

// **************************
//
TriangleStrip::TriangleStrip   ( RenderableArrayDataArraysSingleVertexBuffer * rad, RenderableEffect * effect )
    : Triangles( static_cast< RenderableArrayDataSingleVertexBuffer * >( rad ), effect, RenderableType::RT_TRIANGLE_STRIP )
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
