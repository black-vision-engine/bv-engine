#pragma once

#include "Engine/Graphics/Resources/Texture2D.h"

#include "Engine/Graphics/Renderers/OGLRenderer/PdrTexture.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"

namespace bv
{

class Renderer;

class PdrTextureCube : public PdrTexture
{
private:
    SizeType			m_width;
    SizeType			m_height;

private:

    explicit			PdrTextureCube  ( const Texture2D * texture );

    void				Initialize      ( const Texture2D * texture );
    void				Deinitialize    ();

public:

    virtual				~PdrTextureCube ();

    virtual GLuint		Bind			();
    virtual void		Unbind			();

    void				Update          ( const Texture2D * texture );

    static PdrTextureCube *   Create	( const Texture2D * texture );

};

} // bv
