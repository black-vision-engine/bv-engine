#pragma once

#include "Engine/Graphics/SceneGraph/RenderableEntityWithBoundingBox.h"


namespace bv {

class RenderableArrayDataArraysSingleVertexBuffer;
class RenderableEffect;

namespace model { class BoundingVolume; }

class Triangles : public RenderableEntityWithBoundingBox
{

public:

            Triangles               ( RenderableArrayDataSingleVertexBuffer * rad, const model::BoundingVolume * boundingBox, RenderableEffectPtr effect, RenderableType type = RenderableType::RT_TRIANGLES );
            ~Triangles              ();

    virtual int     NumTriangles    ( unsigned int ccNum ) const;
    virtual int     NumVertices     ( unsigned int ccNum ) const;

    //TODO:
    //virtual GetVertex
    //virtual GetTriangle
    //Calculate normal
    //Some bounding box, volume methods
    //Implement Updating vertex normals and reference frames per vertex if necessary
};

}
