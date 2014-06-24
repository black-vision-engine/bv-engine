#pragma once

#include "gl/glew.h"

#include "Engine/Graphics/Resources/RenderTarget.h"


namespace bv {

class Renderer;

class PdrRenderTarget
{
private:

    //bool          m_hasStencilBuffer; //FIXME: to be implemented

    unsigned int                    m_numTargets;

    unsigned int                    m_width;
    unsigned int                    m_height;

    bool                            m_hasDepthBuffer;

    std::vector< TextureFormat >    m_textureFormats;

    GLuint                          m_fboID;
    GLuint                          m_depthBufID;

    std::vector< GLuint >           m_textures;
    std::vector< GLuint >           m_drawBuffers;

    int                             m_prevViewportCoords[ 4 ];
    double                          m_prevDepthRange[ 2 ];

public:

                    PdrRenderTarget     ( Renderer * renderer, RenderTarget * rt );
                    ~PdrRenderTarget    ();
public:


    void            Enable              ( Renderer * renderer );
    void            Disable             ( Renderer * renderer );

    void            ReadColorTexture    ( unsigned int i, Renderer * renderer, Texture2D*& outputTex );

private:

    GLuint          GetPrevTexture      () const;

    void            AddColorAttachments ( Renderer * renderer, RenderTarget * rt );
    void            AddDepthBuffer      ( Renderer * renderer, RenderTarget * rt );
    bool            FramebuferStatusOK  () const;

};

} //bv
