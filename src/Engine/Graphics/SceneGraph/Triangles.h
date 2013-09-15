#pragma once

#include "RenderableEntity.h"

namespace bv {

class VertexDescriptor;
class VertexBuffer;
class IndexBuffer;
class VertexArray;
class RenderableEffect;

class Triangles : public RenderableEntity
{

public:

            Triangles               ( VertexArray * vao, VertexDescriptor * vd, VertexBuffer * vb, IndexBuffer * ib, RenderableEffect * effect, RenderableType type = RenderableType::RT_TRIANGLES );
            ~Triangles              ();

    virtual int     NumTriangles    () const = 0;
    virtual int     NumVertices     () const = 0;

    //TODO:
    //virtual GetVertex
    //virtual GetTriangle
    //Calculate normal
    //Some bounding box, volume methods
    //Implement Updating vertex normals and reference frames per vertex if necessary
};

}
