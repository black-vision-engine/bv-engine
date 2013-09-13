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

public:

    void            Enable          ( Renderer * renderer );
    void            Disable         ( Renderer * renderer );

    void            Bind            ();
    void            Unbind          ();


    static PdrTexture2D*   Create(const Texture2D* texture);

private:

    explicit        PdrTexture2D    (const Texture2D* texture);

};

} // bv
