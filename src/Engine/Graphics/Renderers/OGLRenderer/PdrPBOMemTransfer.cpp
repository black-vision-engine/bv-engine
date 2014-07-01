#include "PdrPBOMemTransfer.h"


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
// FIXME: tutaj tylko zapis bedzie szybi, odczyt w przypadku GL_PIXEL_UNPACK_BUFFER bedzie koszmarnie wolny
void * PdrPBOMemTransfer::LockTexture   ( MemoryLockingType mlt )
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
void   PdrPBOMemTransfer::UnlockTexture ( GLuint textureID, GLuint width, GLuint height, GLuint format, GLuint type )
{
    assert( m_numPBOs == 1 );

    if( m_lockedMemoryPtr )
    {
        glBindBuffer( m_pboTarget, m_pboID[ m_index ] );
        glUnmapBuffer( m_pboTarget );

        if( m_writeLock )
        {
            GLint prevTex = 0;

            glGetIntegerv( GL_TEXTURE_BINDING_2D, &prevTex );
            glBindTexture( GL_TEXTURE_2D, textureID );

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
//
bool PdrPBOMemTransfer::PBORequired     ( DataBuffer::Semantic semantic )
{
    return semantic != DataBuffer::Semantic::S_STATIC && semantic != DataBuffer::Semantic::S_TEXTURE_STATIC;
}

// ****************************
//
GLuint  PdrPBOMemTransfer::NumPBOs      ( DataBuffer::Semantic semantic ) const
{
    //FIXME: test case only
    return 1;

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
