#include "PdrRenderTarget.h"

#include <cassert>

#include "Engine/Graphics/Resources/Textures/Texture2DCache.h" //FIXME: some other class should be used to create new textures

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Graphics/Renderers/OGLRenderer/PdrTexture2D.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrPBOMemTransfer.h"

//FIXME: remove
//#include "System/HRTimer.h"

namespace bv {

//FIXME: remove
//extern HighResolutionTimer GTimer;


// ****************************
//
PdrRenderTarget::PdrRenderTarget     ( Renderer * renderer, const RenderTarget * rt )
    : m_fboID( 0 )
    , m_depthBufID( 0 )
    , m_width( rt->Width() )
    , m_height( rt->Height() )
    , m_hasDepthBuffer( rt->HasDepthBuffer() )
    , m_numTargets( rt->NumTargets() )
    , m_drawBuffers( rt->NumTargets() )
    , m_textures( rt->NumTargets() )
    , m_textureFormats( rt->NumTargets() )
    , m_readbackBuffers( rt->NumTargets() )
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
    { renderer; } // FIXME: suppress unused
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
    { renderer; } // FIXME: suppress unused
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );

    glViewport( m_prevViewportCoords[ 0 ], m_prevViewportCoords[ 1 ], m_prevViewportCoords[ 2 ], m_prevViewportCoords[ 3 ] );
    glDepthRange( m_prevDepthRange[ 0 ], m_prevDepthRange[ 1 ] );
}

// ****************************
// FIXME: dodac streaming flag do bufora (dla multi PBO)
void            PdrRenderTarget::ReadColorTexture   ( unsigned int i, Renderer * renderer, PdrPBOMemTransfer * pboMem, Texture2DPtr & outputTex )
{
    assert( i < m_numTargets );

    auto & txBufMap = m_readbackBuffers[ i ];

    auto format = m_textureFormats[ i ];
    MemoryChunkPtr buffer = nullptr;

    if( outputTex == nullptr )
    {
        buffer = MemoryChunk::Create( Texture2D::RawFrameSize( format, m_width, m_height ) );

        outputTex = Texture2DCache::CreateTexture( format, m_width, m_height, DataBuffer::Semantic::S_TEXTURE_STATIC, buffer );

        txBufMap[ outputTex.get() ] = buffer;
    }

    assert( txBufMap.find( outputTex.get() ) != txBufMap.end() );

    buffer = txBufMap[ outputTex.get() ];

    if( outputTex->GetFormat() != format || outputTex->GetWidth() != m_width || outputTex->GetHeight() != m_height )
    {
        printf( "Reading %d texture from render target with incompatibile texture (%d, %d, %d) -> expected (%d, %d, %d) - ASSERT\n"
                , i
                , outputTex->GetFormat()
                , outputTex->GetWidth()
                , outputTex->GetHeight()
                , format
                , m_width
                , m_height );

        assert( false ); //FIXME: disallowed in current implementation
#if 0        
        assert( buffer->Size() != Texture2D::RawFrameSize( format, m_width, m_height ) ); //FIXME: not safe - chances are that multiple formats may have exactly the same size (in which case mem buffer should be simply reused)
        buffer->Allocate( Texture2D::RawFrameSize( format, m_width, m_height ) );

        auto tx = Texture2DCache::CreateTexture( format, m_width, m_height, DataBuffer::Semantic::S_TEXTURE_STATIC, buffer );
        outputTex = tx;
#endif
    }

    //double readStart = GTimer.CurElapsed();
    Enable( renderer );

    auto fmt    = ConstantsMapper::GLConstantTextureFormat( format );
    auto type   = ConstantsMapper::GLConstantTextureType( format );

    void * data = pboMem->LockRenderTarget( m_drawBuffers[ i ], m_width, m_height, fmt, type );
    memcpy( buffer->GetWritable(), data, outputTex->RawFrameSize() );
    pboMem->UnlockRenderTarget();

    Disable( renderer );
    //double readTime = GTimer.CurElapsed() - readStart;
    //printf( "Frame readback took %.4f ms\n", readTime * 1000.f );
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
void            PdrRenderTarget::AddColorAttachments( Renderer * renderer, const RenderTarget * rt )
{
    for( unsigned int i = 0; i < rt->NumTargets(); ++i )
    {
        Texture2DPtr tx = rt->ColorTexture( i );
        assert( !renderer->IsRegistered( tx.get() ) );

        PdrTexture2D * pdrTx = PdrTexture2D::Create( tx.get() );
        renderer->RegisterTexture2D( tx.get(), pdrTx );

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
void            PdrRenderTarget::AddDepthBuffer     ( Renderer * renderer, const RenderTarget * rt )
{
    { renderer; rt; } // FIXME: suppress unused
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
