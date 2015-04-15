#include "RenderablePass.h"

#include "Engine/Graphics/Shaders/PixelShader.h"
#include "Engine/Graphics/Shaders/VertexShader.h"
#include "Engine/Graphics/Shaders/GeometryShader.h"

#include "Engine/Graphics/SceneGraph/Camera.h"


namespace bv
{

//FIXME: add states here

// *********************************
//
RenderablePass::RenderablePass( PixelShader * ps, VertexShader * vs, GeometryShader * gs )
    : m_geometryShader( gs )
    , m_vertexShader( vs )
    , m_pixelShader( ps )
{
    m_stateInstance = new RendererStateInstance();
}

// *********************************
//
RenderablePass::~RenderablePass         ()
{
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
RendererStateInstance *     RenderablePass::GetStateInstance        ()
{
    return m_stateInstance;
}

// *********************************
//
void                RenderablePass::Update                 ( RenderableEntity * renderable, Camera * camera )
{
    //PixelShader and VertexShader must allways be present, no if is necessary here
    //if( m_pixelShader )
    m_pixelShader->Update( renderable, camera );

    //if( m_vertexShader )
    m_vertexShader->Update( renderable, camera );

    if( m_geometryShader )
        m_geometryShader->Update( renderable, camera );
}

} // bv
