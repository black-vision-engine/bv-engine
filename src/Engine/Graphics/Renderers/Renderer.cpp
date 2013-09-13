#include "Renderer.h"

#include <gl/glew.h>
#include "glutils.h"
#include <cassert>

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrShader.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrIndexBuffer.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrVertexBuffer.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrVertexArrayObject.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrVertexDescriptor.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrTexture2D.h"
#include "Engine/Graphics/Shaders/RenderablePass.h"
#include "Engine/Graphics/Shaders/RenderableEffect.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"
#include "Engine/Graphics/SceneGraph/TriangleStrip.h"
#include "Engine/Graphics/SceneGraph/Camera.h"

namespace bv {

// *********************************
//
void	Renderer::Initialize	    ( int w, int h, Texture::TFormat colorFormat )
{
    assert( w > 0 && h > 0 );

    m_Width			= w;
    m_Height		= h;
    m_ColorFormat	= colorFormat;

    //FIXME: add all renderer states (either as separate classes or as bunch of states stored in member variables, possibly add multisampling support)
    //FIXME: possibly add stencil data
 
    m_Camera		= nullptr;
    m_ClearColor	= glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );
    m_ClearDepth	= 1.0f;
}

// *********************************
//
void	Renderer::SetCamera         (Camera* cam)
{
    m_Camera = cam;
}

// *********************************
//
void	Renderer::Terminate         ()
{
    //FIXME: delete all states and additional resources used exclusively by the renderer (vertex format, bufffer, textures, shaders, render targets)
}

// *********************************
//
void	Renderer::ClearBuffers		()
{
    glClearColor( m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a );
    //glClearColor( 0.f, 0.f,0.f,1.f );
    //FIXME: implement
    //glClearDepth((GLclampd)mClearDepth);
    //glClearStencil((GLint)mClearStencil);
    glClear( GL_COLOR_BUFFER_BIT /*| GL_DEPTH_BUFFER_BIT*/ );
}

// *********************************
//
void	Renderer::SetClearColor		( const glm::vec4 & col )
{
    m_ClearColor = col;
}

// *********************************
//
void	Renderer::Resize			    ( int w, int h )
{
    glViewport(0 , 0, w, h);
   
    //FIXME: implement
}

// *********************************
//
void    Renderer::SetFaceCulling        ( FaceKind face )
{
    glCullFace( ConstantsMapper::GlConstant( face ) );
}

// *********************************
//
void    Renderer::DisableFaceCulling    ()
{
    glDisable(GL_CULL_FACE);
}

// *********************************
//
void    Renderer::EnableFaceCulling     ()
{
    glEnable(GL_CULL_FACE);
}


// *********************************
//
bool    Renderer::PreDraw               ()
{
    return true;
}

// *********************************
//
bool    Renderer::DrawRenderable        ( RenderableEntity* ent )
{
    RenderableEntity::RenderableType type = ent->GetType();

    switch (ent->GetType())
    {
    case RenderableEntity::RenderableType::RT_TRIANGLE_STRIP:
        glDrawArrays(ConstantsMapper::GlConstant(type), 0, static_cast<TriangleStrip*>(ent)->NumVertices() );
        break;
    default:
        assert(!"Should not be here");
    }

    return true;
}

bool    Renderer::Draw                  ( RenderableEntity* ent )
{
    //Based on Entity Type:
    //                        Enable(vertexbuffer);
    //                        Enable(indexBuffer) -> if present
    //                        Enable(vertexArrayObject)
    //                        GetShader:
    //                            Shader->Use
    //                        DrawPrimitive(entity)
    //                           glDrawArrays(vertexArrayObject)
    auto vd     = ent->GetVertexDescriptor();
    auto vb     = ent->GetVertexBuffer();
    auto ib     = ent->GetIndexBuffer();
    auto eff    = ent->GetRenderableEffect();
    auto vao    = ent->GetVertexArray();

    Enable(vb); // FIXME: KOLEJONSC
    if(ib) 
        Enable(ib);

    if (vao)
        Enable(vao);

    Enable(eff->GetPass(0), ent); //FIXME: 1 pass

    //glPolygonMode(GL_FRONT, GL_LINE);
    //glPolygonMode(GL_BACK, GL_LINE);

    DrawRenderable( ent );

    //Disable(eff->GetPass(0));  //FIXME:
    //Disable(vb, vd);
    //if(ib) 
    //    Disable(ib);

    //Disable(vb);


    return true;
}

// *********************************
//
bool    Renderer::PostDraw              ()
{
    return true;
}

// *********************************
//
void    Renderer::Enable              ( RenderablePass* pass, TransformableEntity* transform )
{
    auto it = m_PdrShaderMap.find(pass);

    PdrShader* shader = nullptr;

    if(it == m_PdrShaderMap.end())
    {
        shader = PdrShader::Create(pass->GetPixelShader(), pass->GetVertexShader(), pass->GetGeometryShader());
        m_PdrShaderMap[pass] = shader;
    }
    else
    {
        shader = it->second;
    }

    pass->Update();

    auto projMat = glm::mat4(1.f);

    if(m_Camera)
    {
        projMat = m_Camera->GetProjectionMatrix();
    }

    auto worldTrans = transform->WorldTransform().Matrix();
    auto mvp        = projMat * worldTrans;
    pass->GetVertexShader()->UpdateMVP( mvp );
    pass->GetVertexShader()->UpdateMV( worldTrans );
    pass->GetVertexShader()->UpdateP( projMat );

    if(auto gs = pass->GetGeometryShader())
    {
        gs->UpdateMVP( mvp );
        gs->UpdateMV( worldTrans );
        gs->UpdateP( projMat );
    }

    shader->Enable( this );
}

// *********************************
//
void    Renderer::Enable              ( VertexBuffer * vb )
{
    PdrVertexBuffer * pvb = GetPdrVertexBuffer( vb );
    pvb->Enable( this );
}

// *********************************
//
void    Renderer::Enable              ( IndexBuffer * ib )
{
    PdrIndexBuffer * pib = GetPdrIndexBuffer( ib );
    pib->Enable( this );
}

// *********************************
//
void    Renderer::Enable              ( VertexArray * vao )
{
    PdrVertexArrayObject  * pvao = GetPdrVertexArray( vao );
    pvao->Enable( this );    
}

// *********************************
//
void    Renderer::Enable              ( Texture2D * texture )
{

}

// *********************************
//
PdrVertexBuffer *           Renderer::GetPdrVertexBuffer        ( VertexBuffer * vb )
{
    auto it = m_PdrVertexBufferMap.find( vb );

    PdrVertexBuffer * vBuffer = nullptr;

    if( it == m_PdrVertexBufferMap.end() )
    {
        vBuffer = new PdrVertexBuffer( this, vb );
        m_PdrVertexBufferMap[ vb ] = vBuffer;
    }
    else
    {
        vBuffer = it->second;
    }

    return vBuffer;
}

// *********************************
//
PdrIndexBuffer *            Renderer::GetPdrIndexBuffer         ( IndexBuffer * ib )
{
    auto it = m_PdrIndexBufferMap.find( ib );

    PdrIndexBuffer * iBuffer = nullptr;

    if( it == m_PdrIndexBufferMap.end() )
    {
        iBuffer = new PdrIndexBuffer( this, ib );
        m_PdrIndexBufferMap[ ib ] = iBuffer;
    }
    else
    {
        iBuffer = it->second;
    }

    return iBuffer;
}

// *********************************
//
PdrVertexDescriptor *       Renderer::GetPdrVertexDescriptor    ( VertexDescriptor* vd  )
{
    auto it = m_PdrVertexDescriptorMap.find( vd );

    PdrVertexDescriptor * pdrVd = nullptr;

    if( it == m_PdrVertexDescriptorMap.end() )
    {
        pdrVd = new PdrVertexDescriptor( this, vd );
        m_PdrVertexDescriptorMap[ vd ] = pdrVd;
    }
    else
    {
        pdrVd = it->second;
    }

    return pdrVd;
}

// *********************************
//
PdrVertexArrayObject *         Renderer::GetPdrVertexArray         ( VertexArray * vao )
{
    auto it = m_PdrVertexArrayObjectMap.find( vao );

    PdrVertexArrayObject * pdrVao = nullptr;

    if( it == m_PdrVertexArrayObjectMap.end() )
    {
        pdrVao = new PdrVertexArrayObject( this, vao );
        m_PdrVertexArrayObjectMap[ vao ] = pdrVao;
    }
    else
    {
        pdrVao = it->second;
    }

    return pdrVao;
}

PdrTexture2D *                  Renderer::GetPdrTexture2D         ( Texture2D * texture )
{
    auto it = m_PdrTextures2DMap.find( texture );

    PdrTexture2D* pdrTex = nullptr;

    if( it == m_PdrTextures2DMap.end() )
    {
        pdrTex = PdrTexture2D::Create( texture );
        m_PdrTextures2DMap[ texture ] = pdrTex;
    }
    else
    {
        pdrTex = it->second;
    }

    return pdrTex;
}


}
