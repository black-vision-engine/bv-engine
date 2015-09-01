#pragma once

#include "Engine/Graphics/Resources/Texture2D.h"

#include "Engine/Graphics/Renderers/OGLRenderer/PdrTexture.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"

namespace bv
{

class Renderer;

class PdrTexture3D : public PdrTexture
{
private:
    SizeType		m_width;
    SizeType		m_height;
    SizeType		m_depth;

private:

    explicit        PdrTexture3D    ( const Texture2D * texture );

    void            Initialize      ( const Texture2D * texture );
    void            Deinitialize    ();

public:

    virtual         ~PdrTexture3D   ();

    virtual GLuint  Bind			();
    virtual void    Unbind			();

    void            Update          ( const Texture2D * texture );

    static PdrTexture3D *   Create  ( const Texture2D * texture );

};

} // bv
