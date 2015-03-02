#include "Renderer.h"

#include <cassert>

#include "OGLRenderer/GL/BVGL.h"

#include "Engine/Graphics/Resources/Texture2D.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrShader.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrIndexBuffer.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrVertexBuffer.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrVertexArrayObject.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrVertexArrayObjectSingleVB.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrVertexDescriptor.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrTexture2D.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrRenderTarget.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrPBOMemTransfer.h"

#include "Engine/Graphics/Shaders/RenderablePass.h"
#include "Engine/Graphics/Shaders/RenderableEffect.h"
#include "Engine/Graphics/SceneGraph/TriangleStrip.h"
#include "Engine/Graphics/SceneGraph/Camera.h"

#include "Engine/Graphics/Resources/VertexBuffer.h"
#include "Engine/Graphics/Resources/VertexArray.h"
#include "Engine/Graphics/Resources/RenderableArrayDataArrays.h"

#include "System/HRTimer.h"

//FIXME: add disable methods so that current state can be cleared after a frame is rendered

namespace bv {

extern HighResolutionTimer GTimer;

// *********************************
//
void	Renderer::Initialize	    ( int w, int h, TextureFormat colorFormat )
{
    assert( w > 0 && h > 0 );

    m_Width			= w;
    m_Height		= h;
    m_ColorFormat	= colorFormat;


    //FIXME: properly implement draw methods
    //FIXME: add all renderer states (either as separate classes or as bunch of states stored in member variables, possibly add multisampling support)
    //FIXME: possibly add stencil data
 
    m_Camera		= nullptr;
    m_ClearColor	= glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );
    m_ClearDepth	= 1.0f;

    m_defaultStateInstance.SetState( new AlphaState() );
    m_defaultStateInstance.SetState( new CullState() );
    m_defaultStateInstance.SetState( new DepthState() );
    m_defaultStateInstance.SetState( new FillState() );
    m_defaultStateInstance.SetState( new OffsetState() );
    m_defaultStateInstance.SetState( new StencilState() );

    m_currentStateInstance = m_defaultStateInstance;

    m_PdrPBOMemTransferRT = nullptr;
}

// *********************************
//
void	Renderer::SetCamera         ( Camera * cam )
{
    m_Camera = cam;
}

// *********************************
//FIXME: most probably state can be stored in RenderData only (no currentStateInstance is required) - but let it be that way for the moment
void    Renderer::SetStateInstance    ( const RendererStateInstance & stateInstance )
{
    m_currentStateInstance.SetStateIgnoreEmptyEntries( stateInstance );
    
    SetAlphaState( m_currentStateInstance.GetAlphaState() );
    SetCullState( m_currentStateInstance.GetCullState() );
    SetDepthState( m_currentStateInstance.GetDepthState() );
    SetFillState( m_currentStateInstance.GetFillState() );
    SetOffsetState( m_currentStateInstance.GetOffsetState() );
    SetStencilState( m_currentStateInstance.GetStencilState() );

    m_RendererData->m_CurrentRS.UpdateState( stateInstance );
}

// *********************************
//
void	Renderer::Terminate         ()
{
    //FIXME: delete all states and additional resources used exclusively by the renderer (vertex format, bufffer, textures, shaders, render targets)
}

// *********************************
// FIXME: stencil is not required here so it is just fine, glClearColor and glClearDepth should be set only once, not every buffer clear - but there is no need to optimize it
void	Renderer::ClearBuffers		()
{
    //FIXME: it should be set once only, when clear color is changed
    BVGL::bvglClearColor( m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a );
    //FIXME: implement
    BVGL::bvglClearDepth((GLclampd)m_ClearDepth);
    //glClearStencil((GLint)mClearStencil);
    BVGL::bvglClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

// *********************************
//
void	Renderer::SetClearColor		( const glm::vec4 & col )
{
    m_ClearColor = col;
}

// *********************************
//
void	Renderer::Resize	        ( unsigned int w, unsigned int h )
{
    BVGL::bvglViewport( 0 , 0, w, h );

    m_Camera->SetViewportSize( w, h );

    m_Width = w;
    m_Height = h;
    //FIXME: implement
}

// *********************************
//
bool    Renderer::PreDraw           ()
{
    return true;
}

// *********************************
//
bool    Renderer::DrawRenderable    ( RenderableEntity * ent )
{
    RenderableEntity::RenderableType type = ent->GetType();

    switch ( type )
    {
    case RenderableEntity::RenderableType::RT_TRIANGLE_STRIP:
        DrawTriangleStrips( static_cast< TriangleStrip * >( ent ) );
        //FIXME: FIX-1
        //glDrawArrays(ConstantsMapper::GLConstant(type), 0, static_cast<TriangleStrip*>(ent)->NumVertices() );
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
    static GLuint mode = ConstantsMapper::GLConstant( RenderableEntity::RenderableType::RT_TRIANGLE_STRIP );

    const VertexArraySingleVertexBuffer * vao = static_cast< const RenderableArrayDataArraysSingleVertexBuffer * >( strip->GetRenderableArrayData() )->VAO();

    Enable  ( vao );

    unsigned int firstVertex = 0;
    for( unsigned int i = 0; i < vao->GetNumConnectedComponents(); ++i )
    {
        unsigned int numVertices = vao->GetNumVertices( i );
        BVGL::bvglDrawArrays( mode, firstVertex, numVertices );
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
    assert( ent );
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
    if( eff )
    {
        Enable( eff->GetPass( 0 ), ent ); //FIXME: 1 pass ONLY RIGHT NOW

        DrawRenderable( ent );

        //FIXME: Disable whathever there is to be disabled
        //Disable(eff->GetPass(0));  //FIXME:
        //Disable(vb, vd);
        //if(ib) 
        //    Disable(ib);

        //Disable(vb);
    }

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
int     Renderer::GetWidth            () const
{
    return m_Width;
}

// *********************************
//
int     Renderer::GetHeight           () const
{
    return m_Height;
}

// *********************************
//
void    Renderer::Enable              ( RenderablePass * pass, RenderableEntity * renderable )
{
    auto it = m_PdrShaderMap.find( pass );

    PdrShader * shader = nullptr;

    if( it == m_PdrShaderMap.end() )
    {
        shader = PdrShader::Create( pass->GetPixelShader(), pass->GetVertexShader(), pass->GetGeometryShader() );
        m_PdrShaderMap[ pass ] = shader;
    }
    else
    {
        shader = it->second;
    }

    pass->Update( renderable, m_Camera );

    assert( pass->GetStateInstance() );
    SetStateInstance( *pass->GetStateInstance() );

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
void    Renderer::Update              ( const VertexBuffer * vb )
{
    PdrVertexBuffer * pdrVb = GetPdrVertexBuffer( vb );
    pdrVb->Update( vb );
}

// *********************************
//
void    Renderer::Recreate            ( const VertexBuffer * vb )
{
    PdrVertexBuffer * pdrVb = GetPdrVertexBuffer( vb );
    pdrVb->Recreate( vb );
}

// *********************************
//
bool    Renderer::IsRegistered        ( const Texture2D * texture )
{
    return m_PdrTextures2DMap.find( texture ) != m_PdrTextures2DMap.end();
}

// *********************************
//
void    Renderer::RegisterTexture2D   ( const Texture2D * texture, PdrTexture2D * pdrTexture )
{
    assert( !IsRegistered( texture ) );

    m_PdrTextures2DMap[ texture ] = pdrTexture;
}

// *********************************
//
void    Renderer::Enable              ( const Texture2D * texture, int textureUnit )
{
    PdrTexture2D * pdrTex2D = GetPdrTexture2D( texture );

    if( texture->Changed() )
    {
        pdrTex2D->Update( texture );
        texture->SetChanged( false );
        pdrTex2D->SetUpdated( true );
    }
    else
    {
        pdrTex2D->SetUpdated( false );        
    }

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
void    Renderer::Enable              ( const RenderTarget * rt )
{
    PdrRenderTarget * pdrRt = GetPdrRenderTarget( rt );
    pdrRt->Enable( this );
}

// *********************************
//
void    Renderer::Disable             ( const RenderTarget * rt )
{
    PdrRenderTarget * pdrRt = GetPdrRenderTarget( rt );
    pdrRt->Disable( this );
}

// *********************************
//
void    Renderer::ReadColorTexture    ( unsigned int i, const RenderTarget * rt, Texture2DPtr & outputTex )
{
    PdrRenderTarget * pdrRt = GetPdrRenderTarget( rt );

    if( !m_PdrPBOMemTransferRT )
    {
        m_PdrPBOMemTransferRT = new PdrPBOMemTransfer( DataBuffer::Semantic::S_TEXTURE_STREAMING_READ, rt->ColorTexture( i )->RawFrameSize() );
    }

    assert( m_PdrPBOMemTransferRT->DataSize() == rt->ColorTexture( i )->RawFrameSize() );

    pdrRt->ReadColorTexture( i, this, m_PdrPBOMemTransferRT, outputTex );
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
PdrTexture2D *                  Renderer::GetPdrTexture2D       ( const Texture2D * texture )
{
    auto it = m_PdrTextures2DMap.find( texture );

    PdrTexture2D * pdrTex = nullptr;

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

// *********************************
//
PdrRenderTarget *               Renderer::GetPdrRenderTarget    ( const RenderTarget * rt )
{
    auto it = m_PdrRenderTargetMap.find( rt );

    PdrRenderTarget * pdrRt = nullptr;

    if( it == m_PdrRenderTargetMap.end() )
    {
        pdrRt = new PdrRenderTarget( this, rt );
        m_PdrRenderTargetMap[ rt ] = pdrRt;
    }
    else
    {
        pdrRt = it->second;
    }

    return pdrRt;
}

// *********************************
//
void  Renderer::NaiveReadback       ( char * buf, int w, int h )
{
    assert( w == m_Width );
    assert( h == m_Height );
    assert( buf );

    //FIXME: use renderer machinery and RT machinery to read back texture data
    BVGL::bvglReadPixels( 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, buf );
}

} //bv
