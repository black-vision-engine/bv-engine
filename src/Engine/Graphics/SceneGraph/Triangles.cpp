#include "Triangles.h"

#include "VertexDescriptor.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RenderableEffect.h"

namespace bv {

// *********************************
//
Triangles::Triangles               ( VertexArray * vao, VertexDescriptor * vd, VertexBuffer * vb, IndexBuffer * ib, RenderableEffect * effect, RenderableType type )
    : RenderableEntity( type, vao, vd, vb, ib, effect )
{
}

// *********************************
//
Triangles::~Triangles              ()
{

}

}