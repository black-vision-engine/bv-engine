#pragma once

#include "Engine/Graphics/Resources/Texture2D.h"

#include "Engine/Graphics/Renderers/OGLRenderer/PdrTexture.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrPBOMemTransfer.h"

namespace bv
{

class Renderer;

class PdrTexture2D : public PdrTexture
{
private:

    std::vector< PdrUploadPBO * > m_pboMem;

    SizeType		m_width;
    SizeType		m_height;

private:

    explicit        PdrTexture2D    ( const Texture2D * texture );

    void            Initialize      ( const Texture2D * texture );
    void            Deinitialize    ();

    void            UpdateTexData   ( const Texture2D * texture );
    void            PBOUploadData	( const Texture2D * texture, UInt32 lvl );

public:

                    ~PdrTexture2D   ();

    void            Update          ( const Texture2D * texture );

    virtual GLuint  Bind            () override;
    virtual void    Unbind          () override;

    static PdrTexture2D *   Create  ( const Texture2D * texture );

};

} // bv
