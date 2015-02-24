#include "PdrPBOMemTransfer.h"

//#define DISALLOW_ASYNC_PBO

namespace bv {

// ****************************
//
PdrPBOMemTransfer::PdrPBOMemTransfer    ( DataBuffer::Semantic semantic, unsigned int dataSize )
    : m_index( 0 ) 
    , m_dataSize( dataSize )
    , m_writeLock( false )
    , m_lockedMemoryPtr( nullptr )
{
    assert( (int) DataBuffer::Semantic::S_TOTAL == 7 );
    assert( PBORequired( semantic ) );

    m_pboTarget = PBOTarget( semantic );
    m_pboUsage  = PBOUsage( semantic );
    m_numPBOs   = NumPBOs( semantic );

    // FIXME: for the time being
    m_numPBOs = 1;

    assert( m_numPBOs > 0 );

    glGenBuffers( m_numPBOs, m_pboID );

    for( unsigned int i = 0; i < m_numPBOs; ++i )
    {
        glBindBuffer( m_pboTarget, m_pboID[ i ] );
        glBufferData( m_pboTarget, dataSize, 0, m_pboUsage );
        glBindBuffer( m_pboTarget, 0 );
    }
}

// ****************************
//
PdrPBOMemTransfer::~PdrPBOMemTransfer   ()
{
    glDeleteBuffers( m_numPBOs, m_pboID );
}

// ****************************
//
void *  PdrPBOMemTransfer::LockTexture         ( MemoryLockingType mlt, GLuint textureID, GLuint width, GLuint height, GLuint format, GLuint type )
{
    if( m_numPBOs == 1 )
    {
        return SyncLockTexture( mlt );
    }

    return AsyncLockTexture( mlt, textureID, width, height, format, type );
}

// ****************************
//
void    PdrPBOMemTransfer::UnlockTexture       ( GLuint textureID, GLuint width, GLuint height, GLuint format, GLuint type )
{
    if( m_numPBOs == 1 )
    {
        SyncUnlockTexture( textureID, width, height, format, type );
    }
    else
    {
        AsyncUnlockTexture();
    }
}

// ****************************
//
void *  PdrPBOMemTransfer::LockRenderTarget    ( GLenum readBufferID, GLuint width, GLuint height, GLuint format, GLuint type )
{
    if( m_numPBOs == 1 )
    {
        return SyncLockRenderTarget( readBufferID, width, height, format, type );
    }

    return AsyncLockRenderTarget( readBufferID, width, height, format, type );
}

// ****************************
//
void    PdrPBOMemTransfer::UnlockRenderTarget  ()
{
    if( m_numPBOs == 1 )
    {
        SyncUnlockRenderTarget();
    }
    else
    {
        AsyncUnlockRenderTarget();
    }
}

// ****************************
// FIXME: tutaj tylko zapis bedzie szybi, odczyt w przypadku GL_PIXEL_UNPACK_BUFFER bedzie koszmarnie wolny
void * PdrPBOMemTransfer::SyncLockTexture   ( MemoryLockingType mlt )
{
    assert( m_numPBOs == 1 );

    if( !m_lockedMemoryPtr )
    {
        glBindBuffer( m_pboTarget, m_pboID[ m_index ] );
        m_lockedMemoryPtr = glMapBuffer( m_pboTarget, PBOAccess( mlt ) );
        glBindBuffer( m_pboTarget, 0 );

        m_writeLock = mlt != MemoryLockingType::MLT_READ_ONLY;
    }

    return m_lockedMemoryPtr;
}

// ****************************
// FIXME: only 2D textures so far
void   PdrPBOMemTransfer::SyncUnlockTexture ( GLuint textureID, GLuint width, GLuint height, GLuint format, GLuint type )
{
    assert( m_numPBOs == 1 );

    if( m_lockedMemoryPtr )
    {
        glBindBuffer( m_pboTarget, m_pboID[ m_index ] );
        glUnmapBuffer( m_pboTarget );

        if( m_writeLock )
        {
            GLint prevTex = BindTexture( textureID );

            //READ FROM PBO
            glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, width, height, format, type, 0 );

            glBindTexture( GL_TEXTURE_2D, prevTex );

            m_writeLock = false;
        }

        glBindBuffer( m_pboTarget, 0 );

        m_lockedMemoryPtr = nullptr;
    }
}

// ****************************
// FIXME: only for writing now
void *  PdrPBOMemTransfer::AsyncLockTexture     ( MemoryLockingType mlt, GLuint textureID, GLuint width, GLuint height, GLuint format, GLuint type )
{
    assert( m_numPBOs == 2 );
    assert( mlt == MemoryLockingType::MLT_READ_WRITE || mlt == MemoryLockingType::MLT_WRITE_ONLY );
    assert( m_pboTarget == GL_PIXEL_UNPACK_BUFFER );
    assert( m_pboUsage == GL_STREAM_DRAW );

    if( !m_lockedMemoryPtr )
    {
        m_writeLock = mlt != MemoryLockingType::MLT_READ_ONLY;

        m_index = ( m_index + 1 ) % 2;
        
        if( m_writeLock )
        {
            glBindBuffer( m_pboTarget, m_pboID[ m_index ] );

            GLint prevTex = BindTexture( textureID );

            //READ FROM PBO and write to texture - async, via DMA
            glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, width, height, format, type, 0 );
            glBindTexture( GL_TEXTURE_2D, prevTex );

            glBindBuffer( m_pboTarget, m_pboID[ ( m_index + 1 ) % 2 ] );

            // Note that glMapBufferARB() causes sync issue.
            // If GPU is working with this buffer, glMapBufferARB() will wait(stall)
            // until GPU to finish its job. To avoid waiting (idle), you can call
            // first glBufferDataARB() with NULL pointer before glMapBufferARB().
            // If you do that, the previous data in PBO will be discarded and
            // glMapBufferARB() returns a new allocated pointer immediately
            // even if GPU is still working with the previous data.
            glBufferData( m_pboTarget, m_dataSize, 0, m_pboUsage );

            m_lockedMemoryPtr = glMapBuffer( m_pboTarget, PBOAccess( mlt ) );

            glBindBuffer( m_pboTarget, 0 );
        }
    }

    return m_lockedMemoryPtr;
}

// ****************************
//
void    PdrPBOMemTransfer::AsyncUnlockTexture   ()
{
    assert( m_numPBOs == 2 );

    if( m_lockedMemoryPtr )
    {
        glBindBuffer( m_pboTarget, m_pboID[ ( m_index + 1 ) % 2 ] );
        glUnmapBuffer( m_pboTarget );
        glBindBuffer( m_pboTarget, 0 );

        m_writeLock = false;
        m_lockedMemoryPtr = nullptr;
    }
}

// ****************************
//
void *  PdrPBOMemTransfer::SyncLockRenderTarget    ( GLenum readBufferID, GLuint width, GLuint height, GLuint format, GLuint type )
{
    assert( m_numPBOs == 1 );
    assert( m_pboTarget == GL_PIXEL_PACK_BUFFER );

    if( !m_lockedMemoryPtr )
    {
        glReadBuffer( readBufferID );

        glBindBuffer( m_pboTarget, m_pboID[ m_index ] );
        glReadPixels( 0, 0, width, height, format, type, 0 );
        m_lockedMemoryPtr = glMapBuffer( m_pboTarget, GL_READ_ONLY );

        glBindBuffer( m_pboTarget, 0 );
    }

    return m_lockedMemoryPtr;
}

// ****************************
//
void    PdrPBOMemTransfer::SyncUnlockRenderTarget  ()
{
    assert( m_numPBOs == 1 );
    assert( m_pboTarget == GL_PIXEL_PACK_BUFFER );

    glBindBuffer( m_pboTarget, m_pboID[ m_index ] );
    glUnmapBuffer( m_pboTarget );
    glBindBuffer( m_pboTarget, 0 );

    m_lockedMemoryPtr = nullptr;
}

// ****************************
//
void *  PdrPBOMemTransfer::AsyncLockRenderTarget   ( GLenum readBufferID, GLuint width, GLuint height, GLuint format, GLuint type )
{
    assert( m_numPBOs == 2 );
    assert( m_pboTarget == GL_PIXEL_PACK_BUFFER );
    assert( m_pboUsage == GL_STREAM_READ );

    if( !m_lockedMemoryPtr )
    {
        m_index = ( m_index + 1 ) % 2;

        glReadBuffer( readBufferID );

        glBindBuffer( m_pboTarget, m_pboID[ m_index ] );
        glReadPixels( 0, 0, width, height, format, type, 0 ); //Async DMA read

        glBindBuffer( m_pboTarget, m_pboID[ ( m_index + 1 ) % 2 ] );
        m_lockedMemoryPtr = glMapBuffer( m_pboTarget, GL_READ_ONLY );

        glBindBuffer( m_pboTarget, 0 );
    }

    return m_lockedMemoryPtr;
}

// ****************************
//
void    PdrPBOMemTransfer::AsyncUnlockRenderTarget ()
{
    assert( m_numPBOs == 2 );
    assert( m_pboTarget == GL_PIXEL_PACK_BUFFER );

    glBindBuffer( m_pboTarget, m_pboID[ ( m_index + 1 ) % 2 ] );
    glUnmapBuffer( m_pboTarget );
    glBindBuffer( m_pboTarget, 0 );

    m_lockedMemoryPtr = nullptr;
}

// ****************************
//
void    PdrPBOMemTransfer::Flush        ( GLuint textureUnit )
{
    { textureUnit; }
    //if ( NumPBOs() == 2 )
    //{
    //}
}

// ****************************
//
GLuint  PdrPBOMemTransfer::NumPBOs      () const
{
    return m_numPBOs;
}

// ****************************
//
GLuint  PdrPBOMemTransfer::DataSize     () const
{
    return m_dataSize;
}

// ****************************
//
bool PdrPBOMemTransfer::PBORequired     ( DataBuffer::Semantic semantic )
{
    return semantic != DataBuffer::Semantic::S_STATIC && semantic != DataBuffer::Semantic::S_TEXTURE_STATIC;
}

// ****************************
//
GLint   PdrPBOMemTransfer::BindTexture  ( GLuint textureID )
{
    GLint prevTex = 0;

    glGetIntegerv( GL_TEXTURE_BINDING_2D, &prevTex );
    glBindTexture( GL_TEXTURE_2D, textureID );

    return prevTex;
}

// ****************************
//
GLuint  PdrPBOMemTransfer::NumPBOs      ( DataBuffer::Semantic semantic ) const
{
#ifdef DISALLOW_ASYNC_PBO
    return 1
#endif

    if( semantic == DataBuffer::Semantic::S_DYNAMIC || semantic == DataBuffer::Semantic::S_TEXTURE_DYNAMIC )
    {
        return 1;
    }
    else if ( semantic == DataBuffer::Semantic::S_TEXTURE_STREAMING_READ || semantic == DataBuffer::Semantic::S_TEXTURE_STREAMING_WRITE )
    {
        return 2;
    }

    return 0;
}

// ****************************
//
GLenum  PdrPBOMemTransfer::PBOTarget    ( DataBuffer::Semantic semantic ) const
{
    if( semantic == DataBuffer::Semantic::S_DYNAMIC || semantic == DataBuffer::Semantic::S_TEXTURE_DYNAMIC || semantic == DataBuffer::Semantic::S_TEXTURE_STREAMING_WRITE )
    {
        return GL_PIXEL_UNPACK_BUFFER;
    }
    else if ( semantic == DataBuffer::Semantic::S_TEXTURE_STREAMING_READ )
    {
        return GL_PIXEL_PACK_BUFFER;
    }

    return 0;
}

// ****************************
//
GLenum  PdrPBOMemTransfer::PBOUsage     ( DataBuffer::Semantic semantic ) const
{
    if( semantic == DataBuffer::Semantic::S_DYNAMIC || semantic == DataBuffer::Semantic::S_TEXTURE_DYNAMIC || semantic == DataBuffer::Semantic::S_TEXTURE_STREAMING_WRITE )
    {
        return ConstantsMapper::GLConstant( semantic );
    }
    else if ( semantic == DataBuffer::Semantic::S_TEXTURE_STREAMING_READ )
    {
        return ConstantsMapper::GLConstant( semantic );
    }

    return 0;
}

// ****************************
//
GLenum  PdrPBOMemTransfer::PBOAccess    ( MemoryLockingType mlt ) const
{
    return ConstantsMapper::GLConstant( mlt );
}

} //bv
