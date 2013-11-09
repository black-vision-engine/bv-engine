#include "RenderablePass.h"

#include "Engine\Graphics\Shaders\PixelShader.h"
#include "Engine\Graphics\Shaders\VertexShader.h"
#include "Engine\Graphics\Shaders\GeometryShader.h"

namespace bv
{

//FIXME: add states here

// *********************************
//
RenderablePass::RenderablePass( PixelShader * ps, VertexShader * vs, GeometryShader * gs )
    : m_geometryShader(gs)
    , m_vertexShader(vs)
    , m_pixelShader(ps)
{
    m_stateInstance = new StateInstance();
}

// *********************************
//
RenderablePass::~RenderablePass         ()
{
    m_stateInstance->DeleteStates();

    delete m_stateInstance;
}

// *********************************
//
PixelShader *       RenderablePass::GetPixelShader         ()
{
    return m_pixelShader;
}

// *********************************
//
VertexShader *      RenderablePass::GetVertexShader        ()
{
    return m_vertexShader;
}

// *********************************
//
GeometryShader *    RenderablePass::GetGeometryShader      ()
{
    return m_geometryShader;
}

// *********************************
//
StateInstance *     RenderablePass::GetStateInstance        ()
{
    return m_stateInstance;
}

// *********************************
//
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
