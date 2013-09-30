#pragma once

#include "Engine\Graphics\SceneGraph\Triangles.h"

namespace bv {

class RenderableArrayData;

class TriangleStrip : public Triangles
{
public:

    TriangleStrip   ( RenderableArrayData * rad, RenderableEffect * effect );
    ~TriangleStrip  ();

    virtual int     NumTriangles    ( int ccNum ) const override;
    virtual int     NumVertices     ( int ccNum ) const override;

};

}
