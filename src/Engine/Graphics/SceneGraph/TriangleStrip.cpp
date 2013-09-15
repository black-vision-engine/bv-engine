#include "TriangleStrip.h"

#include "VertexDescriptor.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "RenderableEffect.h"

namespace bv {

// **************************
//
TriangleStrip::TriangleStrip   ( VertexArray * vao, RenderableEffect * effect )
    : Triangles( vao, nullptr, nullptr, nullptr, effect, RenderableType::RT_TRIANGLE_STRIP )
{
}

// **************************
//
TriangleStrip::~TriangleStrip  ()
{
}

// **************************
//
int     TriangleStrip::NumTriangles    () const
{
    return NumVertices() - 2;
}

// **************************
//
int     TriangleStrip::NumVertices     () const
{
    return m_vao->Entry( 0 ).vertexBuffer->NumElements();
}

}
