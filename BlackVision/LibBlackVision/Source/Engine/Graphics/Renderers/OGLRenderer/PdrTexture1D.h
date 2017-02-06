#pragma once

#include "Engine/Graphics/Resources/Textures/Texture1D.h"

#include "Engine/Graphics/Renderers/OGLRenderer/PdrTexture.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrPBOMemTransfer.h"

namespace bv
{

class Renderer;

class PdrTexture1D : public PdrTexture
{
private:
    std::vector< PdrUploadPBOUPtr > m_pboMem;

    SizeType		m_width;

private:

    explicit        PdrTexture1D    ( const Texture1D * texture );

    void            Initialize      ( const Texture1D * texture );
    void            Deinitialize    ();

	void            UpdateTexData   ( const Texture1D * texture );
    void            PBOUploadData	( const Texture1D * texture, UInt32 lvl );

public:

    virtual         ~PdrTexture1D   ();

	virtual GLuint  Bind            ();
    virtual void    Unbind          ();

	virtual void	GenerateMipmaps	() override {};

    void            Update          ( const Texture1D * texture );

    static PdrTexture1D *   Create  ( const Texture1D * texture );

};

} // bv
