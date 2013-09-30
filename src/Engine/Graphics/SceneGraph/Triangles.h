#pragma once

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

namespace bv {

class RenderableArrayData;
class RenderableEffect;

class Triangles : public RenderableEntity
{

public:

            Triangles               ( RenderableArrayData * rad, RenderableEffect * effect, RenderableType type = RenderableType::RT_TRIANGLES );
            ~Triangles              ();

    virtual int     NumTriangles    ( int ccNum ) const = 0;
    virtual int     NumVertices     ( int ccNum ) const = 0;

    //TODO:
    //virtual GetVertex
    //virtual GetTriangle
    //Calculate normal
    //Some bounding box, volume methods
    //Implement Updating vertex normals and reference frames per vertex if necessary
};

}
