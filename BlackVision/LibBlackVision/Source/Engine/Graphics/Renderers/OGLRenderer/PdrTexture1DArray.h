#pragma once

#include "Engine/Graphics/Resources/Textures/Texture1DArray.h"

#include "Engine/Graphics/Renderers/OGLRenderer/PdrTexture.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"

namespace bv
{

class Renderer;

class PdrTexture1DArray : public PdrTexture
{
private:
    SizeType		m_width;

	SizeType		m_layers;

private:

    explicit        PdrTexture1DArray	( const Texture1DArray * textureArray );

    void            Initialize			( const Texture1DArray * textureArray );
    void            Deinitialize		();

public:
    virtual         ~PdrTexture1DArray  ();

    virtual GLuint  Bind				();
    virtual void    Unbind				();

    void            Update				( const Texture1DArray * textureArray );

    static PdrTexture1DArray *   Create	( const Texture1DArray * textureArray );

};

} // bv
