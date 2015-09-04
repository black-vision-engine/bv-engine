#pragma once

#include "Engine/Graphics/Resources/Textures/Texture1D.h"

#include "Engine/Graphics/Renderers/OGLRenderer/PdrTexture.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"

namespace bv
{

class Renderer;

class PdrTexture1D : public PdrTexture
{
private:
    SizeType		m_width;

private:

    explicit        PdrTexture1D    ( const Texture1D * texture );

    void            Initialize      ( const Texture1D * texture );
    void            Deinitialize    ();

public:

    virtual         ~PdrTexture1D   ();

	virtual GLuint  Bind            ();
    virtual void    Unbind          ();

    void            Update          ( const Texture1D * texture );

    static PdrTexture1D *   Create  ( const Texture1D * texture );

};

} // bv
