#pragma once

#include "Engine/Graphics/Resources/Textures/TextureCubeArray.h"

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

    explicit			PdrTextureCubeArray ( const TextureCubeArray * textureArray );

    void				Initialize			( const TextureCubeArray * textureArray );
    void				Deinitialize		();

public:

    virtual             ~PdrTextureCubeArray();
	
    virtual GLuint      Bind				();
    virtual void		Unbind				();

    void				Update				( const TextureCubeArray * textureArray );

    static PdrTextureCubeArray *   Create  ( const TextureCubeArray * textureArray );
};

} // bv
