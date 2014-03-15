#pragma once

#include "gl/glew.h"

#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"


namespace bv
{

class Texture2D;
class Renderer;

class PdrTextureAnimatedSequence2D
{
private:

    GLuint          m_textureID;
    GLuint          m_pboID;

    GLuint          m_prevTextureID;

    bool            m_writeLock;
    void *          m_lockedMemoryPtr;

    int             m_width;
    int             m_height;

private:

    explicit        PdrTextureAnimatedSequence2D    ( const Texture2D * texture );

public:

                    ~PdrTextureAnimatedSequence2D   ();

    void            Enable          ( Renderer * renderer, int textureUnit );
    void            Disable         ( Renderer * renderer, int textureUnit );

    void *          Lock            ( MemoryLockingType mlt );
    void            Unlock          ();

    void            Update          ( const Texture2D * texture );

    GLuint          Bind            ();
    void            Unbind          ();

private:

public:

    static PdrTextureAnimatedSequence2D *   Create  ( const Texture2D * texture );

};

} // bv
