#pragma once

#include <glm/glm.hpp>

#include <gl/glew.h>
#include <gl/wglew.h>
#include <gl/gl.h>

#include "Engine/Graphics/Resources/DataBuffer.h"

#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"


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
    
    void * LockTexture          ( MemoryLockingType mlt );
    void   UnlockTexture        ( GLuint textureID, GLuint width, GLuint height, GLuint format, GLuint type );

    static bool PBORequired     ( DataBuffer::Semantic semantic );

private:

    GLuint      NumPBOs         ( DataBuffer::Semantic semantic ) const;

    GLenum      PBOTarget       ( DataBuffer::Semantic semantic ) const;
    GLenum      PBOUsage        ( DataBuffer::Semantic semantic ) const;
    GLenum      PBOAccess       ( MemoryLockingType mlt ) const;

};

} //bv
