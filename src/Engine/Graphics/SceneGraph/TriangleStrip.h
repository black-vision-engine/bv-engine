#pragma once

#include "Triangles.h"

namespace bv {

class TriangleStrip : public Triangles
{
private:

public:

    TriangleStrip   ( VertexDescriptor * vd, VertexBuffer * vb, RenderableEffect * effect );
    ~TriangleStrip  ();

    virtual int     NumTriangles    () const override;
    virtual int     NumVertices     () const override;

};

}
