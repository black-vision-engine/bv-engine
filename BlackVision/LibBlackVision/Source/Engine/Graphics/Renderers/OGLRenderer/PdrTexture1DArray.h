#pragma once

#include "Engine/Graphics/Resources/Textures/Texture1DArray.h"

#include "Engine/Graphics/Renderers/OGLRenderer/PdrTexture.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrPBOMemTransfer.h"

namespace bv
{

class Renderer;

class PdrTexture1DArray : public PdrTexture
{
private:
    std::vector< PdrUploadPBOUPtr > m_pboMem;

    SizeType		m_width;

	SizeType		m_layers;

private:

    explicit        PdrTexture1DArray	( const Texture1DArray * textureArray );

    void            Initialize			( const Texture1DArray * textureArray );
    void            Deinitialize		();

	void            UpdateTexData		( const Texture1DArray * textureArray );
    void            PBOUploadData		( const Texture1DArray * textureArray, UInt32 layer, UInt32 lvl );

public:
    virtual         ~PdrTexture1DArray  ();

    virtual GLuint  Bind				();
    virtual void    Unbind				();

    void            Update				( const Texture1DArray * textureArray );

    static PdrTexture1DArray *   Create	( const Texture1DArray * textureArray );

};

} // bv
