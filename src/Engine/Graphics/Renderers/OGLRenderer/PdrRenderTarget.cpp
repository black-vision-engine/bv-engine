#include "PdrRenderTarget.h"

#include <cassert>

#include "Engine/Graphics/Resources/Texture2DImpl.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrTexture2D.h"


namespace bv {

// ****************************
//
PdrRenderTarget::PdrRenderTarget     ( Renderer * renderer, RenderTarget * rt )
    : m_fboID( 0 )
    , m_depthBufID( 0 )
    , m_width( rt->Width() )
    , m_height( rt->Height() )
    , m_hasDepthBuffer( rt->HasDepthBuffer() )
    , m_numTargets( rt->NumTargets() )
    , m_drawBuffers( rt->NumTargets() )
    , m_textures( rt->NumTargets() )
    , m_textureFormats( rt->NumTargets() )
{
    assert( m_numTargets > 0 );

    for( unsigned int i = 0; i < m_numTargets; ++i )
    {
        auto tx = rt->ColorTexture( i );
        m_textureFormats.push_back( tx->GetFormat() );
    }

    for( unsigned int i = 0 ; i < 4; ++i )
    {
        m_prevViewportCoords[ i ] = 0;
    }

    m_prevDepthRange[ 0 ] = 0.0;
    m_prevDepthRange[ 1 ] = 0.0;

    glGenFramebuffers( 1, &m_fboID );
    glBindFramebuffer( GL_FRAMEBUFFER, m_fboID );

    GLuint prevTexture = GetPrevTexture();

    AddColorAttachments( renderer, rt );

    if( rt->HasDepthBuffer() )
    {
        AddDepthBuffer( renderer, rt );
    }

    glBindTexture( GL_TEXTURE_2D, prevTexture );

    assert( FramebuferStatusOK() );

    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

// ****************************
//
PdrRenderTarget::~PdrRenderTarget    ()
{
    if( m_fboID != 0 )
    {
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
        glDeleteFramebuffers( 1, &m_fboID );
    }

    if( m_depthBufID != 0 )
        glDeleteRenderbuffers( 1, &m_depthBufID );
}

// ****************************
//
void            PdrRenderTarget::Enable             ( Renderer * renderer )
{
    glBindFramebuffer( GL_FRAMEBUFFER, m_fboID );
    glDrawBuffers( m_numTargets, &m_drawBuffers[ 0 ] );

    glGetIntegerv( GL_VIEWPORT, m_prevViewportCoords );
    glGetDoublev( GL_DEPTH_RANGE, m_prevDepthRange );

    glViewport( 0, 0, m_width, m_height );
    glDepthRange( 0.0, 1.0 );
}

// ****************************
//
void            PdrRenderTarget::Disable            ( Renderer * renderer )
{
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );

    glViewport( m_prevViewportCoords[ 0 ], m_prevViewportCoords[ 1 ], m_prevViewportCoords[ 2 ], m_prevViewportCoords[ 3 ] );
    glDepthRange( m_prevDepthRange[ 0 ], m_prevDepthRange[ 1 ] );
}

// ****************************
//
void            PdrRenderTarget::ReadColorTexture   ( unsigned int i, Renderer * renderer, Texture2D*& outputTex )
{
    assert( i < m_numTargets );

    Enable( renderer );

    auto format = m_textureFormats[ i ];

    if( outputTex == nullptr )
    {
        auto tx = new Texture2DImpl( format, m_width, m_height ); 
        tx->AllocateMemory();
        outputTex = tx;
    }

    if( outputTex->GetFormat() != format || outputTex->GetWidth() != m_width || outputTex->GetHeight() != m_height )
    {
        printf( "Reading %d texture from render target with incompatibile texture (%d, %d, %d) -> expected (%d, %d, %d)\n"
                , i
                , outputTex->GetFormat()
                , outputTex->GetWidth()
                , outputTex->GetHeight()
                , format
                , m_width
                , m_height );

        delete outputTex;
    
        auto tx = new Texture2DImpl( format, m_width, m_height ); 
        tx->AllocateMemory();
        outputTex = tx;
    }

    glReadBuffer( m_drawBuffers[ i ] );
    glReadPixels( 0, 0, m_width, m_height, ConstantsMapper::GLConstantTextureFormat( format ), ConstantsMapper::GLConstantTextureType( format ), outputTex->GetData() );

    Disable( renderer );
}

// ****************************
//
GLuint          PdrRenderTarget::GetPrevTexture     () const
{
    GLint current = 0;
    glGetIntegerv( GL_TEXTURE_BINDING_2D, &current );
    return (GLuint) current;
}

// ****************************
//
void            PdrRenderTarget::AddColorAttachments( Renderer * renderer, RenderTarget * rt )
{
    for( int i = 0; i < rt->NumTargets(); ++i )
    {
        Texture2D * tx = rt->ColorTexture( i );
        assert( !renderer->IsRegistered( tx ) );

        PdrTexture2D * pdrTx = PdrTexture2D::Create( tx );
        renderer->RegisterTexture2D( tx, pdrTx );

        m_textures[ i ] = pdrTx->GetTextureID();
        m_drawBuffers[ i ] = GL_COLOR_ATTACHMENT0 + i;

        glBindTexture( GL_TEXTURE_2D, pdrTx->GetTextureID() );
        
        //FIXME: no mipmaps here
        //FIXME: only NEAREST filters used here - should be just fine, but some implementations use linear filtering for some reasons here
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

        glFramebufferTexture2D( GL_FRAMEBUFFER, m_drawBuffers[ i ], GL_TEXTURE_2D, m_textures[ i ], 0 );
    }
}

// ****************************
//
void            PdrRenderTarget::AddDepthBuffer     ( Renderer * renderer, RenderTarget * rt )
{
    glGenRenderbuffers( 1, &m_depthBufID );
    glBindRenderbuffer( GL_RENDERBUFFER, m_depthBufID );
    glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height );
    glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBufID );
}

// ****************************
// FIXME: 
bool            PdrRenderTarget::FramebuferStatusOK () const
{
    //FIXME: in production renderer all errors should be logged somewhere (or at least printed to console)
    auto status = glCheckFramebufferStatus( GL_FRAMEBUFFER );

    if( status != GL_FRAMEBUFFER_COMPLETE )
    {
        printf( "FBO creation failed with status: %d\n", status );

        return false;
    }

    return true;
}

} //bv
