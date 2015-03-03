#pragma once

#include "GL/BVGL.h"

#include "Mathematics/glm_inc.h"

#include "Engine/Graphics/Resources/DataBuffer.h"

#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"


//FIXME: implicitly assumes that Lock/Unlock pairs are called in a sane way (lockRT/unlockRT without mixing Texture/RT)
namespace bv {

class PdrPBOMemTransfer
{
private:

    GLuint          m_numPBOs;

    GLuint          m_pboID[ 2 ];

    GLenum          m_pboTarget;
    GLenum          m_pboUsage;

    GLenum          m_dataSize;
    GLuint          m_index;

    bool            m_writeLock;
    void *          m_lockedMemoryPtr;

public:

            PdrPBOMemTransfer   ( DataBuffer::Semantic semantic, unsigned int dataSize );
            ~PdrPBOMemTransfer  ();
    
    void *  LockTexture         ( MemoryLockingType mlt, GLuint textureID, GLuint width, GLuint height, GLuint format, GLuint type );
    void    UnlockTexture       ( GLuint textureID, GLuint width, GLuint height, GLuint format, GLuint type );

    void *  LockRenderTarget    ( GLenum readBufferID, GLuint width, GLuint height, GLuint format, GLuint type );
    void    UnlockRenderTarget  ();

    void *  SyncLockTexture     ( MemoryLockingType mlt );
    void    SyncUnlockTexture   ( GLuint textureID, GLuint width, GLuint height, GLuint format, GLuint type );

    void *  AsyncLockTexture    ( MemoryLockingType mlt, GLuint textureID, GLuint width, GLuint height, GLuint format, GLuint type );
    void    AsyncUnlockTexture  ();

    void *  SyncLockRenderTarget    ( GLenum readBufferID, GLuint width, GLuint height, GLuint format, GLuint type );
    void    SyncUnlockRenderTarget  ();

    void *  AsyncLockRenderTarget   ( GLenum readBufferID, GLuint width, GLuint height, GLuint format, GLuint type );
    void    AsyncUnlockRenderTarget ();

    void    Flush               ( GLuint textureUnit );

    GLuint  NumPBOs             () const;
    GLuint  DataSize            () const;

    static bool PBORequired     ( DataBuffer::Semantic semantic );

private:

    GLint       BindTexture     ( GLuint textureID );

    GLuint      NumPBOs         ( DataBuffer::Semantic semantic ) const;

    GLenum      PBOTarget       ( DataBuffer::Semantic semantic ) const;
    GLenum      PBOUsage        ( DataBuffer::Semantic semantic ) const;
    GLenum      PBOAccess       ( MemoryLockingType mlt ) const;

};

} //bv
