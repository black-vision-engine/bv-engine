#pragma once

#include "gl/glew.h"

namespace bv
{

class Texture2D;

class PdrTexture2D
{
private:
    GLuint          m_textureID;

public:

    void            Enable          ();
    void            Disable         ();

    void            Bind            ();
    void            Unbind          ();


    static PdrTexture2D*   Create(const Texture2D* texture);

private:

    explicit        PdrTexture2D    (const Texture2D* texture);

};

} // bv
