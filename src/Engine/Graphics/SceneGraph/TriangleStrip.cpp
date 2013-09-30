#include "TriangleStrip.h"

#include "Engine\Graphics\Resources\RenderableArrayData.h"
#include "Engine\Graphics\Shaders\RenderableEffect.h"

namespace bv {

// **************************
//
TriangleStrip::TriangleStrip   ( RenderableArrayData * rad, RenderableEffect * effect )
    : Triangles( rad, effect, RenderableType::RT_TRIANGLE_STRIP )
{
}

// **************************
//
TriangleStrip::~TriangleStrip  ()
{
}

// **************************
//
int     TriangleStrip::NumTriangles    ( int ccNum ) const
{
    return NumVertices( ccNum ) - 2;
}

// **************************
//
int     TriangleStrip::NumVertices     ( int ccNum ) const
{
    assert( ccNum >= 0 );
    assert( ccNum < GetNumconnectedComponents() );

    return RAD()->GetCCNumVertices( ccNum );
}

}
