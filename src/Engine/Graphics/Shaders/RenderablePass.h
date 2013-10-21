#pragma once

#include "PixelShader.h"
#include "VertexShader.h"
#include "GeometryShader.h"

namespace bv {

//FIXME: add states here

class RenderablePass
{
private:

    PixelShader *       m_pixelShader;
    VertexShader *      m_vertexShader;
    GeometryShader *    m_geometryShader;

public:

    RenderablePass                          ( PixelShader * ps, VertexShader * vs, GeometryShader * gs );

    void                Update                 ();

    PixelShader *       GetPixelShader         ();
    VertexShader *      GetVertexShader        ();
    GeometryShader *    GetGeometryShader      ();
};

}
