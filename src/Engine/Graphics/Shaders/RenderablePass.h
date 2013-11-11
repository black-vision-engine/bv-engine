#pragma once

#include "Engine/Graphics/Shaders/PixelShader.h"
#include "Engine/Graphics/Shaders/VertexShader.h"
#include "Engine/Graphics/Shaders/GeometryShader.h"

#include "Engine/Graphics/State/StateInstance.h"


namespace bv {

class RenderablePass
{
private:

    StateInstance *     m_stateInstance;

    PixelShader *       m_pixelShader;
    VertexShader *      m_vertexShader;
    GeometryShader *    m_geometryShader;

public:

                        RenderablePass          ( PixelShader * ps, VertexShader * vs, GeometryShader * gs );
                        ~RenderablePass         ();

    void                Update                  ();

    PixelShader *       GetPixelShader          ();
    VertexShader *      GetVertexShader         ();
    GeometryShader *    GetGeometryShader       ();

    StateInstance *     GetStateInstance        ();

};

}
