#pragma once

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

namespace bv {

class RenderableArrayDataArraysSingleVertexBuffer;
class RenderableEffect;

class Triangles : public RenderableEntity
{

public:

            Triangles               ( RenderableArrayDataSingleVertexBuffer * rad, RenderableEffect * effect, RenderableType type = RenderableType::RT_TRIANGLES );
            ~Triangles              ();

    virtual int     NumTriangles    ( unsigned int ccNum ) const = 0;
    virtual int     NumVertices     ( unsigned int ccNum ) const = 0;

    //TODO:
    //virtual GetVertex
    //virtual GetTriangle
    //Calculate normal
    //Some bounding box, volume methods
    //Implement Updating vertex normals and reference frames per vertex if necessary
};

}
