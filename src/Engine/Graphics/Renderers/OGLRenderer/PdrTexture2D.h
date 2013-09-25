#pragma once

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

private:

    explicit        PdrTexture2D    ( const Texture2D * texture );

public:

    void            Enable          ( Renderer * renderer, int textureUnit );
    void            Disable         ( Renderer * renderer, int textureUnit );

    GLuint          Bind            ();
    void            Unbind          ();


    static PdrTexture2D *   Create  ( const Texture2D * texture );


};

} // bv
