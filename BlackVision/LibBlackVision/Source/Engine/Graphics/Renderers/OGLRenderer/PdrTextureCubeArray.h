#pragma once

#include "Engine/Graphics/Resources/Texture2D.h"

#include "Engine/Graphics/Renderers/OGLRenderer/PdrTexture.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"

namespace bv
{

class Renderer;

class PdrTextureCubeArray : public PdrTexture
{
private:
    SizeType		m_width;
    SizeType		m_height;

	SizeType		m_layers;

private:

    explicit			PdrTextureCubeArray ( const Texture2D * texture );

    void				Initialize			( const Texture2D * texture );
    void				Deinitialize		();

public:

    virtual             ~PdrTextureCubeArray();
	
    virtual GLuint      Bind				();
    virtual void		Unbind				();

    void				Update				( const Texture2D * texture );

    static PdrTextureCubeArray *   Create  ( const Texture2D * texture );
};

} // bv
