#pragma once

#include <glm/glm.hpp>

#include <gl/glew.h>
#include <gl/wglew.h>
#include <gl/gl.h>

#include "Engine/Graphics/Resources/DataBuffer.h"


namespace bv {

class PdrPBOMemTransfer
{
private:

    GLuint          m_numPBOs;

    GLuint          m_pboID[ 2 ];

    GLenum          m_pboTarget;
    GLenum          m_pboUsage;
    GLenum          m_pboAccess;

public:

                PdrPBOMemTransfer   ( DataBuffer::Semantic semantic );
                ~PdrPBOMemTransfer  ();

    static bool IsStreaming         ( DataBuffer::Semantic semantic );

private:

    GLenum      PBOTarget       ( DataBuffer::Semantic semantic ) const;
    GLenum      PBOUsage        ( DataBuffer::Semantic semantic ) const;
    GLenum      PBOAccess       ( DataBuffer::Semantic semantic ) const;

};

} //bv
