#pragma once

#include "Engine/Graphics/Shaders/PixelShader.h"
#include "Engine/Graphics/Shaders/VertexShader.h"
#include "Engine/Graphics/Shaders/GeometryShader.h"

#include "Engine/Graphics/State/RendererStateInstance.h"


namespace bv {

class RenderableEntity;
class Camera;

class RenderablePass
{
private:

    RendererStateInstance *     m_stateInstance;

    PixelShader *       m_pixelShader;
    VertexShader *      m_vertexShader;
    GeometryShader *    m_geometryShader;

public:

                        RenderablePass          ( PixelShader * ps, VertexShader * vs, GeometryShader * gs );
                        ~RenderablePass         ();

    void                Update                  ( RenderableEntity * renderable, Camera * camera );

    PixelShader *       GetPixelShader          ();
    VertexShader *      GetVertexShader         ();
    GeometryShader *    GetGeometryShader       ();

    RendererStateInstance *     GetStateInstance        ();

};

} //bv
