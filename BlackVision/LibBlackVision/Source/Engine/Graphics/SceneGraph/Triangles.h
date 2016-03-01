#pragma once

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"


namespace bv {

class RenderableArrayDataArraysSingleVertexBuffer;
class RenderableEffect;

namespace mathematics {
struct Box;
}

class Triangles : public RenderableEntity
{
    const mathematics::Box *        m_boundingBox;
public:

                                    Triangles               ( RenderableArrayDataSingleVertexBuffer * rad, const mathematics::Box * boundingBox, RenderableEffectPtr effect, RenderableType type = RenderableType::RT_TRIANGLES );
                                    ~Triangles              ();

    virtual int                     NumTriangles    ( unsigned int ccNum ) const = 0;
    virtual int                     NumVertices     ( unsigned int ccNum ) const = 0;

    virtual const mathematics::Box * GetBoundingBox  () const;

    //TODO:
    //virtual GetVertex
    //virtual GetTriangle
    //Calculate normal
    //Some bounding box, volume methods
    //Implement Updating vertex normals and reference frames per vertex if necessary
};

}
