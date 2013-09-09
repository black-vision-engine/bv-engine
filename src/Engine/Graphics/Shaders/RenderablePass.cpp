#include "RenderablePass.h"

#include "Engine\Graphics\Shaders\PixelShader.h"
#include "Engine\Graphics\Shaders\VertexShader.h"
#include "Engine\Graphics\Shaders\GeometryShader.h"

namespace bv
{

RenderablePass::RenderablePass( PixelShader * ps, VertexShader * vs, GeometryShader * gs )
    : m_geometryShader(gs)
    , m_vertexShader(vs)
    , m_pixelShader(ps)
{}

PixelShader *       RenderablePass::GetPixelShader         ()
{
    return m_pixelShader;
}

VertexShader *      RenderablePass::GetVertexShader        ()
{
    return m_vertexShader;
}

GeometryShader *    RenderablePass::GetGeometryShader      ()
{
    return m_geometryShader;
}

void                RenderablePass::Update                 ()
{
    if(m_pixelShader)
        m_pixelShader->Update();

    if(m_vertexShader)
        m_vertexShader->Update();

    if(m_geometryShader)
        m_geometryShader->Update();

}


} // bv
