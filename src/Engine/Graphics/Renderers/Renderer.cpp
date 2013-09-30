#include "Renderer.h"

#include <gl/glew.h>
#include "glutils.h"
#include <cassert>

#include "Engine/Graphics/Renderers/OGLRenderer/PdrShader.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrIndexBuffer.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrVertexBuffer.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrVertexArrayObject.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrVertexArrayObjectSingleVB.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrVertexDescriptor.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrTexture2D.h"
#include "Engine/Graphics/Shaders/RenderablePass.h"
#include "Engine/Graphics/Shaders/RenderableEffect.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"
#include "Engine/Graphics/SceneGraph/TriangleStrip.h"
#include "Engine/Graphics/SceneGraph/Camera.h"

#include "Engine\Graphics\Resources\VertexBuffer.h"
#include "Engine\Graphics\Resources\VertexArray.h"
#include "Engine\Graphics\Resources\RenderableArrayDataArrays.h"
//FIXME: add disable methods so that current state can be cleared after frame is rendered

namespace bv {

// *********************************
//
void	Renderer::Initialize	    ( int w, int h, TextureFormat colorFormat )
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
bool    Renderer::DrawRenderable        ( RenderableEntity * ent )
{
    RenderableEntity::RenderableType type = ent->GetType();

    switch (ent->GetType())
    {
    case RenderableEntity::RenderableType::RT_TRIANGLE_STRIP:
        DrawTriangleStrips( static_cast< TriangleStrip * >( ent ) );
        //FIXME: FIX-1
        //glDrawArrays(ConstantsMapper::GlConstant(type), 0, static_cast<TriangleStrip*>(ent)->NumVertices() );
        break;
    default:
        assert(!"Should not be here");
    }

    return true;
}

// *********************************
//
bool     Renderer::DrawTriangleStrips      ( TriangleStrip * strip )
{
    static GLuint mode = ConstantsMapper::GlConstant( RenderableEntity::RenderableType::RT_TRIANGLE_STRIP );

    const VertexArraySingleVertexBuffer * vao = static_cast< const RenderableArrayDataArraysSingleVertexBuffer * >( strip->GetRenderableArrayData() )->VAO();

    Enable  ( vao );

    unsigned int firstVertex = 0;
    for( unsigned int i = 0; i < vao->GetNumConnectedComponents(); ++i )
    {
        unsigned int numVertices = vao->GetNumVertices( i );
        glDrawArrays( mode, firstVertex, numVertices );
        firstVertex += numVertices;
    }

    Disable ( vao );

    return true;
}

// *********************************
//FIXME: add disable methods so that current state can be cleared after frame is rendered
//FIXME: This one requires one solid reimplementation
bool    Renderer::Draw                  ( RenderableEntity * ent )
{
    //Based on Entity Type:
    //                        Enable(vertexbuffer);
    //                        Enable(indexBuffer) -> if present
    //                        Enable(vertexArrayObject)
    //                        GetShader:
    //                            Shader->Use
    //                        DrawPrimitive(entity)
    //                           glDrawArrays(vertexArrayObject)
    auto eff    = ent->GetRenderableEffect();

    //FIXME: instancing should also be implemented somewhere here
    //FIXME: read how http://www.opengl.org/sdk/docs/man/xhtml/glDrawArraysInstanced.xml
    //FIXME: works
    //FIXME: additional rendering branch (and engine/pdr classes are required for glDrawElements call - draw based on indices)
    
    //FIXME: effect should only bind glsl program and set per frame paramters
    //FIXME: then there should be an instancing loop with MVP binding and then entity rendering
    Enable( eff->GetPass( 0 ), ent ); //FIXME: 1 pass ONLY RIGHT NOW

    DrawRenderable( ent );

    //glPolygonMode(GL_FRONT, GL_LINE);
    //glPolygonMode(GL_BACK, GL_LINE);

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
void    Renderer::Enable              ( RenderablePass * pass, TransformableEntity * transform )
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

    //FIXME: add proper loop here
    auto worldTrans = transform->WorldTransforms()[0].Matrix(); //FIXME: instancing to be added here
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
void    Renderer::Enable              ( const VertexBuffer * vb )
{
    PdrVertexBuffer * pvb = GetPdrVertexBuffer( vb );
    pvb->Enable( this );
}

// *********************************
//
void    Renderer::Enable              ( const IndexBuffer * ib )
{
    PdrIndexBuffer * pib = GetPdrIndexBuffer( ib );
    pib->Enable( this );
}

// *********************************
//
void    Renderer::Enable              ( const VertexArray * vao )
{
    PdrVertexArrayObject  * pvao = GetPdrVertexArray( vao );
    pvao->Enable( this );    
}

// *********************************
//
void    Renderer::Enable              ( const VertexArraySingleVertexBuffer * vao )
{
    PdrVertexArrayObjectSingleVB  * pvao = GetPdrVertexArraySingleVB( vao );
    pvao->Enable( this );        
}

// *********************************
//
void    Renderer::Disable             ( const VertexArraySingleVertexBuffer * vao )
{
    PdrVertexArrayObjectSingleVB  * pvao = GetPdrVertexArraySingleVB( vao );
    pvao->Disable( this );        
}

// *********************************
//
void    Renderer::Disable             ( const  VertexBuffer * vb )
{
    PdrVertexBuffer * pdrVb = GetPdrVertexBuffer( vb );
    pdrVb->Disable( this );
}

// *********************************
//
void    Renderer::Enable              ( const Texture2D * texture, int textureUnit )
{
    PdrTexture2D * pdrTex2D = GetPdrTexture2D( texture );
    pdrTex2D->Enable( this, textureUnit );
}

// *********************************
//
void    Renderer::Disable             ( const Texture2D * texture, int textureUnit )
{
    PdrTexture2D * pdrTex2D = GetPdrTexture2D( texture );
    pdrTex2D->Disable( this, textureUnit );
}

// *********************************
//
PdrVertexBuffer *           Renderer::GetPdrVertexBuffer        ( const VertexBuffer * vb )
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
PdrIndexBuffer *            Renderer::GetPdrIndexBuffer         ( const IndexBuffer * ib )
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
PdrVertexDescriptor *       Renderer::GetPdrVertexDescriptor    ( const VertexDescriptor* vd  )
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
PdrVertexArrayObject *         Renderer::GetPdrVertexArray         ( const VertexArray * vao )
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

// *********************************
//
PdrVertexArrayObjectSingleVB *  Renderer::GetPdrVertexArraySingleVB   ( const VertexArraySingleVertexBuffer * vao )
{
    auto it = m_PdrVertexArrayObjectSingleVBMap.find( vao );

    PdrVertexArrayObjectSingleVB * pdrVao = nullptr;

    if( it == m_PdrVertexArrayObjectSingleVBMap.end() )
    {
        pdrVao = new PdrVertexArrayObjectSingleVB( this, vao );
        m_PdrVertexArrayObjectSingleVBMap[ vao ] = pdrVao;
    }
    else
    {
        pdrVao = it->second;
    }

    return pdrVao;
}

// *********************************
//
PdrTexture2D *                  Renderer::GetPdrTexture2D         ( const Texture2D * texture )
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


    //FIXME: remove
    //const float * fData = (const float *) vb->Data();
    //unsigned int vertSize = 5;
    //unsigned int rectSize = vertSize * 4;
    //for( unsigned int i = 0; i < numCC; ++i )
    //{
    //    std::cout << std::endl << "Rectangle " << i << std::endl;
    //    const float * rctData = &fData[ i * rectSize ];

    //    std::cout << "Pos [" << rctData[ 0 ] << ", " << rctData[ 1 ] << ", " << rctData[ 2 ] << "] ";
    //    std::cout << "[" << rctData[ 5 ] << ", " << rctData[ 6 ] << ", " << rctData[ 7 ] << "] ";
    //    std::cout << "[" << rctData[ 10 ] << ", " << rctData[ 11 ] << ", " << rctData[ 12 ] << "] ";
    //    std::cout << "[" << rctData[ 15 ] << ", " << rctData[ 16 ] << ", " << rctData[ 17 ] << "] " << std::endl;
    //    std::cout << "uv [" << rctData[ 3 ] << ", " << rctData[ 4 ] << "]";
    //    std::cout << "[" << rctData[ 8 ] << ", " << rctData[ 9 ] << "]";
    //    std::cout << "[" << rctData[ 13 ] << ", " << rctData[ 14 ] << "]";
    //    std::cout << "[" << rctData[ 18 ] << ", " << rctData[ 19 ] << "]" << std::endl;

    //}

