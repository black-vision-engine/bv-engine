#pragma once

#include "Engine/Graphics/Resources/Texture2D.h"

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

    explicit        PdrTexture1D    ( const Texture2D * texture );

    void            Initialize      ( const Texture2D * texture );
    void            Deinitialize    ();

    //void            UpdateTexData   ( const Texture2D * texture );

public:

    virtual         ~PdrTexture1D   ();

	virtual GLuint  Bind            ();
    virtual void    Unbind          ();

    void            Update          ( const Texture2D * texture );

    static PdrTexture1D *   Create  ( const Texture2D * texture );

};

} // bv
