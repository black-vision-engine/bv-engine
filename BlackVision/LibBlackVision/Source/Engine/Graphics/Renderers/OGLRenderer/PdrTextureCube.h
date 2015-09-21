#pragma once

#include "Engine/Graphics/Resources/Textures/TextureCube.h"

#include "Engine/Graphics/Renderers/OGLRenderer/PdrTexture.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrPBOMemTransfer.h"

namespace bv
{

class Renderer;

class PdrTextureCube : public PdrTexture
{
private:
    std::vector< PdrUploadPBOUPtr > m_pboMem;

    SizeType			m_width;
    SizeType			m_height;

private:

    explicit			PdrTextureCube  ( const TextureCube * texture );

    void				Initialize      ( const TextureCube * texture );
    void				Deinitialize    ();

    void				UpdateTexData   ( const TextureCube * texture );
    void				PBOUploadData	( const TextureCube * texture, UInt32 face, UInt32 lvl );

public:

    virtual				~PdrTextureCube ();

    virtual GLuint		Bind			();
    virtual void		Unbind			();

    void				Update          ( const TextureCube * texture );

    static PdrTextureCube *   Create	( const TextureCube * texture );

};

} // bv
