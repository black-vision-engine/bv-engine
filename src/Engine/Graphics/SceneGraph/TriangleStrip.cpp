#include "TriangleStrip.h"

#include "VertexDescriptor.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RenderableEffect.h"

namespace bv {

// **************************
//
TriangleStrip::TriangleStrip   ( VertexDescriptor * vd, VertexBuffer * vb, RenderableEffect * effect )
    : Triangles( vd, vb, nullptr, effect, RenderableType::RT_TRIANGLE_STRIP )
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
    return m_vBuf->NumElements() - 2;
}

// **************************
//
int     TriangleStrip::NumVertices     () const
{
    return m_vBuf->NumElements();
}

}
