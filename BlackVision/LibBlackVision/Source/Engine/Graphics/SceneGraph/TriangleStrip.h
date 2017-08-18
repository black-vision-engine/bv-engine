#pragma once

#include "Engine/Graphics/SceneGraph/Triangles.h"


namespace bv {

class RenderableArrayDataArraysSingleVertexBuffer;

class TriangleStrip : public Triangles
{
public:

    TriangleStrip   ( RenderableArrayDataArraysSingleVertexBuffer * rad, RenderableEffectPtr effect );
    ~TriangleStrip  ();

    virtual int     NumTriangles    ( unsigned int ccNum ) const override;
    virtual int     NumVertices     ( unsigned int ccNum ) const override;

};

} // bv
