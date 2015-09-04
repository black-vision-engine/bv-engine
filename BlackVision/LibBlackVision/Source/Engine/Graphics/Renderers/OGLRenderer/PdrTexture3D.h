#pragma once

#include "Engine/Graphics/Resources/Textures/Texture3D.h"

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

    explicit        PdrTexture3D    ( const Texture3D * texture );

    void            Initialize      ( const Texture3D * texture );
    void            Deinitialize    ();

public:

    virtual         ~PdrTexture3D   ();

    virtual GLuint  Bind			();
    virtual void    Unbind			();

    void            Update          ( const Texture3D * texture );

    static PdrTexture3D *   Create  ( const Texture3D * texture );

};

} // bv
