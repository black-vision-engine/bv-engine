#pragma once

#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"
#include "gl/glew.h"


namespace bv
{

class Texture2D;
class Renderer;

class PdrTexture2D
{
private:

    GLuint          m_textureID;
    GLuint          m_prevTextureID;

    GLuint          m_pboID;

    bool            m_writeLock;
    void *          m_lockedMemoryPtr;

    unsigned int    m_width;
    unsigned int    m_height;

private:

    explicit        PdrTexture2D    ( const Texture2D * texture );

    void            Initialize      ( const Texture2D * texture );
    void            Deinitialize    ();

    void            UpdateTexData   ( const Texture2D * texture );

public:

                    ~PdrTexture2D   ();

    void            Enable          ( Renderer * renderer, int textureUnit );
    void            Disable         ( Renderer * renderer, int textureUnit );

    void *          Lock            ( MemoryLockingType mlt );
    void            Unlock          ();

    void            Update          ( const Texture2D * texture );

    GLuint          Bind            ();
    void            Unbind          ();


    static PdrTexture2D *   Create  ( const Texture2D * texture );

};

} // bv
