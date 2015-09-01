#pragma once

#include "Engine/Graphics/Resources/Texture2D.h"

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

    explicit        PdrTexture1DArray	( const Texture2D * texture );

    void            Initialize			( const Texture2D * texture );
    void            Deinitialize		();

public:
    virtual         ~PdrTexture1DArray  ();

    virtual GLuint  Bind				();
    virtual void    Unbind				();

    void            Update				( const Texture2D * texture );

    static PdrTexture1DArray *   Create	( const Texture2D * texture );

};

} // bv
