#pragma once

#include "Engine/Graphics/Resources/Texture2D.h"

#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrPBOMemTransfer.h"

namespace bv
{

class Renderer;

class PdrTexture2D
{
private:

    PdrPBOMemTransfer * m_pboMem;
    bool                m_prevFrameUpdated;
    bool                m_curFrameUpdated;

    GLuint          m_textureID;
    GLuint          m_prevTextureID;

    unsigned int    m_width;
    unsigned int    m_height;

    TextureFormat   m_txFormat;

    GLuint          m_format;
    GLuint          m_internalFormat;
    GLuint          m_type;

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
    void            SetUpdated      ( bool updated );

    GLuint          Bind            ();
    void            Unbind          ();

    GLuint          GetTextureID    () const;

    static PdrTexture2D *   Create  ( const Texture2D * texture );

};

} // bv
