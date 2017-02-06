#pragma once

#include "Engine/Graphics/Resources/Textures/Texture2DArray.h"

#include "Engine/Graphics/Renderers/OGLRenderer/PdrTexture.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrPBOMemTransfer.h"

namespace bv
{

class Renderer;

class PdrTexture2DArray : public PdrTexture
{
private:

    std::vector< PdrUploadPBOUPtr > m_pboMem;
    
	SizeType		m_width;
    SizeType		m_height;
    
	SizeType		m_layers;

private:

    explicit        PdrTexture2DArray	( const Texture2DArray * textureArray );

    void            Initialize			( const Texture2DArray * textureArray );
    void            Deinitialize		();

	void            UpdateTexData		( const Texture2DArray * textureArray );
    void            PBOUploadData		( const Texture2DArray * textureArray, UInt32 layer, UInt32 lvl );

public:
    virtual         ~PdrTexture2DArray  ();

    virtual GLuint  Bind				();
    virtual void    Unbind				();

	virtual void	GenerateMipmaps		() override {};

    void            Update				( const Texture2DArray * textureArray );

    static PdrTexture2DArray *   Create	( const Texture2DArray * textureArray );

};

} // bv
