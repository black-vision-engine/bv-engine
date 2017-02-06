#pragma once

#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrPBOMemTransfer.h"

namespace bv
{

class Renderer;

class PdrTexture
{
protected:
	//bool            m_prevFrameUpdated;	//?
    //bool            m_curFrameUpdated;	//?

    GLuint          m_textureID;
    GLuint          m_prevTextureID;

    TextureFormat   m_txFormat;

    GLuint          m_format;
    GLuint          m_internalFormat;
    GLuint          m_type;

public:
						PdrTexture		();
	virtual				~PdrTexture		();

    virtual void        Enable          ( Renderer * renderer, int textureUnit );
    virtual void        Disable         ( Renderer * renderer, int textureUnit );

    virtual GLuint      Bind            () = 0;
    virtual void        Unbind          () = 0;

	virtual void		GenerateMipmaps	() = 0;

    //void				SetUpdated      ( bool updated );

    GLuint				GetTextureID    () const;
};

} // bv
