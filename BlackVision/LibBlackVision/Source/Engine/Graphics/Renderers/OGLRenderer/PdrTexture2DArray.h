#pragma once

#include "Engine/Graphics/Resources/Texture2DArray.h"

#include "Engine/Graphics/Renderers/OGLRenderer/PdrTexture.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"

namespace bv
{

class Renderer;

class PdrTexture2DArray : public PdrTexture
{
private:
    SizeType		m_width;
    SizeType		m_height;
    
	SizeType		m_layers;

private:

    explicit        PdrTexture2DArray	( const Texture2DArray * texture );

    void            Initialize			( const Texture2DArray * texture );
    void            Deinitialize		();

public:
    virtual         ~PdrTexture2DArray  ();

    virtual GLuint  Bind				();
    virtual void    Unbind				();

    void            Update				( const Texture2DArray * texture );

    static PdrTexture2DArray *   Create	( const Texture2DArray * texture );

};

} // bv
