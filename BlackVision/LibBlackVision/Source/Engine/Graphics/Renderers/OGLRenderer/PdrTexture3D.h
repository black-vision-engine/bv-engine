#pragma once

#include "Engine/Graphics/Resources/Textures/Texture3D.h"

#include "Engine/Graphics/Renderers/OGLRenderer/PdrTexture.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrPBOMemTransfer.h"

namespace bv
{

class Renderer;

class PdrTexture3D : public PdrTexture
{
private:
    std::vector< PdrUploadPBOUPtr > m_pboMem;

    SizeType		m_width;
    SizeType		m_height;
    SizeType		m_depth;

private:

    explicit        PdrTexture3D    ( const Texture3D * texture );

    void            Initialize      ( const Texture3D * texture );
    void            Deinitialize    ();

	void            UpdateTexData   ( const Texture3D * texture );
    void            PBOUploadData	( const Texture3D * texture, UInt32 lvl );

public:

    virtual         ~PdrTexture3D   ();

    virtual GLuint  Bind			();
    virtual void    Unbind			();

	virtual void	GenerateMipmaps	() override {};

    void            Update          ( const Texture3D * texture );

    static PdrTexture3D *   Create  ( const Texture3D * texture );

};

} // bv
