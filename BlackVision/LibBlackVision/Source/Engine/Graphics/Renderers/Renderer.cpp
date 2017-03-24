#include "stdafx.h"

#include "Renderer.h"

#include <cassert>

#include "BVGL.h"

#include "Engine/Graphics/Resources/Textures/Texture2D.h"
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
#include "Engine/Graphics/SceneGraph/Lines.h"
#include "Engine/Graphics/SceneGraph/Camera.h"

#include "Engine/Graphics/Resources/VertexBuffer.h"
#include "Engine/Graphics/Resources/VertexArray.h"
#include "Engine/Graphics/Resources/RenderableArrayDataArrays.h"

#include "Engine/Graphics/Resources/UniformBuffer.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrUniformBufferObject.h"

#include "Engine/Graphics/SceneGraph/Scene.h"
#include "Engine/Graphics/Effects/NodeEffect/NodeEffect.h"

#include "Tools/HRTimer.h"

#include "UseLoggerLibBlackVision.h"

#include <set>

//FIXME: add disable methods so that current state can be cleared after a frame is rendered



#include "Memory/MemoryLeaks.h"



namespace bv {

extern HighResolutionTimer GTimer;


const UInt32         Renderer::LIGHTS_UBO_BINDING_IDX   = 0;
const UInt32         Renderer::CAMERA_UBO_BINDING_IDX   = 1;


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

    m_defaultStateInstance.SetState( std::make_shared<AlphaState>() );
    m_defaultStateInstance.SetState( std::make_shared<CullState>() );
    m_defaultStateInstance.SetState( std::make_shared<DepthState>() );
    m_defaultStateInstance.SetState( std::make_shared<FillState>() );
    m_defaultStateInstance.SetState( std::make_shared<OffsetState>() );
    m_defaultStateInstance.SetState( std::make_shared<StencilState>() );

    m_currentStateInstance = m_defaultStateInstance;

    m_PdrPBOMemTransferRT = nullptr;
    m_PdrPBOMemTransferSyncRT = nullptr;

    m_lightsUBO = nullptr;
    m_cameraUBO = nullptr;
}

// *********************************
//
void	Renderer::SetCamera         ( Camera * cam )
{
    m_Camera = cam;
}

// *********************************
//
Camera * Renderer::GetCamera        ()
{
    return m_Camera;
}

// *********************************
// FIXME: nrl - refactor this one - its logic should be placed outside this class
void Renderer::EnableScene          ( Scene * scene )
{
    auto buffer = scene->GetLightsBuffer();
    if( buffer )
    {
        if( !m_lightsUBO )
        {
            m_lightsUBO = new PdrUniformBufferObject( this, buffer, LIGHTS_UBO_BINDING_IDX );
        }

        m_lightsUBO->Update( buffer );
    }

    auto cameraBuffer = scene->GetCameraBuffer();
    if( cameraBuffer )
    {
        if( !m_cameraUBO )
        {
            m_cameraUBO = new PdrUniformBufferObject( this, cameraBuffer, CAMERA_UBO_BINDING_IDX );
        }

        m_cameraUBO->Update( cameraBuffer );
    }
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
}

// *********************************
//
void	Renderer::Terminate             ()
{
    delete m_PdrPBOMemTransferRT;
    delete m_PdrPBOMemTransferSyncRT;
    delete m_lightsUBO;
    delete m_cameraUBO;

    FreePdrResources();

    BVGL::PrintCompleteSummary( "\n\n\nAFTER RENDERER CLEANUP" );
}

// *********************************
//
void    Renderer::FreePdrResources   ()
{
    DeleteShadersPDR();
    DeleteVertexBufersPDR();
    DeleteIndexBuffersPDR();
    DeleteVertexDescriptorsPDR();
    DeleteVertexArrayObjectsPDR();
    DeleteTextures2DPDR();
    DeleteVertexArrayObjectsSVBPDR();
    DeleteRenderTargetsPDR();
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

    //m_Camera->SetViewportSize( w, h );

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
    case RenderableEntity::RenderableType::RT_TRIANGLES:
        DrawTriangles( static_cast< Triangles * >( ent ) );
        break;
    case RenderableEntity::RenderableType::RT_LINES:
        DrawLines( static_cast< Lines * >( ent ) );
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

    // FIXME: this line suxx as hell - only RenderableArrayDataArraysSingleVertexBuffer is supported
    const VertexArraySingleVertexBuffer * vao = static_cast< const RenderableArrayDataArraysSingleVertexBuffer * >( strip->GetRenderableArrayData() )->VAO();

    Enable  ( vao );

    unsigned int firstVertex = 0;
	auto ccNum = vao->GetNumConnectedComponents();
    for( unsigned int i = 0; i < ccNum; ++i )
    {
        unsigned int numVertices = vao->GetNumVertices( i );
        BVGL::bvglDrawArrays( mode, firstVertex, numVertices );
        firstVertex += numVertices;
    }

    Disable ( vao );

    return true;
}

// *********************************
//
bool     Renderer::DrawTriangles            ( Triangles * triangles )
{
    static GLuint mode = ConstantsMapper::GLConstant( RenderableEntity::RenderableType::RT_TRIANGLES );

    // FIXME: this line suxx as hell - only RenderableArrayDataArraysSingleVertexBuffer is supported
    const VertexArraySingleVertexBuffer * vao = static_cast< const RenderableArrayDataArraysSingleVertexBuffer * >( triangles->GetRenderableArrayData() )->VAO();

    Enable  ( vao );

    unsigned int firstVertex = 0;
    auto ccNum = vao->GetNumConnectedComponents();
    for( unsigned int i = 0; i < ccNum; ++i )
    {
        unsigned int numVertices = vao->GetNumVertices( i );
        BVGL::bvglDrawArrays( mode, firstVertex, numVertices );
        firstVertex += numVertices;
    }

    Disable ( vao );

    return true;
}

// *********************************
//
bool     Renderer::DrawLines      ( Lines * lines )
{
    static GLuint mode = ConstantsMapper::GLConstant( RenderableEntity::RenderableType::RT_LINES );

    // FIXME: this line suxx as hell - only RenderableArrayDataArraysSingleVertexBuffer is supported
    const VertexArraySingleVertexBuffer * vao = static_cast< const RenderableArrayDataArraysSingleVertexBuffer * >( lines->GetRenderableArrayData() )->VAO();

    Enable  ( vao );

    BVGL::bvglLineWidth( lines->GetWidth() );

    unsigned int firstVertex = 0;
    auto ccNum = vao->GetNumConnectedComponents();
    for( unsigned int i = 0; i < ccNum; ++i )
    {
//assert( !BVGL::bvglGetError() );
//
        unsigned int numVertices = vao->GetNumVertices( i );
        BVGL::bvglDrawArrays( mode, firstVertex, numVertices );

//auto error = BVGL::bvglGetError();
//if( error )
//{
//    LOG_MESSAGE( SeverityLevel::error ) << "gl error " << BVGL::bvgluErrorString( error );
//    assert( false );
//}

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
        for( unsigned int pass = 0; pass < eff->NumPasses(); ++pass )
        {
            Enable( eff->GetPass( pass ), ent ); //FIXME: 1 pass ONLY RIGHT NOW

            //enable light ubo

            DrawRenderable( ent );

            //FIXME: Disable whathever there is to be disabled
            //Disable(eff->GetPass(0));  //FIXME:
            //Disable(vb, vd);
            //if(ib) 
            //    Disable(ib);

            //Disable(vb);
        }
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
        
        shader->BindUniformBuffer( m_lightsUBO );
        shader->BindUniformBuffer( m_cameraUBO );
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
    m_TextureUpdateIDMap[ texture ] = 0;
}

// *********************************
//
int     Renderer::GetSamplerTexUnit ( int samplerLoc )
{
    auto it = m_enabledSamplerTexUnitsMap.find( samplerLoc );

    if( it == m_enabledSamplerTexUnitsMap.end() )
    {
        m_enabledSamplerTexUnitsMap[ samplerLoc ] = -1;
    }

    return m_enabledSamplerTexUnitsMap[ samplerLoc ];
}

// *********************************
//
void    Renderer::SetSamplerTexUnit ( int samplerLoc, int textureUnit )
{
    // FIXME: this assert does not hold - e.g. when textureUnit is cached for the first time (GetSamplerTexUnit(...) is not called beforehand)
    // assert( m_enabledSamplerTexUnitsMap.find( samplerLoc ) != m_enabledSamplerTexUnitsMap.end() );

    m_enabledSamplerTexUnitsMap[ samplerLoc ] = textureUnit;
}

// *********************************
//
void    Renderer::Enable              ( const Texture2D * texture, int textureUnit )
{
    PdrTexture2D * pdrTex2D = GetPdrTexture2D( texture );
    bool updated = false;

    if( texture->GetUpdateID() > m_TextureUpdateIDMap[ texture ] )
    {
        pdrTex2D->Update( texture );
        m_TextureUpdateIDMap[ texture ] = texture->GetUpdateID();

        updated = true;
    }

    // Note: If texture was updated it must be rebound to shader even if it was eneabled.
    // Update process can delete texture and create new one, which caused problems in the past.
    if( updated || !IsEnabled( texture, textureUnit ) )
    {
        pdrTex2D->Enable( this, textureUnit );

        SetEnabled( texture, textureUnit );
    }
}

// *********************************
//
void    Renderer::Disable             ( const Texture2D * texture, int textureUnit )
{
    PdrTexture2D * pdrTex2D = GetPdrTexture2D( texture );
    pdrTex2D->Disable( this, textureUnit );

    SetDisabled( texture, textureUnit );
}

// *********************************
//
bool    Renderer::IsEnabled           ( const Texture2D * texture, int textureUnit )
{
    auto it = m_enabledTexturesMap.find( textureUnit );

    if ( it == m_enabledTexturesMap.end() )
    {
        m_enabledTexturesMap[ textureUnit ] = nullptr;
    }

    return m_enabledTexturesMap[ textureUnit ] == texture;
}

// *********************************
//
void    Renderer::SetEnabled          ( const Texture2D * texture, int textureUnit )
{
    assert( m_enabledTexturesMap.find( textureUnit ) != m_enabledTexturesMap.end() );

    m_enabledTexturesMap[ textureUnit ] = texture;
}

// *********************************
//
void    Renderer::SetDisabled         ( const Texture2D * texture, int textureUnit )
{
    assert( m_enabledTexturesMap.find( textureUnit ) != m_enabledTexturesMap.end() );

    { texture; }

    m_enabledTexturesMap[ textureUnit ] = nullptr;
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
        m_PdrPBOMemTransferRT = new PdrDownloadPBO( DataBuffer::Semantic::S_TEXTURE_STREAMING_READ, rt->ColorTexture( i )->RawFrameSize(), false );
    }

    //assert( m_PdrPBOMemTransferRT->DataSize() == rt->ColorTexture( i )->RawFrameSize() );

    pdrRt->ReadColorTexture( i, this, m_PdrPBOMemTransferRT, outputTex );
}

// *********************************
//
void	Renderer::GenerateMipmaps( const RenderTarget * rt, UInt32 i )
{
	auto tex = rt->ColorTexture( i );
	if( tex && tex->HasMipmaps() )
	{
		GetPdrTexture2D( tex.get() )->GenerateMipmaps();
	}
}

// *********************************
//
void	Renderer::GenerateMipmaps( const RenderTarget * rt )
{
	for( UInt32 i = 0; i < rt->NumTargets(); ++i )
	{
		if( rt->HasMipmaps( i ) )
		{
			GenerateMipmaps( rt, i );
		}
	}
}


// ***********************
//
void    Renderer::ReadColorTexturSync ( unsigned int i, const RenderTarget * rt, Texture2DPtr & outputTex )
{
    PdrRenderTarget * pdrRt = GetPdrRenderTarget( rt );

    if( !m_PdrPBOMemTransferSyncRT )
    {
        m_PdrPBOMemTransferSyncRT = new PdrDownloadPBO( DataBuffer::Semantic::S_TEXTURE_STREAMING_READ, rt->ColorTexture( i )->RawFrameSize(), true );
    }

    //assert( m_PdrPBOMemTransferRT->DataSize() == rt->ColorTexture( i )->RawFrameSize() );

    pdrRt->ReadColorTexture( i, this, m_PdrPBOMemTransferSyncRT, outputTex );
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
        m_TextureUpdateIDMap[ texture ] = 0;
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

// *********************************
//
void    Renderer::DeleteShadersPDR                ()
{
    DeletePDRResource( m_PdrShaderMap );
}

// *********************************
//
void    Renderer::DeleteVertexBufersPDR           ()
{
    DeletePDRResource( m_PdrVertexBufferMap );
}

// *********************************
//
void    Renderer::DeleteIndexBuffersPDR           ()
{
    DeletePDRResource( m_PdrIndexBufferMap );
}

// *********************************
//
void    Renderer::DeleteVertexDescriptorsPDR      ()
{
    DeletePDRResource( m_PdrVertexDescriptorMap );
}

// *********************************
//
void    Renderer::DeleteVertexArrayObjectsPDR     ()
{
    DeletePDRResource( m_PdrVertexArrayObjectMap );
}

// *********************************
//
void    Renderer::DeleteTextures2DPDR             ()
{
    DeletePDRResource( m_PdrTextures2DMap );
}

// *********************************
//
void    Renderer::DeleteVertexArrayObjectsSVBPDR  ()
{
    DeletePDRResource( m_PdrVertexArrayObjectSingleVBMap );
}

// *********************************
//
void    Renderer::DeleteRenderTargetsPDR          ()
{
    DeletePDRResource( m_PdrRenderTargetMap );
}

// *********************************
//
void    Renderer::DeletePDR                                 ( const RenderablePass * pass )
{
    DeleteSinglePDR( m_PdrShaderMap, pass );
}

// *********************************
//
void    Renderer::DeletePDR                                 ( const VertexBuffer * vb )
{
    DeleteSinglePDR( m_PdrVertexBufferMap, vb );
}

// *********************************
//
void    Renderer::DeletePDR                                 ( const IndexBuffer * ib )
{
    DeleteSinglePDR( m_PdrIndexBufferMap, ib );
}

// *********************************
//
void    Renderer::DeletePDR                                 ( const VertexDescriptor * vd )
{
    DeleteSinglePDR( m_PdrVertexDescriptorMap, vd );
}

// *********************************
//
void    Renderer::DeletePDR                                 ( const VertexArray * vao )
{
    DeleteSinglePDR( m_PdrVertexArrayObjectMap, vao );
}

// *********************************
//
void    Renderer::DeletePDR                                 ( const VertexArraySingleVertexBuffer * vao )
{
    DeleteSinglePDR( m_PdrVertexArrayObjectSingleVBMap, vao );
}

// *********************************
//
void    Renderer::DeletePDR                                 ( const Texture2D * texture )
{
    DeleteSinglePDR( m_PdrTextures2DMap, texture );
    m_TextureUpdateIDMap.erase( texture );
}

// *********************************
//
void    Renderer::DeletePDR                                 ( const RenderTarget * rt )
{
    DeleteSinglePDR( m_PdrRenderTargetMap, rt );
}

// *********************************
//
void    Renderer::FreeAllPDResources                        ( RenderableEntity * renderable )
{
    assert( renderable->GetType() == RenderableEntity::RenderableType::RT_TRIANGLE_STRIP || 
            renderable->GetType() == RenderableEntity::RenderableType::RT_TRIANGLES ||
            renderable->GetType() == RenderableEntity::RenderableType::RT_LINES );

    // FIXME: this suxx as we implictly assume that RenderableArrayDataSingleVertexBuffer is in fact of type RenderableArrayDataArraysSingleVertexBuffer
    auto radasvb = static_cast< RenderableArrayDataArraysSingleVertexBuffer * >( renderable->GetRenderableArrayData() );
    
    FreeRADASVBPDR  ( radasvb );
    FreeEffectPDR   ( renderable->GetRenderableEffect().get() );

    m_performance.FreeAllPDResources( renderable );
}

// *********************************
//
void    Renderer::FreeRADASVBPDR                  ( RenderableArrayDataArraysSingleVertexBuffer * radasvb )
{
    if( radasvb)
    {
        auto vao = radasvb->VAO();

        auto vb = vao->GetVertexBuffer();
        auto vd = vao->GetVertexDescriptor();

        if( vb )
            DeletePDR( vb );

        if( vd )
            DeletePDR( vd );

        DeletePDR( vao );
    }
}

// *********************************
//
void    Renderer::FreeEffectPDR                   ( RenderableEffect * effect )
{
    if( effect )
    {
        for( unsigned int i = 0; i < effect->NumPasses(); ++i )
        {
            auto pass = effect->GetPass( i );

            auto ps = pass->GetPixelShader();
            auto vs = pass->GetVertexShader();
            auto gs = pass->GetGeometryShader();

            if( ps )
                FreeShaderPDR( ps );

            if( vs )
                FreeShaderPDR( vs );

            if( gs )
                FreeShaderPDR( gs );

            DeletePDR( pass );
        }
    }
}

// *********************************
//
void    Renderer::FreeShaderPDR                   ( Shader * shader )
{
    auto params = shader->GetParameters();

    for( unsigned int i = 0; i < params->NumTextures(); ++i )
    {
        auto tx = params->GetTexture( i );

        // TODO: implement some smart machinery to track texture resources
        // FIXME: right now it any other node uses the same texture it would have to be reloaded
        DeletePDR( tx.get() );
    }
}

// *********************************
//
template < typename MapType >
void    Renderer::DeletePDRResource ( MapType & resMap )
{
    for( auto e : resMap )
    {
        auto res = e.second;

        delete res;
    }
    
    resMap.clear();
}

// *********************************
//
template< typename MapType >
void    Renderer::DeleteSinglePDR   ( MapType & resMap, typename MapType::key_type & key )
{
    auto it = resMap.find( key );

    if( it != resMap.end() )
    {
        auto res = it->second;

        delete res;

        resMap.erase( it );
    }
}

//// *********************************
////
//void    Renderer::PassCCNumUniform  ( int i, SizeType num )
//{
//    GLint id;
//    BVGL::bvglGetIntegerv( GL_CURRENT_PROGRAM, &id );
//
//    auto loc = BVGL::bvglGetUniformLocation( id, "cc_num" );
//
//    if( loc >= 0 )
//    {
//        BVGL::bvglUniform1i( loc, i );
//    }
//
//    loc = BVGL::bvglGetUniformLocation( id, "cc_num_total" );
//    
//    if( loc >= 0 )
//    {
//        BVGL::bvglUniform1i( loc, ( bv::GLint )num );
//    }
//
//}

// *********************************
//
void    Renderer::FreeNodeEffectPDR ( const NodeEffect * nodeEffect )
{
    { nodeEffect; }
    // FIXME: nrl update
    //std::set< const RenderablePass * > passes;
    //nodeEffect->GetRenderPasses( &passes );

    //for( auto p : passes )
    //{
    //    m_PdrShaderMap.erase( p );
    //}
}

} //bv
